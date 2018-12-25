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
    private:
        using ParentType = FrameFactoryInterface<Mat<T>, Col<T>>;

    public:
        explicit FrameFactoryMat(const Mat<T> &input) : FrameFactoryInterface<Mat<T>, Col<T>>(input) {}

        Col<T> create() override {
            return Col<T>();
        }

        void fill(Col<T> &frame, int i) override {
            frame = ParentType::input.col(i);
        }

        int getFrameCount() override {
            return ACI(ParentType::input.n_cols);
        }
    };
}}
