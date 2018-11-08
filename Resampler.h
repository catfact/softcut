//
// Created by ezra on 4/21/18.
//

#ifndef SOFTCUTHEAD_RESAMPLER_H
#define SOFTCUTHEAD_RESAMPLER_H

#include <iostream>
#include <cmath>

// ultra-simple linear-interpolated resampling class
// works on mono output buffer and processes one input sample at a time

namespace softcut {

    class Resampler {

    public:

        enum {
            IN_BUF_FRAMES = 2, // limits interpolation order
            OUT_BUF_FRAMES = 64 // limits resampling ratio
        };

        // constructor
        Resampler(float *buf, int frames);
        int processFrame(float x);
        void setRate(double r);
        // void setBuffer(float *buf, int frames);
        void setPhase(double phase);
        const float* output();
        void reset();

    private:
        // output buffer
        float buf_[OUT_BUF_FRAMES];
        double rate_;
        // phase increment
        double phi_;
        // last written phase
        double phase_;
        // current input value
        float x_;
        // last input value
        float x_1_;

    private:
        // push an input value
        void push(float x);

        // interpolate the most recent input samples
        // @param f in [0, 1]
        float interp(float f);

        // write, upsampling
        // return frames written (>= 1)
        // assumptions: input has been pushed. rate_ > 1.0
        int writeUp();

        // write, downsampling
        // return frames written (0 or 1)
        // assumptions: input has been pushed. rate_ <= 1.0
        int writeDown();

    };

}

#endif //SOFTCUTHEAD_RESAMPLER_H

