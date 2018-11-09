//
// Created by ezra on 11/4/18.
//

#ifndef SOFTCUT_OSCINTERFACE_H
#define SOFTCUT_OSCINTERFACE_H

#include <iostream>
#include <string>

#include <lo/lo.h>

#include "SoftCut.h"
#include "Commands.h"

using namespace softcut;

using std::cout;
using std::endl;

class OscInterface {
private:
    static lo_server_thread st;
    static bool quitFlag;
    static std::string port;

private:
    static void handleLoError(int num, const char *m, const char *path) {
        std::cerr << "liblo error \t" <<  std::endl;
    }

    static int setRate(const char *path, const char *types,
                               lo_arg **argv, int argc, lo_message msg, void *user_data) {
        //auto *sc  = (SoftCut*)user_data;
        cout << "set rate \t" <<  argv[0]->f << endl;
             Commands::post(Commands::SET_RATE, argv[0]->f);
        return 0;
    }

    static int setLoopStart(const char *path, const char *types,
                       lo_arg **argv, int argc, lo_message msg, void *user_data) {
        //auto *sc  = (SoftCut*)user_data;
        cout << "set loop start \t" <<  argv[0]->f << endl;
        Commands::post(Commands::SET_LOOP_START, argv[0]->f);
        return 0;
    }

    static int setLoopEnd(const char *path, const char *types,
                            lo_arg **argv, int argc, lo_message msg, void *user_data) {
        //auto *sc  = (SoftCut*)user_data;
        cout << "set loop end \t" <<  argv[0]->f << endl;
        Commands::post(Commands::SET_LOOP_END, argv[0]->f);
        return 0;
    }

    static int setLoopFlag(const char *path, const char *types,
                          lo_arg **argv, int argc, lo_message msg, void *user_data) {
        //auto *sc  = (SoftCut*)user_data;
        cout << "set loop flag\t" <<  argv[0]->f << endl;
        Commands::post(Commands::SET_LOOP_FLAG, argv[0]->f > 0);
        return 0;
    }

    static int setFadeTime(const char *path, const char *types,
                            lo_arg **argv, int argc, lo_message msg, void *user_data) {
        //auto *sc  = (SoftCut*)user_data;
        cout << "set fade time \t" <<  argv[0]->f << endl;
        Commands::post(Commands::SET_FADE_TIME, argv[0]->f);
        return 0;
    }

    static int setRecLevel(const char *path, const char *types,
                            lo_arg **argv, int argc, lo_message msg, void *user_data) {
        //auto *sc  = (SoftCut*)user_data;
        cout << "set rec level \t" <<  argv[0]->f << endl;
        Commands::post(Commands::SET_REC_LEVEL, argv[0]->f);
        return 0;
    }

    static int setPreLevel(const char *path, const char *types,
                            lo_arg **argv, int argc, lo_message msg, void *user_data) {
        //auto *sc  = (SoftCut*)user_data;
        cout << "set pre level \t" <<  argv[0]->f << endl;
        Commands::post(Commands::SET_PRE_LEVEL, argv[0]->f);
        return 0;
    }

    static int setRecFlag(const char *path, const char *types,
                            lo_arg **argv, int argc, lo_message msg, void *user_data) {
        //auto *sc  = (SoftCut*)user_data;
        cout << "set rec flag \t" <<  argv[0]->f << endl;
        Commands::post(Commands::SET_REC_FLAG, argv[0]->f > 0);
        return 0;
    }

//    static int setRecOffset(const char *path, const char *types,
//                            lo_arg **argv, int argc, lo_message msg, void *user_data) {
//        //auto *sc  = (SoftCut*)user_data;
//        cout << "set rec offset \t" <<  argv[0]->f << endl;
//        Commands::post(Commands::SET_REC_OFFSET, argv[0]->f);
//        return 0;
//    }

    static int setPosition(const char *path, const char *types,
                            lo_arg **argv, int argc, lo_message msg, void *user_data) {
        //auto *sc  = (SoftCut*)user_data;
        cout << "set position \t" <<  argv[0]->f << endl;
        Commands::post(Commands::SET_POSITION, argv[0]->f);
        return 0;
    }

