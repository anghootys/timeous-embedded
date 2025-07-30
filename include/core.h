#ifndef __CORE_H__
#define __CORE_H__

#include <Arduino.h>
#include "logger.h"

#define LOG_CORE_CTX "CORE"

void halt_processor();

void reset_processor();

template<typename T>
T *
alloc(size_t len, int initializer = 0) {
    T *t = (T *) malloc(len);
    memset(t, initializer, len);
    return t;
}

#endif // __CORE_H__
