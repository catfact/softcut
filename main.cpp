#include <iostream>
#include <chrono>
#include <thread>

#include "PaWrapper.h"

static inline void sleep(int ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

int main() {
    PaWrapper::setup();
    PaWrapper::start();

    // FIXME: stop!
    while(true) {
        sleep(100);
    }

    PaWrapper::stop();
    PaWrapper::cleanup();
}