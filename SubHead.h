//
// Created by ezra on 4/21/18.
//

/*
 * this class implements one half of a crossfaded read/write head.
 */

#ifndef SOFTCUTHEAD_SUBHEAD_H
#define SOFTCUTHEAD_SUBHEAD_H

#include "Resampler.h"

namespace softcut {

    typedef enum { ACTIVE=0, INACTIVE=1, FADEIN=2, FADEOUT=3 } State;
    typedef enum { NONE, STOP, LOOP_POS, LOOP_NEG } Action ;

    class SubHead {
        friend class SoftCutHeadLogic;

    public:
        SubHead();
        void init();

    private:
        float peek4(double phase);
        unsigned int wrapBufIndex(int x);

    protected:
        float peek();
        void poke(float in, float pre, float rec, float fadePre, float fadeRec);
        Action updatePhase(double start, double end, bool loop);
        void updateFade(double inc);

        // getters
        double phase() { return phase_; }
        float fade() { return fade_; }
        float trig() { return trig_; }
        State state() { return state_; }
        
        // setters
        void setState(State state) { state_ = state; }
        
        void setPhase(double phase) {
            phase_ = phase;
            // on phase change, the resampler should clear and reset its internal ringbuffer
            resamp_.reset();
        }

        // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        // **NB** buffer size must be a power of two!!!!
        void setBuffer(float *buf, unsigned int frames) {
            buf_  = buf;
            bufFrames_ = frames;
            bufMask_ = frames - 1;
        }

        void setRate(float rate) {
            rate_ = rate;
            // NB: resampler doesn't handle negative rates.
            // instead we copy the resampler output backwards into the buffer when rate < 0.
            resamp_.setRate(std::fabs(rate));
        }

	void setTrig(float trig) { trig_ = trig; }

    private:
        float * buf_; // output buffer
        unsigned int bufFrames_;
        unsigned int bufMask_;
        Resampler resamp_;
        State state_;
        double rate_;
        double phase_;
        float fade_;
        float trig_; // output trigger value
        bool active_;


    };

}


#endif //SOFTCUTHEAD_SUBHEAD_H
