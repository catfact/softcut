
#include <boost/assert.hpp>
#include "Interpolate.h"
#include "Resampler.h"

using namespace softcut;

//----------
//-- public

Resampler::Resampler() : rate_(1.0), phi_(1.0), phase_(0.0)
#ifdef RESAMPLER_INTERPOLATE_LINEAR
#else
,inBufIdx_(0)
#endif
{}

void Resampler::setRate(double r) {
    rate_ = r;
    phi_ = 1.0 / r;
}

void Resampler::setPhase(double phase) { phase_ = phase; }

void Resampler::reset() {

#ifdef RESAMPLER_INTERPOLATE_LINEAR
    x_ = 0.f;
#else
    for (float &i : inBuf_) { i = 0.f; }
    for (float &i : outBuf_) { i = 0.f; }
    inBufIdx_ = 0;
#endif
}

int Resampler::processFrame(float x) {
    pushInput(x);
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
    unsigned int nf = static_cast<unsigned int>(p);
    // we can assume that n >= 1 here
    // we want to track fractional output phase for interpolation
    // this is normalized to the distance between input frames
    // so: the distance to the first output frame boundary:
    double f = 1.0 - phase_;
    // normalized (divided by rate);
    f = f * phi_;
    // write the first output frame
    unsigned int i=0;
    outBuf_[i] = interpolate(static_cast<float>(f));
    i++;
    while(i < nf) {
        // distance between output frames in this normalized space is 1/rate
        f += phi_;
        outBuf_[i] = interpolate(static_cast<float>(f));
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
    BOOST_ASSERT_MSG(p >= 0.0, "resampler encountered negative phase");
    auto nf = static_cast<unsigned int>(p);
    if (nf > 0) {
        float f = 1.f - static_cast<float>(phase_);
        f *= phi_;
        outBuf_[0] = interpolate(f);
        phase_ = p - static_cast<double>(nf);
    } else {
        phase_ = p;
    }
    return nf;
}

void Resampler::pushInput(float x) {
#ifdef RESAMPLER_INTERPOLATE_LINEAR
    x_1_ = x_;
    x_ = x;
#else
    inBufIdx_ = (inBufIdx_ + 1) & IN_BUF_MASK;
    inBuf_[inBufIdx_] = x;
#endif
}

float Resampler::interpolate(float f) {
#ifdef RESAMPLER_INTERPOLATE_LINEAR
    return x_1_ + (x_ - x_1_)*f;
#else
    unsigned int i0, i1, i2, i3;
    i0 = (inBufIdx_ + 1) & IN_BUF_MASK;
    i1 = (inBufIdx_ + 2) & IN_BUF_MASK;
    i2 = (inBufIdx_ + 3) & IN_BUF_MASK;
    i3 = inBufIdx_;
    return static_cast<float>(Interpolate::hermite(f, inBuf_[i0], inBuf_[i1], inBuf_[i2], inBuf_[i3]));
#endif
}

const float *Resampler::output() {
    return static_cast<const float*>(outBuf_);
}
