//
// Created by egordm on 9-12-18.
//

#pragma once

#include <armadillo>

using namespace arma;

namespace litsignal { namespace pulse_utils {
/**
     * Generates sine and consine pulse for given window and feature rate at given bpm.
     * @param f
     * @param size
     * @param feature_rate
     * @param shift by: number of samples by which the zero of the pulse should be fixed
     * @return
     */
    void generate_pulse(vec &cosine, vec &sine, double f, int size, int feature_rate, int shift_by = 0);
}}


