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
        std::cerr << "liblo error: " <<  num << "; " << m << "; " << path << std::endl;
    }

    static int setRate(const char *path, const char *types,
                               lo_arg **argv, int argc, lo_message msg, void *data) {
        (void)path; (void)types; (void)argc; (void)msg; (void)data;
        if(argc<2) { return 0; }
        Commands::post(Commands::SET_RATE, argv[0]->i, argv[1]->f);
        return 0;
    }

    static int setLoopStart(const char *path, const char *types,
                       lo_arg **argv, int argc, lo_message msg, void *data) {
        (void)path; (void)types; (void)argc; (void)msg; (void)data;
        if(argc<2) { return 0; }
        Commands::post(Commands::SET_LOOP_START, argv[0]->i, argv[1]->f);
        return 0;
    }

    static int setLoopEnd(const char *path, const char *types,
                            lo_arg **argv, int argc, lo_message msg, void *data) {
        (void)path; (void)types; (void)argc; (void)msg; (void)data;
        if(argc<2) { return 0; }
        Commands::post(Commands::SET_LOOP_END, argv[0]->i, argv[1]->f);
        return 0;
    }

    static int setLoopFlag(const char *path, const char *types,
                          lo_arg **argv, int argc, lo_message msg, void *data) {
        (void)path; (void)types; (void)argc; (void)msg; (void)data;
        if(argc<2) { return 0; }
        Commands::post(Commands::SET_LOOP_FLAG, argv[0]->i, argv[1]->f > 0);
        return 0;
    }

    static int setFadeTime(const char *path, const char *types,
                            lo_arg **argv, int argc, lo_message msg, void *data) {
        (void)path; (void)types; (void)argc; (void)msg; (void)data;
        if(argc<2) { return 0; }
        Commands::post(Commands::SET_FADE_TIME, argv[0]->i, argv[1]->f);
        return 0;
    }

    static int setRecLevel(const char *path, const char *types,
                            lo_arg **argv, int argc, lo_message msg, void *data) {
        (void)path; (void)types; (void)argc; (void)msg; (void)data;
        if(argc<2) { return 0; }
        Commands::post(Commands::SET_REC_LEVEL, argv[0]->i, argv[1]->f);
        return 0;
    }

    static int setPreLevel(const char *path, const char *types,
                            lo_arg **argv, int argc, lo_message msg, void *data) {
        (void)path; (void)types; (void)argc; (void)msg; (void)data;
        if(argc<2) { return 0; }
        Commands::post(Commands::SET_PRE_LEVEL, argv[0]->i, argv[1]->f);
        return 0;
    }

    static int setRecFlag(const char *path, const char *types,
                            lo_arg **argv, int argc, lo_message msg, void *data) {
        (void)path; (void)types; (void)argc; (void)msg; (void)data;
        if(argc<2) { return 0; }
        Commands::post(Commands::SET_REC_FLAG, argv[0]->i, argv[1]->f > 0);
        return 0;
    }

    static int setPosition(const char *path, const char *types,
                            lo_arg **argv, int argc, lo_message msg, void *data) {
        (void)path; (void)types; (void)argc; (void)msg; (void)data;
        if(argc<2) { return 0; }
        Commands::post(Commands::SET_POSITION, argv[0]->i, argv[1]->f);
        return 0;
    }

    static int setQuit(const char *path, const char *types,
                    lo_arg **argv, int argc, lo_message msg, void *data) {
        (void)argv; (void)path; (void)types; (void)argc; (void)msg; (void)data;
        quitFlag = true;
        return 0;
    }
    
    static int setFilterFc(const char *path, const char *types, lo_arg **argv, int argc, lo_message msg, void *data) {
        (void)path; (void)types; (void)argc; (void)msg; (void)data;
        if(argc<2) { return 0; }
	Commands::post(Commands::SET_FILTER_FC, argv[1]->i, argv[1]->f);
	return 0;
    }
    
    static int setFilterFcMod(const char *path, const char *types, lo_arg **argv, int argc, lo_message msg, void *data) {
        (void)path; (void)types; (void)argc; (void)msg; (void)data;
        if(argc<2) { return 0; }
        Commands::post(Commands::SET_FILTER_FC_MOD, argv[0]->i, argv[1]->f);
        return 0;
    }
    
    static int setFilterRq(const char *path, const char *types, lo_arg **argv, int argc, lo_message msg, void *data) {
        (void)path; (void)types; (void)argc; (void)msg; (void)data;
        if(argc<2) { return 0; }
	Commands::post(Commands::SET_FILTER_RQ, argv[0]->i, argv[1]->f);
      return 0;
    }
    
    static int setFilterLp(const char *path, const char *types, lo_arg **argv, int argc, lo_message msg, void *data) {
        (void)path; (void)types; (void)argc; (void)msg; (void)data;
        if(argc<2) { return 0; }
	Commands::post(Commands::SET_FILTER_FC, argv[0]->i, argv[1]->f);
      return 0;
    }
    
    static int setFilterHp(const char *path, const char *types, lo_arg **argv, int argc, lo_message msg, void *data) {
        (void)path; (void)types; (void)argc; (void)msg; (void)data;
        if(argc<2) { return 0; }
	Commands::post(Commands::SET_FILTER_HP, argv[0]->i, argv[1]->f);
      return 0;
    }
    
    static int setFilterBp(const char *path, const char *types, lo_arg **argv, int argc, lo_message msg, void *data) {
        (void)path; (void)types; (void)argc; (void)msg; (void)data;
        if(argc<2) { return 0; }
	Commands::post(Commands::SET_FILTER_BP, argv[0]->i, argv[1]->f);
      return 0;
    }
    
    static int setFilterBr(const char *path, const char *types, lo_arg **argv, int argc, lo_message msg, void *data) {
        (void)path; (void)types; (void)argc; (void)msg; (void)data;
        if(argc<2) { return 0; }
	Commands::post(Commands::SET_FILTER_BR, argv[0]->i, argv[1]->f);
      return 0;
    }
    
    static int setFilterDry(const char *path, const char *types, lo_arg **argv, int argc, lo_message msg, void *data) {
        (void)path; (void)types; (void)argc; (void)msg; (void)data;
        if(argc<2) { return 0; }
	Commands::post(Commands::SET_FILTER_DRY, argv[0]->i, argv[1]->f);
      return 0;
    }


    static int setAmpLeft(const char *path, const char *types, lo_arg **argv, int argc, lo_message msg, void *data) {
        (void)path; (void)types; (void)argc; (void)msg; (void)data;
        if(argc<2) { return 0; }
        Commands::post(Commands::SET_AMP_L, argv[0]->i, argv[1]->f);
        return 0;
    }

    static int setAmpRight(const char *path, const char *types, lo_arg **argv, int argc, lo_message msg, void *data) {
        (void)path; (void)types; (void)argc; (void)msg; (void)data;
        if(argc<2) { return 0; }
        Commands::post(Commands::SET_AMP_R, argv[0]->i, argv[1]->f);
        return 0;
    }

    static int setPreFadeWindow(const char *path, const char *types, lo_arg **argv, int argc, lo_message msg, void *data) {
        (void)path; (void)types; (void)argc; (void)msg; (void)data;
        if(argc<1) { return 0; }
        Commands::post(Commands::SET_PRE_FADE_WINDOW, 0, argv[0]->f);
        return 0;
    }

    static int setRecFadeDelay(const char *path, const char *types, lo_arg **argv, int argc, lo_message msg, void *data) {
        (void)path; (void)types; (void)argc; (void)msg; (void)data;
        if(argc<1) { return 0; }
        Commands::post(Commands::SET_REC_FADE_DELAY, 0, argv[0]->f);
        return 0;
    }

    static int setPreFadeShape(const char *path, const char *types, lo_arg **argv, int argc, lo_message msg, void *data) {
        (void)path; (void)types; (void)argc; (void)msg; (void)data;
        if(argc<1) { return 0; }
        Commands::post(Commands::SET_PRE_FADE_SHAPE, 0, argv[0]->f);
        return 0;
    }
    static int setRecFadeShape(const char *path, const char *types, lo_arg **argv, int argc, lo_message msg, void *data) {
        (void)path; (void)types; (void)argc; (void)msg; (void)data;
        if(argc<1) { return 0; }
        Commands::post(Commands::SET_REC_FADE_SHAPE, 0, argv[0]->f);
        return 0;
    }


