//
// Created by egordm on 3-12-18.
//

#pragma once

#include <armadillo>

using namespace arma;

namespace litsignal { namespace mat_utils {
    /**
     * Creates a matrix from multipel vectors
     * @param data
     * @return
     */
    mat mat_from_vecs(const std::vector<vec> &data);

    /*
     * Pads given mat by repeating its data before and after the pad
     */
    mat pad_mat(const mat &data, unsigned int n_before, unsigned int n_after, bool repeat = true);

    vec smooth_filter_subtract(const vec &novelty_curve, float smooth_length, int feature_rate);
}}