//
// Created by ezra on 11/3/18.
//

#include "Commands.h"

using namespace softcut;


boost::lockfree::spsc_queue <Commands::CommandPacket> Commands::q(100);

class Commands::CommandPacket {
public:
    CommandPacket() { id = SET_LOOP_START; value.f = 0.0; }
    CommandPacket(Id i, float val) : id(i) { value.f = val; }
    CommandPacket(Id i, bool val) : id(i) { value.b = val; }

    void setValue(bool v) { value.b = v; }
    void setValue(float v) { value.f = v; }

    // handler should be called on audio thread
    void handle(SoftCut* sc) {
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
            case SET_LOOP_FLAG:
                sc->setLoopFlag(value.f);
                break;
            case SET_FADE_TIME:
                sc->setFadeTime(value.f);
                break;
            case SET_REC_LEVEL:
                sc->setRecLevel(value.f);
                break;
            case SET_PRE_LEVEL:
                sc->setPreLevel(value.f);
                break;
            case SET_REC_FLAG:
                sc->setRecFlag(value.b);
                break;
            case SET_REC_OFFSET:
                sc->setRecOffset(static_cast<int>(value.f));
                break;
            case SET_POSITION:
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

void Commands::post(Commands::Id id, float value) {
    CommandPacket p(id, value);
    q.push(p);
}

void Commands::post(Commands::Id id, bool value) {
    CommandPacket p(id, value);
    q.push(p);
}

void Commands::handlePending(SoftCut* sc) {
    CommandPacket p;
    while (q.pop(p)) {
        p.handle(sc);
    }
}
