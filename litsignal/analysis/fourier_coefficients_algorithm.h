//
// Created by egordm on 6-2-19.
//

#pragma once

#include <armadillo>

using namespace arma;

namespace litsignal { namespace analysis {
    cx_mat compute_fourier_coefficients(const vec &x, float sr, const vec &f, const vec &window, int hop_length, float &out_fr);
}}