    static int setQuit(const char *path, const char *types,
                    lo_arg **argv, int argc, lo_message msg, void *user_data) {
        quitFlag = true;
        return 0;
    }
    
    static int setFilterFc(const char *path, const char *types, lo_arg **argv, int argc, lo_message msg, void *data) {
      Commands::post(Commands::SET_FILTER_FC, argv[0]->f);
      return 0;
    }
    static int setFilterRq(const char *path, const char *types, lo_arg **argv, int argc, lo_message msg, void *data) {
      Commands::post(Commands::SET_FILTER_RQ, argv[0]->f);
      return 0;
    }
    static int setFilterLp(const char *path, const char *types, lo_arg **argv, int argc, lo_message msg, void *data) {
      Commands::post(Commands::SET_FILTER_FC, argv[0]->f);
      return 0;
    }
    static int setFilterHp(const char *path, const char *types, lo_arg **argv, int argc, lo_message msg, void *data) {
      Commands::post(Commands::SET_FILTER_HP, argv[0]->f);
      return 0;
    }
    static int setFilterBp(const char *path, const char *types, lo_arg **argv, int argc, lo_message msg, void *data) {
      Commands::post(Commands::SET_FILTER_BP, argv[0]->f);
      return 0;
    }
    static int setFilterBr(const char *path, const char *types, lo_arg **argv, int argc, lo_message msg, void *data) {
      Commands::post(Commands::SET_FILTER_BR, argv[0]->f);
      return 0;
    }
    static int setFilterDry(const char *path, const char *types, lo_arg **argv, int argc, lo_message msg, void *data) {
      Commands::post(Commands::SET_FILTER_DRY, argv[0]->f);
      return 0;
    }

public:

    static void init(SoftCut* sc)
    {
        st = lo_server_thread_new(port.c_str(), handleLoError);
        lo_server_thread_add_method(st, "/set/rate", "f", OscInterface::setRate, sc);
        lo_server_thread_add_method(st, "/set/loopStart", "f", OscInterface::setLoopStart, sc);
        lo_server_thread_add_method(st, "/set/loopEnd", "f", OscInterface::setLoopEnd, sc);
        lo_server_thread_add_method(st, "/set/loopFlag", "f", OscInterface::setLoopFlag, sc);
        lo_server_thread_add_method(st, "/set/fadeTime", "f", OscInterface::setFadeTime, sc);
        lo_server_thread_add_method(st, "/set/recLevel", "f", OscInterface::setRecLevel, sc);
        lo_server_thread_add_method(st, "/set/preLevel", "f", OscInterface::setPreLevel, sc);
        lo_server_thread_add_method(st, "/set/recFlag", "f", OscInterface::setRecFlag, sc);
//        lo_server_thread_add_method(st, "/set/recOffset", "f", OscInterface::setRecOffset, sc);
        lo_server_thread_add_method(st, "/set/position", "f", OscInterface::setPosition, sc);
        lo_server_thread_add_method(st, "/set/filterFc", "f", OscInterface::setFilterFc, sc);
        lo_server_thread_add_method(st, "/set/filterRq", "f", OscInterface::setFilterRq, sc);
        lo_server_thread_add_method(st, "/set/filterLp", "f", OscInterface::setFilterLp, sc);
        lo_server_thread_add_method(st, "/set/filterHp", "f", OscInterface::setFilterHp, sc);
        lo_server_thread_add_method(st, "/set/filterBp", "f", OscInterface::setFilterBp, sc);
        lo_server_thread_add_method(st, "/set/filterBr", "f", OscInterface::setFilterBr, sc);
        lo_server_thread_add_method(st, "/set/filterDry", "f", OscInterface::setFilterDry, sc);
	
        lo_server_thread_add_method(st, "/quit", "", OscInterface::setQuit, sc);
        lo_server_thread_start(st);
        quitFlag = false;
    }

    static bool shouldQuit() { return quitFlag; }

    static std::string getPortNumber() { return port; }
};

#endif //SOFTCUT_OSCINTERFACE_H
