//
// Created by emb on 11/10/18.
//

#include "SoftCut.h"
#include "Commands.h"

using namespace softcut;

SoftCut::SoftCut() {
    init();
}

void SoftCut::init() {

    for (auto &v : scv) {
        v.setBuffer(buf, bufFrames);
    }
}

void SoftCut::processBlock(const float *in0, const float* in1, float *out0, float* out1, int numFrames) {
    (void)in1;
    Commands::handlePending(this);
#if 0
    scv[1].processBlockMono(in, out, numFrames);
#else
    for (int v=0; v<numVoices; ++v) {
        scv[v].processBlockMono(in0, outBus, numFrames);
        for(int fr=0; fr<numFrames; ++fr) {
            out0[fr] = outBus[fr] * outAmp[v][0];
            out1[fr] = outBus[fr] * outAmp[v][1];
        }
    }
#endif

}

void SoftCut::setSampleRate(unsigned int hz) {
    for (auto &v : scv) {
        v.setSampleRate(hz);
    }
}

void SoftCut::setRate(int voice, float rate) {
  scv[voice].setRate( rate);
}

void SoftCut::setLoopStart(int voice, float sec) {
  scv[voice].setLoopStart( sec);
}

void SoftCut::setLoopEnd(int voice, float sec) {
  scv[voice].setLoopEnd( sec);
}

void SoftCut::setLoopFlag(int voice, bool val) {
  scv[voice].setLoopFlag( val);
}

void SoftCut::setFadeTime(int voice, float sec) {
  scv[voice].setFadeTime( sec);
}

void SoftCut::setRecLevel(int voice, float amp) {
  scv[voice].setRecLevel( amp);
}

void SoftCut::setPreLevel(int voice, float amp) {
  scv[voice].setPreLevel( amp);
}

void SoftCut::setRecFlag(int voice, bool val) {
  scv[voice].setRecFlag( val);
}

void SoftCut::cutToPos(int voice, float sec) {
    scv[voice].cutToPos(sec);
}

void SoftCut::setFilterFc(int voice, float x) {
  scv[voice].setFilterFc(x);
}

void SoftCut::setFilterRq(int voice, float x) {
  scv[voice].setFilterRq(x);
}

void SoftCut::setFilterLp(int voice, float x) {
  scv[voice].setFilterLp(x);
}

void SoftCut::setFilterHp(int voice, float x) {
  scv[voice].setFilterHp(x);
}

void SoftCut::setFilterBp(int voice, float x) {
  scv[voice].setFilterBp(x);
}

void SoftCut::setFilterBr(int voice, float x) {
  scv[voice].setFilterBr(x);
}

void SoftCut::setFilterDry(int voice, float x) {
  scv[voice].setFilterDry(x);
}

void SoftCut::setFilterFcMod(int voice, float x) {
  scv[voice].setFilterFcMod( x);
}

void SoftCut::setAmpLeft(int voice, float x) {
    outAmp[voice][0] = x;
}

void SoftCut::setAmpRight(int voice, float x) {
    outAmp[voice][1] = x;
}
