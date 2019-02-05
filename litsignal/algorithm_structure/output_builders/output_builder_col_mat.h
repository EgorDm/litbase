//
// Created by egordm on 23-12-18.
//

#pragma once

#include <armadillo>
#include <litsignal_constants.h>
#include "algorithm_structure/algorithm_interface.h"

namespace litsignal { namespace algorithm {
    template<typename T>
    class OutputBuilderColMat : public AlgorithmOutputBuilder<arma::Mat<T>, arma::Col<T>> {
    private:
        using ParentType = AlgorithmOutputBuilder<arma::Mat<T>, arma::Col<T>>;

    protected:
        int frame_size;

    public:
        explicit OutputBuilderColMat(int frame_size) : ParentType(arma::Mat<T>()), frame_size(frame_size) {}

        void fill(AlgorithmContext<arma::Col<T>> *context, int i) override {
            ParentType::output(arma::span::all, i) = context->getOutput();
        }

        void resize(int capacity) override {
            ParentType::output.set_size(ACU(frame_size), ACU(capacity));
        }

        int getSize() override {
            return ACI(ParentType::output.n_cols);
        }
    };
}};


