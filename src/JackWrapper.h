//
// Created by emb on 11/5/18.
//

#ifndef SOFTCUT_JACKWRAPPER_H
#define SOFTCUT_JACKWRAPPER_H

namespace softcut {
    class SoftCut;

    class JackWrapper {
    public:
        static void setup();

        static void cleanup();

        static void start();

        static void stop();

        static SoftCut* getSoftCut();

    private:
        class Imp;

        static Imp imp;
    };
}



#endif //SOFTCUT_JACKWRAPPER_H
