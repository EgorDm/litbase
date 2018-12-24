#include <utility>

//
// Created by egordm on 12-12-18.
//

#pragma once

#include <armadillo>
#include "algorithm_interface.h"


namespace litsignal { namespace algorithm {
    template<typename T>
    class NodeWindowVec : public AlgorithmNodeInterface<void, arma::Col<T>> {
    protected:
        arma::Col<T> window;

    public:
        explicit NodeWindowVec(arma::Col<T> window) : window(std::move(window)) {}

        void apply(arma::Col<T> &frame, void *context) override {
            frame %= window;
        }

        int getWindowSize() {
            return ACI(window.size());
        }
    };
}}
