//
// Created by ezra on 11/3/18.
//

#ifndef SOFTCUT_PAWRAPPER_H
#define SOFTCUT_PAWRAPPER_H

namespace softcut {
    class SoftCutVoice;

    class PaWrapper {
    public:
        static void setup();

        static void cleanup();

        static void start();

        static void stop();

        static SoftCutVoice* getSoftCut();

    private:
        class Imp;

        static Imp imp;
    };
}


#endif //SOFTCUT_PAWRAPPER_H
