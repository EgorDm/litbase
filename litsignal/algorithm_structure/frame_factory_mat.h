//
// Created by egordm on 12-12-18.
//

#pragma once

#include <armadillo>
#include "algorithm_interface.h"
#include "../litsignal_constants.h"

using namespace arma;

namespace litsignal { namespace algorithm {
    template <typename T>
    class FrameFactoryMat : public FrameFactoryInterface<Mat<T>, Col<T>> {
    public:
        FrameFactoryMat() = default;

        Col<T> create() override {
            return Col<T>();
        }

        void fill(const Mat<T> &input, Col<T> &frame, int i) override {
            frame = input.col(i);
        }

        int getFrameCount(const Mat<T> &input) override {
            return ACI(input.n_cols);
        }
    };
}}
