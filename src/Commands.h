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
            SET_POSITION,
            SET_FILTER_FC,
            SET_FILTER_FC_MOD,
            SET_FILTER_RQ,
            SET_FILTER_LP,
            SET_FILTER_HP,
            SET_FILTER_BP,
            SET_FILTER_BR,
            SET_FILTER_DRY,
            SET_AMP_L,
            SET_AMP_R,
            SET_PRE_FADE_WINDOW,
            SET_REC_FADE_DELAY,
            SET_PRE_FADE_SHAPE,
            SET_REC_FADE_SHAPE,
            SET_LEVEL_SLEW_TIME,
            SET_RATE_SLEW_TIME,
            NUM_COMMANDS
        } Id;

    public:
        static void handlePending(SoftCut* sc);
        static void post(Id id, int voice, float value);
        static void post(Id id, int voice, bool value);
        static std::string labels[NUM_COMMANDS];
    private:
        class CommandPacket;
        static boost::lockfree::spsc_queue <CommandPacket> q;

    };

}

#endif //SOFTCUT_COMMANDS_H
