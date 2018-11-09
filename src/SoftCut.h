//
// Created by ezra on 11/3/18.
//

#ifndef SOFTCUT_SOFTCUT_H
#define SOFTCUT_SOFTCUT_H

#include <array>
#include "SoftCutHeadLogic.h"
#include "Svf.h"

namespace softcut {
    class SoftCut {
    public:
        SoftCut();

        void setSampleRate(float hz);
        void setRate(float rate);
        void setLoopStart(float sec);
        void setLoopEnd(float sec);
        void setLoopFlag(bool val);

        void setFadeTime(float sec);

        void setRecLevel(float amp);
        void setPreLevel(float amp);
        void setRecFlag(bool val);
        void setRecOffset(int samples);

        void cutToPos(float sec);
        // process a single channel
        void processBlockMono(float* in, float* out, int numFrames);

    private:
        enum {  BUF_FRAMES = 16777216 };
        float buf[BUF_FRAMES];
        float sampleRate;

        // first try: one head only
        SoftCutHeadLogic sch;
        Svf svf;
        // default frequency for SVF
        // reduced automatically when setting rate
        float fcBase;
    };
}


#endif //SOFTCUT_SOFTCUT_H
