//
// Created by emb on 11/10/18.
//

#include "SoftCut.h"

using namespace softcut;

SoftCut::SoftCut() {
    init();
}

void SoftCut::init() {

    for (auto &v : scv) {
        v.setBuffer(buf, BUF_FRAMES);
    }
}

void SoftCut::processBlockMono(float *in, float *out, int numFrames) {
#if 1
    scv[1].processBlockMono(in, out, numFrames);
#else
    for (auto &v : scv) {
        // TODO
    }
#endif

}

void SoftCut::setSampleRate(unsigned int hz) {
    for (auto &v : scv) {
        v.setSampleRate(hz);
    }
}
