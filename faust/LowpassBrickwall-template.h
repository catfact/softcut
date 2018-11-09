#include <algorithm>

#include "faust/gui/UI.h"
#include "faust/gui/meta.h"
#include "faust/dsp/dsp.h"

using std::max;
using std::min;

<<includeIntrinsic>>
<<includeclass>>

class LowpassBrickwall { 
    mydsp dsp_;
public:        
    void init(int sr) {
        mydsp::classInit(sr);
        dsp_.instanceConstants(sr);
        dsp_.instanceClear();
    }

    void processSample(float* x) {
        dsp_.compute(1, &x, &x);
    }
};