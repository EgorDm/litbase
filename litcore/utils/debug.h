//
// Created by egordm on 6-11-2018.
//

#pragma once

#include <string>

#define LIT_ASSERT(x, t, y, r) {if(!(x)) {\
litcore::debug::log_error(t, y); \
return r; \
}}

namespace litcore { namespace debug {
    void log(const std::string &tag, const std::string &message, ...);

    void log_error(const std::string &tag, const std::string &message, ...);
}}
