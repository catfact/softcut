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
    "SET_AMP_L",
    "SET_AMP_R"
};
#endif

class Commands::CommandPacket {
public:
    CommandPacket() { }
    CommandPacket(Id i, int v, float val) : id(i), voice(v) { value.f = val; }
    CommandPacket(Id i, int v, bool val) : id(i), voice(v) { value.b = val; }

    void setValue(bool v) { value.b = v; }
    void setValue(float v) { value.f = v; }

    // handler should be called on audio thread
    void handle(SoftCut* sc) {
        if (voice < 0 || voice >= sc->getNumVoices()) { return; }
        switch (id) {
            case SET_RATE:
                sc->setRate(voice, value.f);
                break;
            case SET_LOOP_START:
                sc->setLoopStart(voice,value.f);
                break;
            case SET_LOOP_END:
                sc->setLoopEnd(voice,value.f);
                break;
            case SET_LOOP_FLAG:
                sc->setLoopFlag(voice,value.b);
                break;
            case SET_FADE_TIME:
                sc->setFadeTime(voice,value.f);
                break;
            case SET_REC_LEVEL:
                sc->setRecLevel(voice,value.f);
                break;
            case SET_PRE_LEVEL:
                sc->setPreLevel(voice,value.f);
                break;
            case SET_REC_FLAG:
                sc->setRecFlag(voice,value.b);
                break;
            case SET_POSITION:
                sc->cutToPos(voice,value.f);
                break;
            case SET_FILTER_FC:
                sc->setFilterFc(voice,value.f);
                break;
            case SET_FILTER_FC_MOD:
                sc->setFilterFcMod(voice,value.f);
                break;
            case SET_FILTER_RQ:
                sc->setFilterRq(voice,value.f);
                break;
            case SET_FILTER_LP:
                sc->setFilterLp(voice,value.f);
                break;
            case SET_FILTER_HP:
                sc->setFilterHp(voice,value.f);
                break;
            case SET_FILTER_BP:
                sc->setFilterBp(voice,value.f);
                break;
            case SET_FILTER_BR:
                sc->setFilterBr(voice,value.f);
                break;
            case SET_FILTER_DRY:
                sc->setFilterDry(voice,value.f);
                break;
            case SET_AMP_L:
                sc->setAmpLeft(voice, value.f);
                break;
            case SET_AMP_R:
                sc->setAmpRight(voice, value.f);
                break;
            case SET_PRE_FADE_WINDOW:
                sc->setPreFadeWindow(value.f);
                break;
            case SET_REC_FADE_DELAY:
                sc->setRecFadeDelay(value.f);
                break;
            default:
                ;;
        }
    }

private:
    //Command type;
    Id id;
    int voice;
    union {
        bool b;
        float f;
    } value;
};

void Commands::post(Commands::Id id, int voice, float value) {
    CommandPacket p(id, voice, value);
    std::cout << "post command: " << labels[id] << std::endl;
    q.push(p);
}

void Commands::post(Commands::Id id, int voice, bool value) {
    CommandPacket p(id, voice, value);
    std::cout << "post command: " << labels[id] << std::endl;
    q.push(p);
}

void Commands::handlePending(SoftCut* sc) {
    CommandPacket p;
    while (q.pop(p)) {
        p.handle(sc);
    }
}
