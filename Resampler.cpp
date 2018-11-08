#include "Resampler.h"

using namespace softcut;

//----------
//-- public

Resampler::Resampler(float *buf, int frames) :
                buf_(buf), bufFrames_(frames),
                rate_(1.0), phase_(0.0), frame_(0) {}

void Resampler::setRate(double r) {
    rate_ = r;
    phi_ = 1.0 / r;
}

void Resampler::setBuffer(float *buf, int frames) {
    buf_  = buf;
    bufFrames_ = frames;
}
void Resampler::setPhase(double phase) { phase_ = phase; }

float* Resampler::buffer() { return buf_; }

int Resampler::bufferFrames() { return bufFrames_; }

int Resampler::frame() { return frame_; }

void Resampler::reset() {
    frame_ = 0;
    x_ = 0.f;
}


int Resampler::processFrame(float x) {
    push(x);
    if(rate_ > 1.0) {
        return writeUp();
    } else {
        return writeDown();
    }
}

//--------------
//--- private


int Resampler::writeUp() {
    double p = phase_ + rate_;
    int nf = static_cast<int>(p);
    // we can assume that n >= 1 here
    // we want to track fractional output phase for interpolation
    // this is normalized to the distance between input frames
    // so: the distance to the first output frame boundary:
    double f = 1.0 - phase_;
    // normalized (divided by rate);
    f = f * phi_;
    // write the first output frame
    unsigned int i=0;
    buf_[i] = interp(static_cast<float>(f));
    i++;
    while(i < nf) {
        // distance between output frames in this normalized space is 1/rate
        f += phi_;
        buf_[i] = interp(static_cast<float>(f));
        i++;
    }
    // store the remainder of the updated, un-normalized output phase
    phase_ = p - static_cast<double>(nf);
    return nf;
}

int Resampler::writeDown() {
    // number of frames will be 1 or 0.
    // as with upsampling inner loop,
    // we need to produce a fractional interpolation coefficient,
    // by "normalizing" to the output phase period
    double p = phase_ + rate_;
    auto nf = static_cast<unsigned int>(p);
    if (nf > 0) {
        float f = 1.f - static_cast<float>(p);
        f *= phi_;
        buf_[0] = interp(f);
        phase_ = p - static_cast<double>(nf);
        return 1;
    } else {
        phase_ = p;
        return 0;
    }
}

void Resampler::push(float x) {
    x_1_ = x_;
    x_ = x;
}

float Resampler::interp(float f) {
    return x_1_ + (x_ - x_1_)*f;
}