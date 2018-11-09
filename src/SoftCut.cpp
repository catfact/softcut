//
// Created by ezra on 11/3/18.
//

#include "SoftCut.h"
#include "Commands.h"

using namespace softcut;


SoftCut::SoftCut() {
    head.init();
    head.setBuffer(buf, BUF_FRAMES);
}

void SoftCut::processBlockMono(float *in, float *out, int numFrames) {
    // FIXME: save the trigger / phase outputs for anaysis
    float trigDummy;
    float phaseDummy;

    Commands::handlePending(this);

    for(int i=0; i<numFrames; ++i) {
        head.nextSample(in[i], &phaseDummy, &trigDummy, &(out[i]));
    }
}

void SoftCut::setSampleRate(float hz) {
    sampleRate = hz;
    head.setSampleRate(sampleRate);
}

void SoftCut::setRate(float rate) {
    head.setRate(rate);
}

void SoftCut::setLoopStart(float sec) {
    head.setLoopStartSeconds(sec);
}

void SoftCut::setLoopEnd(float sec) {
    head.setLoopEndSeconds(sec);
}

void SoftCut::setFadeTime(float sec) {
    head.setFadeTime(sec);
}

void SoftCut::cutToPos(float sec) {
    // FIXME: wrap this...
    head.cutToPhase(sec * sampleRate);
}

void SoftCut::setRecLevel(float amp) {
    head.setRec(amp);
}

void SoftCut::setPreLevel(float amp) {
    head.setPre(amp);
}

void SoftCut::setRecFlag(bool val) {
    head.setRecRun(val);
}

void SoftCut::setLoopFlag(bool val) {
    head.setLoopFlag(val);
}

void SoftCut::setRecOffset(int samples) {
    head.setRecOffset(samples);
}
