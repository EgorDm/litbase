//
// Created by egordm on 23-12-18.
//

#pragma once

#include <armadillo>
#include <litsignal_constants.h>
#include "algorithm_structure/algorithm_interface.h"

namespace litsignal { namespace algorithm {
    template<typename T>
    class OutputBuilderVec : public AlgorithmOutputBuilder<arma::Col<T>, arma::Col<T>> {
    private:
        using ParentType = AlgorithmOutputBuilder<arma::Col<T>, arma::Col<T>>;

    protected:
        int hop_size = 0;

    public:
        explicit OutputBuilderVec(int hop_size) : ParentType(arma::Col<T>()), hop_size(hop_size) {}

        void fill(AlgorithmContext<arma::Col<T>> *context, int i) override {
            ParentType::getOutput()(arma::span(ParentType::cursor, ParentType::cursor + context->getOutput().size() - 1)) += context->getOutput();
            ParentType::cursor += hop_size;
        }

        void resize(int capacity) override {
            ParentType::getOutput().zeros(ACU(capacity));

        }

        int getSize() override {
            return ACI(ParentType::getOutput().size());
        }
    };
}};


