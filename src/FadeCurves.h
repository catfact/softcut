//
// Created by ezra on 11/15/18.
//
// static class for producing curves in fade period
//
// FIXME: this should be an object owned by SoftCutHead, passed to child SubHeads


#ifndef SOFTCUT_FADECURVES_H
#define SOFTCUT_FADECURVES_H

namespace softcut {

    class FadeCurves {
    public:
        static void setRecDelayRatio(float x);
        static void setPreWindowRatio(float x);
        static void setMinRecDelayFrames(unsigned int x);
        static void setMinPreWindowFrames(unsigned int x);

        // x is assumed to be in [0,1]
        static float getRecFadeValue(float x);

        static float getPreFadeValue(float x);

    private:
        static void calcPreFade();

        static void calcRecFade();

    private:

        // xfade curve buffers
        static constexpr unsigned int fadeBufSize = 1001;
        // record delay and pre window in fade, as proportion of fade time
        static float recDelayRatio;
        static float preWindowRatio;
        // minimum record delay/pre window, in frames
        static unsigned int recDelayMinFrames;
        static unsigned int preWindowMinFrames;
        static float recFadeBuf[fadeBufSize];
        static float preFadeBuf[fadeBufSize];
    };
}

#endif //SOFTCUT_FADECURVES_H
