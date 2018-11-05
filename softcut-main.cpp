#include <iostream>
#include <chrono>
#include <thread>

#include "PaWrapper.h"
#include "OscInterface.h"

static inline void sleep(int ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

int main() {
    PaWrapper::setup();
    PaWrapper::start();

    OscInterface::init(PaWrapper::getSoftCut());

    std::cout << "OSC rx port number: " << OscInterface::getPortNumber() << std::endl;
    std::cout << std::numeric_limits<float>::epsilon() << std::endl;

    // FIXME: stop!
    while(!OscInterface::shouldQuit()) {
        sleep(100);
    }

    PaWrapper::stop();
    PaWrapper::cleanup();
}