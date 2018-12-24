//
// Created by egordm on 23-12-18.
//

#pragma once

#include <armadillo>
#include <algorithm_structure/algorithm_interface.h>
#include <algorithm_structure/node_ifft.h>
#include <algorithm_structure/node_window_vec.h>

using namespace litsignal::algorithm;

namespace litsignal { namespace analysis {
    class STFTAlgorithm : public AlgorithmInterface<arma::cx_vec, IFFTContext> {
    protected:
        int hop_size;
        arma::vec window_inv;

        NodeWindowVec<double> node_window;
        NodeIFFT node_ifft;

    public:
        STFTAlgorithm(const arma::vec &window, int hop_size = -1)
            : node_window(window), node_ifft(), hop_size(hop_size), window_inv(window) {
            if(hop_size < 0) this->hop_size = ACI(window.size() / 2);
            window_inv /= sqrt((window.size() / (double)hop_size) / 2.);
        }

        void processFrame(IFFTContext &context, arma::cx_vec &frame, int i) override {
            node_ifft.apply(frame, &context);
            node_window.apply(context.getOutput(), &context);
            context.getOutput() %= window_inv;
        }

        IFFTContext createContext(arma::cx_vec &frame) override {
            return IFFTContext(ACI(node_window.getWindowSize()));
        }
    };
}}