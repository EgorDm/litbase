//
// Created by egordm on 23-12-18.
//

#pragma once

#include <armadillo>
#include <litsignal_constants.h>
#include "algorithm_interface.h"

namespace litsignal { namespace algorithm {
    template<typename T>
    class OutputBuilderVec : public AlgorithmOutputBuilder<arma::Col<T>, arma::Col<T>> {
    private:
        using ParentType = AlgorithmOutputBuilder<arma::Col<T>, arma::Col<T>>;

    protected:
        int cursor = 0;
        int hop_size = 0;

    public:
        OutputBuilderVec(int hop_size) : ParentType(arma::Col<T>()), hop_size(hop_size) {}

        void fill(AlgorithmContext<arma::Col<T>> *context, int i) override {
            ParentType::getOutput()(arma::span(cursor, cursor + context->getOutput().size() - 1)) = context->getOutput();
            cursor += hop_size;
        }

        void resize(int capacity) override {
            ParentType::getOutput().zeros(ACU(capacity));

        }

        bool is_full() override {
            return cursor >= ParentType::getOutput().size();
        }
    };
}};


