//
// Created by ezra on 11/3/18.
//

#include <iostream>
#include "Commands.h"

using namespace softcut;

// FIXME: can throw, shouldn't be storge level init
boost::lockfree::spsc_queue <Commands::CommandPacket> Commands::q(100);

#if 1 // unused for now
std::string Commands::labels[NUM_COMMANDS] = {
    "SET_RATE",
    "SET_LOOP_START",
    "SET_LOOP_END",
    "SET_LOOP_FLAG",
    "SET_FADE_TIME",
    "SET_REC_LEVEL",
    "SET_PRE_LEVEL",
    "SET_REC_FLAG",
    "SET_POSITION",
    "SET_FILTER_FC",
    "SET_FILTER_FC_MOD",
    "SET_FILTER_RQ",
    "SET_FILTER_LP",
    "SET_FILTER_HP",
    "SET_FILTER_BP",
    "SET_FILTER_BR",
    "SET_FILTER_DRY",
};
#endif

class Commands::CommandPacket {
public:
    CommandPacket() { id = SET_LOOP_START; value.f = 0.0; }
    CommandPacket(Id i, float val) : id(i) { value.f = val; }
    CommandPacket(Id i, bool val) : id(i) { value.b = val; }

    void setValue(bool v) { value.b = v; }
    void setValue(float v) { value.f = v; }

    // handler should be called on audio thread
    void handle(SoftCutVoice* scv) {
        switch (id) {
            case SET_RATE:
                scv->setRate(value.f);
                break;
            case SET_LOOP_START:
                scv->setLoopStart(value.f);
                break;
            case SET_LOOP_END:
                scv->setLoopEnd(value.f);
                break;
            case SET_LOOP_FLAG:
                scv->setLoopFlag(value.b);
                break;
            case SET_FADE_TIME:
                scv->setFadeTime(value.f);
                break;
            case SET_REC_LEVEL:
                scv->setRecLevel(value.f);
                break;
            case SET_PRE_LEVEL:
                scv->setPreLevel(value.f);
                break;
            case SET_REC_FLAG:
                scv->setRecFlag(value.b);
                break;
            case SET_POSITION:
                scv->cutToPos(value.f);
                break;
            case SET_FILTER_FC:
                scv->setFilterFc(value.f);
                break;
            case SET_FILTER_FC_MOD:
                scv->setFilterFcMod(value.f);
                break;
            case SET_FILTER_RQ:
                scv->setFilterRq(value.f);
                break;
            case SET_FILTER_LP:
                scv->setFilterLp(value.f);
                break;
            case SET_FILTER_HP:
                scv->setFilterHp(value.f);
                break;
            case SET_FILTER_BP:
                scv->setFilterBp(value.f);
                break;
            case SET_FILTER_BR:
                scv->setFilterBr(value.f);
                break;
            case SET_FILTER_DRY:
                scv->setFilterDry(value.f);
                break;
            default:
                ;;
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
    // std::cout << "post command: " << labels[id] << std::endl;
    q.push(p);
}

void Commands::post(Commands::Id id, bool value) {
    CommandPacket p(id, value);
    // std::cout << "post command: " << labels[id] << std::endl;
    q.push(p);
}

void Commands::handlePending(SoftCutVoice* sc) {
    CommandPacket p;
    while (q.pop(p)) {
        p.handle(sc);
    }
}
