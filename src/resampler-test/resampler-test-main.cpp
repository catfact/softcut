#include <cmath>
#include <iostream>
#include <string.h>
#include <vector>


#include <memory>
#include <sndfile.h>

#include "../Resampler.h"

using namespace std;
using namespace softcut;

int main (int argc, const char **argv) {
    SNDFILE *outfile ;
    SF_INFO sfinfo;

    double sr;
    double dur;
    size_t nframes;

    std::unique_ptr<float[]>input;

    const char *infilename = nullptr;
    if(argc > 1) {
        // load input file
        SNDFILE *infile ;
        infilename = argv[1];
        memset (&sfinfo, 0, sizeof(sfinfo));
        infile = sf_open(infilename, SFM_READ, &sfinfo);
        nframes = sfinfo.frames;
        sr = sfinfo.samplerate;
        dur = static_cast<double>(nframes) / sr;
        input = std::make_unique<float[]>(nframes);
        sf_read_float(infile, input.get(), sfinfo.frames);
    } else {
        // generate test sine
        // test sine frequency
        double hz = 440.0;
        // input signal parameters
        sr = 48000.0;
        dur = 4.0;
        nframes = static_cast<size_t>(sr * dur);
        input = std::make_unique<float[]>(nframes);
        double phi = 2 * M_PI / sr * hz;
        double x = 0.0;
        for(int i=0; i<nframes; ++i) {
            input[i] = static_cast<float>(sin(x));
            x += phi;
        }
    }

    bool varispeed = true;
    double rate_value = 1.0;
    if(argc > 2) {
        varispeed = false;
        rate_value = atof(argv[2]);
        std::cout << "using constant rate: " << rate_value << std::endl;
    }

    double *rate = (double*)malloc(nframes * sizeof(double));
    if(varispeed) {
        std::cout << "using varispeed LFO" << std::endl;
        double lfo_phi = M_PI * 3.0 / nframes;
        double rx = 0.0;
        for (int i = 0; i < nframes; ++i) {
            rate[i] = sin(rx) + 1.0;
            rx += lfo_phi;
        }
    } else {
        for (int i = 0; i < nframes; ++i) {
            rate[i] = rate_value;
        }
    }

    Resampler rs;
    std::vector<float> output;

    size_t noutframes = 0;
    for(int i=0; i<nframes; ++i) {
        rs.setRate(rate[i]);
        int nf = rs.processFrame(input[i]);
        const float* src = rs.output();
        for(int j=0; j<nf; ++j) {
            output.push_back(src[j]);
        }
        noutframes += nf;
    }

    char outfilename[128];
    sprintf(outfilename, "%s.resamp.wav", infilename == nullptr ? "output" : infilename);
    memset (&sfinfo, 0, sizeof(sfinfo));

    sfinfo.frames = static_cast<sf_count_t>(noutframes);
    if(argc > 3) {
        sr = atoi(argv[3]);
        std::cout << "forcing output sample rate: " << sr << std::endl;
    }
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
