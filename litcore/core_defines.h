//
// Created by egordm on 1-12-18.
//

#pragma once

#ifdef __ANDROID__
#define ANDROID 1
#elif __unix__
#define LINUX 1
#elif defined(_WIN32) || defined(WIN32) || defined(_WIN64)
#define WINDOWS 1
#endif