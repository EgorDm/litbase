//
// Created by egordm on 9-12-18.
//

#pragma once

#include <armadillo>

#define TEST_EPLSION 0.0000001

#define COMP_DBL(a, b) (abs(a - b) < TEST_EPLSION)

template <typename T>
inline bool compare_vec(const arma::Col<T> &a, const arma::Col<T> &b) {
    for (int i = 0; i < a.size(); ++i) {
        if(!COMP_DBL(a[i], b[i])) return false;
    }

    return true;
}