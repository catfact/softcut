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
        // constructor
        Resampler(float *buf, int frames);
        int processFrame(float x);
        void setRate(double r);
        void setBuffer(float *buf, int frames);
        void setPhase(double phase);
        float* buffer();
        int bufferFrames();
        int frame();
        void reset();

    private:
        // output buffer
        float *buf_;
        // total frames in output buffer
        int bufFrames_;
        // output:input ratio
        double rate_;
        // last written phase
        double phase_;
        // last output frame
        int frame_;
        // last input value
        float x_;

    private:

        static int wrap(int val, int bound);
        // write multiple output frames, interpolating between two values
        // FIXME: allow higher-order interpolation
        void writeInterp(float x, int n);

        // write, upsampling
        // return frames written (>= 1)
        // assume rate_ > 1.0
        int writeUp(float x);

        // write, downsampling
        // return frames written (0 or 1)
        // we assume rate_ < 1.0
        int writeDown(float x);

        void write(float x);
   
    };

}

#endif //SOFTCUTHEAD_RESAMPLER_H

