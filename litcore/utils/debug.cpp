//
// Created by egordm on 6-11-2018.
//

#include <iostream>
#include "debug.h"
#include "../core_defines.h"

#ifdef ANDROID

#include <android/log.h>

#endif

void litcore::utils::debug::log(const std::string &tag, const std::string &message, ...) {
#ifdef DEBUG
#if WINDOWS || LINUX
    std::cout << '[' << tag << "] " << message << std::endl;
#elif ANDROID
    __android_log_write(ANDROID_LOG_DEBUG, tag.c_str(), message.c_str());
#endif
#endif
}

void litcore::utils::debug::log_error(const std::string &tag, const std::string &message, ...) {
#ifdef DEBUG
#if WINDOWS || LINUX
    std::cerr << '[' << tag << "] " << message << std::endl;
#elif ANDROID
    __android_log_write(ANDROID_LOG_ERROR, tag.c_str(), message.c_str());
#endif
#endif
}
