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
}}