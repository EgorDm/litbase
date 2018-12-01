//
// Created by egordm on 1-12-18.
//

#ifndef LITBASE_CORE_DEFINES_H
#define LITBASE_CORE_DEFINES_H

#ifdef __ANDROID__
#define ANDROID 1
#elif __unix__
#define LINUX 1
#elif defined(_WIN32) || defined(WIN32) || defined(_WIN64)
#define WINDOWS 1
#endif

#endif //LITBASE_CORE_DEFINES_H
