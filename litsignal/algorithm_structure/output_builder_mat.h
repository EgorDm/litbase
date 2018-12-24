//
// Created by egordm on 23-12-18.
//

#pragma once

#include <armadillo>
#include <litsignal_constants.h>
#include "algorithm_interface.h"

namespace litsignal { namespace algorithm {
    template<typename T>
    class OutputBuilderMat : public AlgorithmOutputBuilder<arma::Mat<T>, arma::Col<T>> {
    private:
        using ParentType = AlgorithmOutputBuilder<arma::Mat<T>, arma::Col<T>>;

    protected:
        int frame_size;
        int cursor = 0;

    public:
        explicit OutputBuilderMat(int frame_size)
                : ParentType(arma::Mat<T>()), frame_size(frame_size) {}


        void fill(AlgorithmContext<arma::Col<T>> *context, int i) override {
            ParentType::output(arma::span::all, cursor++) = context->getOutput();
        }

        void resize(int capacity) override {
            ParentType::output.set_size(ACU(frame_size), ACU(capacity));
        }

        bool is_full() override {
            return cursor >= ParentType::output.n_cols;
        }
    };
}};


