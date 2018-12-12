#include <utility>

//
// Created by egordm on 12-12-18.
//

#pragma once

#include <armadillo>
#include "algorithm_interface.h"


namespace litsignal { namespace algorithm {
class NodeWindowVec : public AlgorithmNodeInterface<void, void, arma::vec> {
    protected:
    arma::vec window;

    public:
        NodeWindowVec(arma::vec window) : window(std::move(window)) {}

        void apply(arma::vec &frame, void *context) override {
            frame %= window;
        }

        int getWindowSize() {
            return ACI(window.size());
        }
    };
}}
