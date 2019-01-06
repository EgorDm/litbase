//
// Created by egordm on 12-12-18.
//

#pragma once

#include <armadillo>
#include "algorithm_structure/algorithm_interface.h"
#include "litsignal_constants.h"

using namespace arma;

namespace litsignal { namespace algorithm {
    template <typename T>
    class FrameFactoryMat : public FrameFactoryInterface<Col<T>> {
    private:
        const Mat<T> &input;

    public:
        explicit FrameFactoryMat(const Mat<T> &input) : input(input) {}

        Col<T> create() override {
            return Col<T>();
        }

        void fill(Col<T> &frame, int i) override {
            frame = input.col(i);
        }

        int getFrameCount() override {
            return ACI(input.n_cols);
        }

        int getInputSize() override {
            return ACI(input.n_cols);
        }
    };
}}
