#include <iostream>
#include <chrono>
#include <thread>

#include "PaWrapper.h"
#include "OscInterface.h"
#include "Platform.h"


static inline void sleep(int ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

int main() {
    AudioWrapper::setup();
    AudioWrapper::start();

    OscInterface::init(AudioWrapper::getSoftCut());

    std::cout << "OSC rx port number: " << OscInterface::getPortNumber() << std::endl;
    std::cout << std::numeric_limits<float>::epsilon() << std::endl;

    while(!OscInterface::shouldQuit()) {
        sleep(100);
    }

    AudioWrapper::stop();
    AudioWrapper::cleanup();
}