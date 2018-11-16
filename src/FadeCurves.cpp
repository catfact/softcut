//
// Created by ezra on 11/15/18.
//

#include <math.h>
#include <algorithm>

#include "Interpolate.h"
#include "FadeCurves.h"

using namespace softcut;

static constexpr float fpi = 3.1415926535898f;

float FadeCurves::recDelayRatio;
float FadeCurves::preWindowRatio;
unsigned int FadeCurves::recDelayMinFrames;
unsigned int FadeCurves::preWindowMinFrames;
float FadeCurves::recFadeBuf[fadeBufSize];
float FadeCurves::preFadeBuf[fadeBufSize];

void FadeCurves::calcRecFade() {
    unsigned int n = fadeBufSize - 1;
    // build rec-fade curve
    // this will be scaled by base rec level
    unsigned int ndr = std::max(recDelayMinFrames,
                                static_cast<unsigned int>(recDelayRatio * fadeBufSize));
    unsigned int nr = n - ndr;
    const float phi = fpi / nr;
    float x = fpi;
    float y = 0.f;
    unsigned int i = 0;
    while (i < ndr) {
        recFadeBuf[i++] = y;
    }
    while (i < n) {
        y = cosf(x) * 0.5f + 0.5f;
        recFadeBuf[i++] = y;
        x += phi;
    }
    recFadeBuf[n] = 1.f;
}

void FadeCurves::calcPreFade() {
    // build pre-fade curve
    // this will be scaled and added to the base pre value (mapping [0, 1] -> [pre, 1])
    unsigned int nwp = std::max(preWindowMinFrames,
                                static_cast<unsigned int>(preWindowRatio * fadeBufSize));
    float x = 0.f;
    const float phi = fpi / nwp;
    unsigned int i = 0;
    while(i <nwp) {
        preFadeBuf[i++] = cosf(x) * 0.5f + 0.5f;
        x += phi;
    }
    while(i<fadeBufSize) { preFadeBuf[i++] = 0.f; }

}

void FadeCurves::setRecDelayRatio(float x) {
    recDelayRatio = x;
    calcRecFade();
}

void FadeCurves::setPreWindowRatio(float x) {
    preWindowRatio = x;
    calcPreFade();
}

void FadeCurves::setMinRecDelayFrames(unsigned int x) {
    recDelayMinFrames = x;
    calcRecFade();
}

void FadeCurves::setMinPreWindowFrames(unsigned int x) {
    preWindowMinFrames = x;
    calcPreFade();
}

float FadeCurves::getRecFadeValue(float x) {
    return Interpolate::tabLinear<float, fadeBufSize>(recFadeBuf, x);
}


float FadeCurves::getPreFadeValue(float x) {
    return Interpolate::tabLinear<float, fadeBufSize>(preFadeBuf, x);
}
