//
// Created by ezra on 11/3/18.
//

#ifndef SOFTCUT_COMMANDS_H
#define SOFTCUT_COMMANDS_H

#include <queue>
#include "SoftCut.h"

namespace softcut {


    class Command {

        typedef enum {
            SET_RATE,
            SET_LOOP_START,
            SET_LOOP_END,
            SET_FADE_TIME,
            SET_REC,
            SET_PRE,
            SET_REC_RUN,
            SET_REC_OFFSET,
            CUT_TO_POS
        } Id;

    private:
        class CommandPacket {
        public:
            CommandPacket(Id i) : id(i) { value.f = 0.0; }

            void setValue(bool v) { value.b = v; }

            void setValue(float v) { value.f = v; }

            // handler should be called on audio thread
            void handle(SoftCut *sc) {
                switch (id) {
                    case SET_RATE:
                        sc->setRate(value.f);
                        break;
                    case SET_LOOP_START:
                        sc->setLoopStart(value.f);
                        break;
                    case SET_LOOP_END:
                        sc->setLoopEnd(value.f);
                        break;
                    case SET_FADE_TIME:
                        sc->setFadeTime(value.f);
                        break;
                    case SET_REC:
                        sc->setRecLevel(value.f);
                        break;
                    case SET_PRE:
                        sc->setPreLevel(value.f);
                        break;
                    case SET_REC_RUN:
                        sc->setRecFlag(value.b);
                        break;
                    case SET_REC_OFFSET:
                        sc->setRecOffset(value.f);
                        break;
                    case CUT_TO_POS:
                        sc->cutToPos(value.f);
                        break;
                    default:;;
                }
            }

        private:
            //Command type;
            Id id;
            union {
                bool b;
                float f;
            } value;
        };

        static std::queue <CommandPacket> q;
    };

}

#endif //SOFTCUT_COMMANDS_H
