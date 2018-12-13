//
// Created by egordm on 2-12-18.
//

#pragma once

#define _USE_MATH_DEFINES
#include <cmath>

#define ACU(x) static_cast<const arma::uword>(x)
#define ACI(x) static_cast<int>(x)

namespace litsignal { namespace constants {
    constexpr double PISQ = M_PI * M_PI;
    constexpr double PI = M_PI;
    constexpr double PI2 = M_PI * 2;
    constexpr double EPSILON = 0.0000001;
}}
