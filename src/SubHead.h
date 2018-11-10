//
// Created by ezra on 4/21/18.
//

/*
 * this class implements one half of a crossfaded read/write sch.
 */

#ifndef SOFTCUTHEAD_SUBHEAD_H
#define SOFTCUTHEAD_SUBHEAD_H

#include <boost/math/special_functions/sign.hpp>

#include "Resampler.h"
#include "LowpassBrickwall.h"
#include "SoftClip.h"

namespace softcut {

    typedef enum { ACTIVE=0, INACTIVE=1, FADEIN=2, FADEOUT=3 } State;
    typedef enum { NONE, STOP, LOOP_POS, LOOP_NEG } Action ;

    class SubHead {
        friend class SoftCutHead;

    public:
        SubHead();
        void init();
        void setSampleRate(float sr);
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
        void setState(State state);
        void setTrig(float trig);
        void setPhase(double phase);

        // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        // **NB** buffer size must be a power of two!!!!
        void setBuffer(float *buf, unsigned int frames);
        void setRate(float rate);


    private:
        Resampler resamp_;
        LowpassBrickwall lpf_;
        SoftClip clip_;

        float * buf_; // output buffer
        unsigned int idx_; // write index
        unsigned int bufFrames_;
        unsigned int bufMask_;


        State state_;
        double rate_;
        int inc_;
        double phase_;
        float fade_;
        float trig_; // output trigger value
        bool active_;
    };

}


#endif //SOFTCUTHEAD_SUBHEAD_H