public:
    // FIXME: doesn't actually need pointer to audio class
    static void init(SoftCut* sc)
    {
        st = lo_server_thread_new(port.c_str(), handleLoError);
        lo_server_thread_add_method(st, "/set/rate", "if", OscInterface::setRate, sc);
        lo_server_thread_add_method(st, "/set/loopStart", "if", OscInterface::setLoopStart, sc);
        lo_server_thread_add_method(st, "/set/loopEnd", "if", OscInterface::setLoopEnd, sc);
        lo_server_thread_add_method(st, "/set/loopFlag", "if", OscInterface::setLoopFlag, sc);
        lo_server_thread_add_method(st, "/set/fadeTime", "if", OscInterface::setFadeTime, sc);
        lo_server_thread_add_method(st, "/set/recLevel", "if", OscInterface::setRecLevel, sc);
        lo_server_thread_add_method(st, "/set/preLevel", "if", OscInterface::setPreLevel, sc);
        lo_server_thread_add_method(st, "/set/recFlag", "if", OscInterface::setRecFlag, sc);
        lo_server_thread_add_method(st, "/set/position", "if", OscInterface::setPosition, sc);
        lo_server_thread_add_method(st, "/set/filterFc", "if", OscInterface::setFilterFc, sc);
        lo_server_thread_add_method(st, "/set/filterFcMod", "if", OscInterface::setFilterFcMod, sc);
        lo_server_thread_add_method(st, "/set/filterRq", "if", OscInterface::setFilterRq, sc);
        lo_server_thread_add_method(st, "/set/filterLp", "if", OscInterface::setFilterLp, sc);
        lo_server_thread_add_method(st, "/set/filterHp", "if", OscInterface::setFilterHp, sc);
        lo_server_thread_add_method(st, "/set/filterBp", "if", OscInterface::setFilterBp, sc);
        lo_server_thread_add_method(st, "/set/filterBr", "if", OscInterface::setFilterBr, sc);
        lo_server_thread_add_method(st, "/set/filterDry", "if", OscInterface::setFilterDry, sc);
        lo_server_thread_add_method(st, "/set/ampLeft", "if", OscInterface::setAmpLeft, sc);
        lo_server_thread_add_method(st, "/set/ampRight", "if", OscInterface::setAmpRight, sc);
        // fade window parameters apply to all voices (and are rather expensive to recompute)
        lo_server_thread_add_method(st, "/set/preFadeWindow", "f", OscInterface::setPreFadeWindow, sc);
        lo_server_thread_add_method(st, "/set/recFadeDelay", "f", OscInterface::setRecFadeDelay, sc);
        lo_server_thread_add_method(st, "/set/preFadeShape", "f", OscInterface::setPreFadeShape, sc);
        lo_server_thread_add_method(st, "/set/recFadeShape", "f", OscInterface::setRecFadeShape, sc);

        lo_server_thread_add_method(st, "/quit", "", OscInterface::setQuit, sc);
        lo_server_thread_start(st);
        quitFlag = false;
    }

    static bool shouldQuit() { return quitFlag; }

    static std::string getPortNumber() { return port; }
};

#endif //SOFTCUT_OSCINTERFACE_H
