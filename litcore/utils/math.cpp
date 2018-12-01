//
// Created by egordm on 21-11-2018.
//

#include <algorithm>
#include "math.h"

using namespace litcore::utils;

float math::clip(float n, float lower, float upper) {
    return std::max(lower, std::min(n, upper));
}
