//
// Created by egordm on 6-11-2018.
//

#ifndef LITCORE_PROJECT_DEBUG_H
#define LITCORE_PROJECT_DEBUG_H

#include <string>

namespace litcore { namespace utils { namespace debug {
    void log(const std::string &tag, const std::string &message, ...);

    void log_error(const std::string &tag, const std::string &message, ...);
}}}

#endif //LITCORE_PROJECT_DEBUG_H
