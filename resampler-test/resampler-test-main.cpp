#include <cmath>
#include <iostream>
#include <string.h>
#include <vector>


#include <sndfile.h>

#include "../Resampler.h"

using namespace std;
using namespace softcut;

int main (int argc, const char **argv) {
    SNDFILE *outfile ;
    SF_INFO sfinfo;

    // test sine frequency
    double hz = 440.0;

    // input signal parameters
    double sr = 48000.0;
    double dur = 4.0;
    size_t nframes = static_cast<size_t>(sr * dur);

    double phi = 2 * M_PI / sr * hz;
    double x = 0.0;

    float input[nframes];

    for(int i=0; i<nframes; ++i) {
        input[i] = static_cast<float>(sin(x));
        x += phi;
    }

    // fill rate buffer with LFO
    double rate[nframes];
    double lfo_phi = M_PI * 3.0 / nframes;
    double rx = 0.0;
    for(int i=0; i<nframes; ++i) {
        rate[i] = sin(rx) + 1.0;
        rx += lfo_phi;
    }

    Resampler rs;
    std::vector<float> output;

    size_t noutframes = 0;
    for(int i=0; i<nframes; ++i) {
        rs.setRate(rate[i]);
        int nf = rs.processFrame(input[i]);
        const float* src = rs.output();
        for(int j=0; j<nf; ++j) {
            output.push_back(*src++);
        }
        noutframes += nf;
    }

    const char *outfilename = "resamp_varispeed.wav";
    memset (&sfinfo, 0, sizeof(sfinfo));

    sfinfo.frames = static_cast<sf_count_t>(noutframes);
    sfinfo.samplerate = static_cast<int>(sr);
    sfinfo.channels = 1;
    sfinfo.format = SF_FORMAT_WAV | SF_FORMAT_FLOAT;

    if (! (outfile = sf_open (outfilename, SFM_WRITE, &sfinfo))) {
        cerr << "failed to open output file: " << outfilename << endl;
        return 1 ;
    }

    sf_write_float(outfile, output.data(), static_cast<sf_count_t>(noutframes));
    sf_close(outfile);
    return 0;

}
