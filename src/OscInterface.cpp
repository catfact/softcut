//
// Created by ezra on 11/4/18.
//

#include "OscInterface.h"

lo_server_thread OscInterface::st;
bool OscInterface::quitFlag;
std::string OscInterface::port = "9999";