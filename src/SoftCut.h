//
// Created by emb on 11/10/18.
//

#ifndef SOFTCUT_SOFTCUT_H
#define SOFTCUT_SOFTCUT_H

#include "SoftCutVoice.h"

namespace softcut {

    class SoftCut {

    private:

    public:
        enum { numVoices = 2};
        enum {  BUF_FRAMES = 16777216 };
        float buf[BUF_FRAMES];
        SoftCutVoice scv[numVoices];
        void init ();
    };
}


#endif //SOFTCUT_SOFTCUT_H
