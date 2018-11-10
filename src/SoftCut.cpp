//
// Created by ezra on 11/3/18.
//
// main audo
//

#include "SoftCut.h"
#include "Commands.h"

using namespace softcut;

SoftCut::SoftCut() {
    fcBase = 16000;
    sch.init();
    sch.setBuffer(buf, BUF_FRAMES);
    svf.setLpMix(1.0);
    svf.setHpMix(0.0);
    svf.setBpMix(0.0);
    svf.setBrMix(0.0);
    svf.setRq(20.0);
    svf.setFc(fcBase);
    svfDryLevel = 1.0;
}

void SoftCut::processBlockMono(float *in, float *out, int numFrames) {
    float trigDummy;
    float phaseDummy;

    Commands::handlePending(this);

    float x;
    for(int i=0; i<numFrames; ++i) {
#if 1
        x = svf.getNextSample(in[i]) + in[i]*svfDryLevel;
#else
        x = in[i];
#endif
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
    updateFilterFc();
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
    sch.cutToPos(sec);
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

void SoftCut::setFilterFc(float x) {
    fcBase = x;
    updateFilterFc();
}

void SoftCut::setFilterRq(float x) {
    svf.setRq(x);
}

void SoftCut::setFilterLp(float x) {
    svf.setLpMix(x);
}

void SoftCut::setFilterHp(float x) {
    svf.setHpMix(x);
}

void SoftCut::setFilterBp(float x) {
    svf.setBpMix(x);
}

void SoftCut::setFilterBr(float x) {
    svf.setBrMix(x);
}

void SoftCut::setFilterDry(float x) {
    svfDryLevel = x;
}

void SoftCut::setFilterFcMod(float x) {
    fcMod = x;
}

void SoftCut::updateFilterFc() {
    float fc = std::min(fcBase, fcBase * std::fabs(sch.getRate()));
    // std::cout << fc << std::endl;
    svf.setFc(fc*fcMod + (1.f-fcMod )*svf.getFc());
}

