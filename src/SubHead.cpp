//
// Created by ezra on 4/21/18.
//

#include <string.h>
#include <limits>

#include "Interpolate.h"
#include "SubHead.h"

using namespace softcut;

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
    // hm... actually we should always be pushing input to both/all resamplers... right?
    // (which in turn suggests they should share an input ringbuffer? (FIXME))
    int nframes = resamp_.processFrame(in);

    if (fade_ < std::numeric_limits<float>::epsilon()) { return; }
    if (rec < std::numeric_limits<float>::epsilon()) { return; }

    if(state_ == INACTIVE) {
        return;
    }

    float fadeInv = 1.f - fade_;
    float preFade = std::fmax(pre, (pre * fadeInv));
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

void SubHead::reset() {
    resamp_.reset();
}
