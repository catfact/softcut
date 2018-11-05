//
// Created by ezra on 11/3/18.
//

#ifndef SOFTCUT_COMMANDS_H
#define SOFTCUT_COMMANDS_H

#include <boost/lockfree/spsc_queue.hpp>

#include "SoftCut.h"

namespace softcut {


    class Commands {
    public:
        typedef enum {
            SET_RATE,
            SET_LOOP_START,
            SET_LOOP_END,
            SET_LOOP_FLAG,
            SET_FADE_TIME,
            SET_REC_LEVEL,
            SET_PRE_LEVEL,
            SET_REC_FLAG,
            SET_REC_OFFSET,
            SET_POSITION
        } Id;

    public:
        static void handlePending(SoftCut* sc);
        static void post(Id id, float value);
        static void post(Id id, bool value);
    private:
        class CommandPacket;
        static boost::lockfree::spsc_queue <CommandPacket> q;

    };

}

#endif //SOFTCUT_COMMANDS_H
