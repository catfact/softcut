//
// Created by ezra on 11/3/18.
//
// main audo
//

#include "Commands.h"
#include "SoftCutVoice.h"

using namespace softcut;

SoftCutVoice::SoftCutVoice() {
    fcBase = 16000;
    sch.init();
    svf.setLpMix(1.0);
    svf.setHpMix(0.0);
    svf.setBpMix(0.0);
    svf.setBrMix(0.0);
    svf.setRq(20.0);
    svf.setFc(fcBase);
    svfDryLevel = 1.0;
}

void SoftCutVoice::processBlockMono(float *in, float *out, int numFrames) {
    float trigDummy;
    float phaseDummy;

    /// FIXME: need to handle in parent; commands need voice ids...
    Commands::handlePending(this);

    float x;
    for(int i=0; i<numFrames; ++i) {
#if 1
        x = svf.getNextSample(in[i]) + in[i]*svfDryLevel;
#else
        x = in[i];
#endif
        sch.processSample(x, &phaseDummy, &trigDummy, &(out[i]));
    }
}

void SoftCutVoice::setSampleRate(float hz) {
    sampleRate = hz;
    sch.setSampleRate(sampleRate);
    svf.setSampleRate(hz);
}

void SoftCutVoice::setRate(float rate) {
    sch.setRate(rate);
    updateFilterFc();
}

void SoftCutVoice::setLoopStart(float sec) {
    sch.setLoopStartSeconds(sec);
}

void SoftCutVoice::setLoopEnd(float sec) {
    sch.setLoopEndSeconds(sec);
}

void SoftCutVoice::setFadeTime(float sec) {
    sch.setFadeTime(sec);
}

void SoftCutVoice::cutToPos(float sec) {
    sch.cutToPos(sec);
}

void SoftCutVoice::setRecLevel(float amp) {
    sch.setRec(amp);
}

void SoftCutVoice::setPreLevel(float amp) {
    sch.setPre(amp);
}

void SoftCutVoice::setRecFlag(bool val) {
    sch.setRecRun(val);
}

void SoftCutVoice::setLoopFlag(bool val) {
    sch.setLoopFlag(val);
}

void SoftCutVoice::setFilterFc(float x) {
    fcBase = x;
    updateFilterFc();
}

void SoftCutVoice::setFilterRq(float x) {
    svf.setRq(x);
}

void SoftCutVoice::setFilterLp(float x) {
    svf.setLpMix(x);
}

void SoftCutVoice::setFilterHp(float x) {
    svf.setHpMix(x);
}

void SoftCutVoice::setFilterBp(float x) {
    svf.setBpMix(x);
}

void SoftCutVoice::setFilterBr(float x) {
    svf.setBrMix(x);
}

void SoftCutVoice::setFilterDry(float x) {
    svfDryLevel = x;
}

void SoftCutVoice::setFilterFcMod(float x) {
    fcMod = x;
}

void SoftCutVoice::updateFilterFc() {
    float fc = std::min(fcBase, fcBase * std::fabs(sch.getRate()));
    // std::cout << fc << std::endl;
    svf.setFc(fc*fcMod + (1.f-fcMod )*svf.getFc());
}

void SoftCutVoice::setBuffer(float *b, int nf) {
    buf = b;
    bufFrames = nf;
    sch.setBuffer(buf, bufFrames);
}

