//
// Created by egordm on 23-12-18.
//

#pragma once

#include <armadillo>
#include <litsignal_constants.h>
#include "algorithm_structure/algorithm_interface.h"

namespace litsignal { namespace algorithm {
    template<typename T>
    class OutputBuilderRowMat : public AlgorithmOutputBuilder<arma::Mat<T>, arma::Row<T>> {
    private:
        using ParentType = AlgorithmOutputBuilder<arma::Mat<T>, arma::Row<T>>;

    protected:
        int frame_size;

    public:
        explicit OutputBuilderRowMat(int frame_size) : ParentType(arma::Mat<T>()), frame_size(frame_size) {}

        void fill(AlgorithmContext<arma::Row<T>> *context, int i) override {
            ParentType::output(ParentType::cursor++, arma::span::all) = context->getOutput();
        }

        void resize(int capacity) override {
            ParentType::output.set_size(ACU(capacity), ACU(frame_size));
        }

        int getSize() override {
            return ACI(ParentType::output.n_rows);
        }
    };
}};


