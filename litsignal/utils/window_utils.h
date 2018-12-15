//
// Created by egordm on 13-12-18.
//

#pragma once

#include <armadillo>
#include "../litsignal_constants.h"

using namespace arma;

namespace litsignal { namespace window {
    inline vec hanning(int n) {
        vec w(ACU(n));
        for (uword i = 0; i < n; i++) {
            w[i] = 0.5 - 0.5 * std::cos((constants::PI2 * i) / (n - 1));
        }
        return w;
    }

    inline vec sinw(int n, float beta) {
        return pow(sin(constants::PI * regspace(0, ACU(n - 1)) / n), beta);
    }


}}
