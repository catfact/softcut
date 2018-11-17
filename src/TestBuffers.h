//
// Created by ezra on 11/16/18.
//

#ifndef SOFTCUT_TESTBUFFERS_H
#define SOFTCUT_TESTBUFFERS_H

#include <iostream>
#include <fstream>

namespace softcut {
class TestBuffers {
public:
    typedef enum { Phase, Fade, Rate, Pre, Rec, numChannels } Channel;
    enum { numFrames = 65536, frameMask = 65535 };

    float buf[numChannels][numFrames]{};
    unsigned int idx = 0;

    void init() {
        for (int ch=0; ch<numChannels; ++ch) {
            for (int fr=0; fr<numFrames; ++fr) {
                buf[ch][fr] = 0.f;
            }
        }
    }

    void update(float phase, float fade, float rate, float pre, float rec) {
        buf[Phase][idx] = phase
        buf[Fade][idx] = fade;
        buf[Rate][idx] = rate;
        buf[Pre][idx] = pre;
        buf[Rec][idx] = rec;
        idx = (idx+1)&frameMask;
    }

    // print buffer contents in matlab format
    void print() {
        using std::endl;
        std::ofstream ofs ("softcut-test-buffers.m", std::ofstream::out);
        ofs << "[" << endl;
        for (int ch=0; ch<numChannels; ++ch) {
            ofs << "[ ";
            for (int fr=0; fr<numFrames; ++fr) {
                ofs << buf[ch][fr] << " ";
            }
            ofs << " ]," << endl << endl;
        }
        ofs << "]" << endl;

        ofs.close();
    }
};
}


#endif //SOFTCUT_TESTBUFFERS_H
