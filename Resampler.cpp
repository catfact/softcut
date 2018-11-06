#include "Resampler.h"

using namespace softcut;

//----------
//-- public

Resampler::Resampler(float *buf, int frames) :
                buf_(buf), bufFrames_(frames),
                rate_(1.0), phase_(0.0), frame_(0) {}

int Resampler::processFrame(float x) {
    //frame_ = wrap(frame_, bufFrames_);
    frame_ %= bufFrames_;
    if (rate_ > 1.0) {
        return writeUp(x);
    } else if (rate_ < 1.0) {
        return writeDown(x);
    } else {
        write(x);
        return 1;
    }
}
void Resampler::setRate(double r) {
    rate_ = r;
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

//--------------
//--- private

int Resampler::wrap(int val, int bound) {
    int x = val;
    while(x >= bound) { x -= bound; }
    while(x < 0) { return x += bound; }
    return x;
}

void Resampler::writeInterp(float x, int n) {
    int i = frame_;                 // index into buffer
    double m = (x - x_) / rate_;    // slope
    double y;                       // interpolated value
    // interpolate up to first frame boundary; distance is 1-(old phase)
    y = x_ + m * (1.0 - phase_);
    buf_[i] = y;
    n--;
    // for the rest,
    while (n > 0) {
        y += m;

        i = (i + 1) % bufFrames_;
        buf_[i] = y;
        n--;
    }
}

int Resampler::writeUp(float x) {
    double phase = phase_ + rate_;
    int nframes = (int) phase;
    writeInterp(x, nframes);
    phase_ = phase - std::floor(phase);
    frame_ = (frame_ + nframes) % bufFrames_;
    x_ = x;
    return nframes;
}

int Resampler::writeDown(float x) {
    double phase = phase_ + rate_;
    int nframes = (int) phase;
    if (nframes > 0) {
        // use linear interpolation from last written value
        // FIXME: use higher order interpolation.
        // this would require enforcing a higher minimum latency...
        double m = (x - x_) / rate_;
        buf_[frame_] = x + (m * (1.0 - phase_));
        frame_ = (frame_ + 1) % bufFrames_;
    }
    phase_ = phase - std::floor(phase);
    x_ = x;
    return nframes;
}

void Resampler::write(float x) {
    buf_[frame_] = x;
    frame_ = (frame_ + 1) % bufFrames_;
}
