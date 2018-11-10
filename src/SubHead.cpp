//
// Created by ezra on 4/21/18.
//

#include <string.h>
#include <limits>

#include "Interpolate.h"
#include "SubHead.h"

using namespace softcut;

static inline bool isSmall(float f) { return f <std::numeric_limits<float>::epsilon(); }

SubHead::SubHead() {
    this->init();
}

void SubHead::init() {
    phase_ = 0;
    fade_ = 0;
    trig_ = 0;
    state_ = INACTIVE;
    resamp_.setPhase(0);
    inc_ = 1;
}

Action SubHead::updatePhase(double start, double end, bool loop) {
    Action res = NONE;
    trig_ = 0.f;
    double p;
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

void SubHead::updateFade(double inc) {
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

void SubHead::poke(float in, float pre, float rec, float fadePre, float fadeRec) {
    // FIXME: since there's never really a reason to not push input, or to reset input rinbuf,
    // it follows that all resamplers can share an input ringbuf
    int nframes = resamp_.processFrame(in);

    if(state_ == INACTIVE) {
        return;
    }

    float fadeInv = 1.f - fade_;

    // pre-level should have its own fade in/out
    // the top of this fade should be 1
    // and the button should be the specified prelevel
    float preFade = fadeInv * (1.f - pre) + pre;
    float recFade = rec * fade_;
    float y; // write value
    const float* src = resamp_.output();
    for(int i=0; i<nframes; ++i) {
        y = src[i];

#if 1 // soft clipper
        y = clip_.processSample(y);
#endif
#if 0 // lowpass filter
        lpf_.processSample(&y);
#endif
        buf_[idx_] *= preFade;
        buf_[idx_] += y * recFade;
        idx_ = wrapBufIndex(idx_ + inc_);
    }
}

float SubHead::peek() {
    return peek4(phase_);
}

float SubHead::peek4(double phase) {
    int phase1 = static_cast<int>(phase_);
    int phase0 = phase1 - 1;
    int phase2 = phase1 + 1;
    int phase3 = phase1 + 2;

    double y0 = buf_[wrapBufIndex(phase0)];
    double y1 = buf_[wrapBufIndex(phase1)];
    double y3 = buf_[wrapBufIndex(phase3)];
    double y2 = buf_[wrapBufIndex(phase2)];

    double x = phase_ - (double)phase1;
    return static_cast<float>(Interpolate::hermite(x, y0, y1, y2, y3));
}

unsigned int SubHead::wrapBufIndex(int x) {
    x += bufFrames_;
    assert(x >= 0);
    return x & bufMask_;
}

void SubHead::setSampleRate(float sr) {
    lpf_.init(static_cast<int>(sr));
}

void SubHead::setPhase(double phase) {
    phase_ = phase;
    // FIXME?: magic number hack here for small record offset
    idx_ = wrapBufIndex(static_cast<int>(phase_) - inc_ * 8);
    // std::cout << "pos change; phase=" << phase_ << "; inc=" << inc_ << "; idx=" << idx_ << std::endl;
    BOOST_ASSERT_MSG(isSmall(fade_), "changing phase with fade>0");
    // on phase change, the resampler should clear and reset its internal ringbuffer

    // actually this seems unnecessary and maybe wrong...
    // it's ok to keep history of input when changing positions.
    // resmp output doesn't need clearing b/c we write/read from beginning oneach sample anyway
    // resamp_.reset();

    // hm...
    resamp_.setPhase(0);
}

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// **NB** buffer size must be a power of two!!!!
void SubHead::setBuffer(float *buf, unsigned int frames) {
    buf_  = buf;
    bufFrames_ = frames;
    bufMask_ = frames - 1;
    assert((bufFrames_ != 0) && !(bufFrames_ & bufMask_));
}

void SubHead::setRate(float rate) {
    rate_ = rate;
    inc_ = boost::math::sign(rate);
    // NB: resampler doesn't handle negative rates.
    // instead we copy the resampler output backwards into the buffer when rate < 0.
    resamp_.setRate(std::fabs(rate));
}


void SubHead::setState(State state) { state_ = state; }
void SubHead::setTrig(float trig) { trig_ = trig; }
