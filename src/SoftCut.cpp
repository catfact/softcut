//
// Created by ezra on 11/3/18.
//

#include "SoftCut.h"
#include "Commands.h"

using namespace softcut;

SoftCut::SoftCut() {
    fcBase = 16000;
    sch.init();
    sch.setBuffer(buf, BUF_FRAMES);
    svf.setLpMix(0.0);
    svf.setHpMix(0.0);
    svf.setBpMix(1.0);
    svf.setBrMix(0.0);
    svf.setRq(20.0);
    svf.setFc(fcBase);
}

void SoftCut::processBlockMono(float *in, float *out, int numFrames) {
    float trigDummy;
    float phaseDummy;

    Commands::handlePending(this);

    float x;
    for(int i=0; i<numFrames; ++i) {
        //x = in[i];
        x = svf.getNextSample(in[i]);
        sch.nextSample(x, &phaseDummy, &trigDummy, &(out[i]));
    }
}

void SoftCut::setSampleRate(float hz) {
    sampleRate = hz;
    sch.setSampleRate(sampleRate);
    svf.setSampleRate(hz);
}

void SoftCut::setRate(float rate) {
    sch.setRate(rate);
    float fc = std::min(fcBase, fcBase * std::fabs(rate));
    std::cout << fc << std::endl;
    svf.setFc(fc);
}

void SoftCut::setLoopStart(float sec) {
    sch.setLoopStartSeconds(sec);
}

void SoftCut::setLoopEnd(float sec) {
    sch.setLoopEndSeconds(sec);
}

void SoftCut::setFadeTime(float sec) {
    sch.setFadeTime(sec);
}

void SoftCut::cutToPos(float sec) {
    // FIXME: wrap this...
    sch.cutToPhase(sec * sampleRate);
}

void SoftCut::setRecLevel(float amp) {
    sch.setRec(amp);
}

void SoftCut::setPreLevel(float amp) {
    sch.setPre(amp);
}

void SoftCut::setRecFlag(bool val) {
    sch.setRecRun(val);
}

void SoftCut::setLoopFlag(bool val) {
    sch.setLoopFlag(val);
}

void SoftCut::setRecOffset(int samples) {
    sch.setRecOffset(samples);
}
