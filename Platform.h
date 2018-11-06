//
// Created by emb on 11/5/18.
//

#ifndef SOFTCUT_PLATFORM_H
#define SOFTCUT_PLATFORM_H

#include "JackWrapper.h"

namespace  softcut {

#ifdef __linux__

    typedef JackWrapper AudioWrapper;

#endif

#ifdef __APPLE__

    typedef PaWrapper AudioWrapper

#endif

}

#endif //SOFTCUT_PLATFORM_H
