//
// Created by emb on 11/10/18.
//

#ifndef SOFTCUT_SOFTCUT_H
#define SOFTCUT_SOFTCUT_H

#include "SoftCutVoice.h"

namespace softcut {

    class SoftCut {


    public:
        enum { numVoices = 2 };
        enum {  BUF_FRAMES = 16777216 };


        SoftCut();

        void init();

        void processBlockMono(float* in, float* out, int numFrames);

        void setSampleRate(unsigned int i);

    private:
        SoftCutVoice scv[numVoices];
        float buf[BUF_FRAMES];
    };
}


#endif //SOFTCUT_SOFTCUT_H
