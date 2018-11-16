//
// Created by ezra on 4/21/18.
//

#include <string.h>
#include <limits>

#include "Interpolate.h"
#include "FadeCurves.h"
#include "SubHead.h"

using namespace softcut;

// FIXME refactor
// equal-power fade,
// x = value at start
// y = value at end
// c = coefficient in [0, 1]
static float epfade(float x, float y, float c) {
    c *= M_PI_2;
    return x * cosf(c) + y * sinf(c);
}

SubHead::SubHead() {
    this->init();
}

void SubHead::init() {
    phase_ = 0;
    fade_ = 0;
    trig_ = 0;
    state_ = INACTIVE;
    resamp_.setPhase(0);
    inc_dir_ = 1;
}

Action SubHead::updatePhase(phase_t start, phase_t end, bool loop) {
    Action res = NONE;
    trig_ = 0.f;
    phase_t p;
    switch(state_) {
        case FADEIN:
        case FADEOUT:
        case ACTIVE:
            p = phase_ + rate_;
            if(active_) {
                // FIXME: should refactor this a bit. 
                if (rate_ > 0.f) {
                    if (p > end || p < start) {
                        if (loop) {
                            trig_ = 1.f;
                            res = LOOP_POS;
                        } else {
                            state_ = FADEOUT;
                            res = STOP;
                        }
                    }
                } else { // negative rate
                    if (p > end || p < start) {
                        if(loop) {
                            trig_ = 1.f;
                            res = LOOP_NEG;
                        } else {
                            state_ = FADEOUT;
                            res = STOP;
                        }
                    }
                } // rate sign check
            } // /active check
            phase_ = p;
            break;
        case INACTIVE:
        default:
            ;; // nothing to do
    }
    return res;
}

void SubHead::updateFade(float inc) {
    switch(state_) {
        case FADEIN:
            fade_ += inc;
            if (fade_ > 1.f) {
                fade_ = 1.f;
                state_ = ACTIVE;
            }
            break;
        case FADEOUT:
            fade_ -= inc;
            if (fade_ < 0.f) {
                fade_ = 0.f;
                state_ = INACTIVE;
            }
            break;
        case ACTIVE:
        case INACTIVE:
        default:;; // nothing to do
    }
}

void SubHead::poke(float in, float pre, float rec, int numFades) {
    // FIXME: since there's never really a reason to not push input, or to reset input ringbuf,
    // it follows that all resamplers can share an input ringbuf
    int nframes = resamp_.processFrame(in);

    if(state_ == INACTIVE) {
        return;
    }

    float preFade;
    float recFade;

    if(std::fabsf(0.5f-fade_) < 0.1) {
        int hello = 0;
        ++hello;
    }
    preFade = pre + (1.f-pre) * FadeCurves::getPreFadeValue(fade_);
    recFade = rec * FadeCurves::getRecFadeValue(fade_);

    sample_t y; // write value
    const sample_t* src = resamp_.output();

    for(int i=0; i<nframes; ++i) {
        y = src[i];

#if 1 // soft clipper
        y = clip_.processSample(y);
#endif
#if 1 // lowpass filter
        lpf_.processSample(&y);
#endif
        buf_[idx_] *= preFade;
        buf_[idx_] += y * recFade;

        idx_ = wrapBufIndex(idx_ + inc_dir_);
    }
}

float SubHead::peek() {
    return peek4();
}

float SubHead::peek4() {
    int phase1 = static_cast<int>(phase_);
    int phase0 = phase1 - 1;
    int phase2 = phase1 + 1;
    int phase3 = phase1 + 2;

    float y0 = buf_[wrapBufIndex(phase0)];
    float y1 = buf_[wrapBufIndex(phase1)];
    float y3 = buf_[wrapBufIndex(phase3)];
    float y2 = buf_[wrapBufIndex(phase2)];

    auto x = static_cast<float>(phase_ - (float)phase1);
    return Interpolate::hermite<float>(x, y0, y1, y2, y3);
}

unsigned int SubHead::wrapBufIndex(int x) {
    x += bufFrames_;
    BOOST_ASSERT_MSG(x >= 0, "buffer index before masking is non-negative");
    return x & bufMask_;
}

void SubHead::setSampleRate(float sr) {
    lpf_.init(static_cast<int>(sr));
}

void SubHead::setPhase(phase_t phase) {
    phase_ = phase;
    // FIXME?: magic number hack here for small record offset
    idx_ = wrapBufIndex(static_cast<int>(phase_) - inc_dir_ * 8);
    // std::cout << "pos change; phase=" << phase_ << "; inc=" << inc_ << "; idx=" << idx_ << std::endl;

    // FIXME: we are hitting this sometimes. fade is always quite small...
    // rounding error? wrong order of calculations?
#if 0
    if(fade_ > std::numeric_limits<float>::epsilon()) {
        std::cerr << "fade=" << fade_ << std::endl;
        BOOST_ASSERT_MSG(false, "changing phase with fade>0");
    }
#endif

    // NB: not resetting the resampler here:
    // - it's ok to keep history of input when changing positions.
    // - resmp output doesn't need clearing b/c we write/read from beginning on each sample anyway
}

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// **NB** buffer size must be a power of two!!!!
void SubHead::setBuffer(float *buf, unsigned int frames) {
    buf_  = buf;
    bufFrames_ = frames;
    bufMask_ = frames - 1;
    BOOST_ASSERT_MSG((bufFrames_ != 0) && !(bufFrames_ & bufMask_), "buffer size is not 2^N");
}

void SubHead::setRate(float rate) {
    rate_ = rate;
    inc_dir_ = boost::math::sign(rate);
    // NB: resampler doesn't handle negative rates.
    // instead we copy the resampler output backwards into the buffer when rate < 0.
    resamp_.setRate(std::fabs(rate));
}


void SubHead::setState(State state) { state_ = state; }
void SubHead::setTrig(float trig) { trig_ = trig; }
