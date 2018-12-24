//
// Created by egordm on 12-12-18.
//

#pragma once

#include <armadillo>
#include <fftw3.h>
#include "algorithm_interface.h"
#include "../litsignal_constants.h"

namespace litsignal { namespace algorithm {
    struct FFTContext : public AlgorithmContext<arma::cx_vec> {
        fftw_plan plan = nullptr;

        explicit FFTContext(int size)
                : ContextType(arma::cx_vec(ACU(size))),
                  plan(fftw_plan_dft_r2c_1d(static_cast<int>(output.size()), nullptr, nullptr, FFTW_ESTIMATE)) {}

        virtual ~FFTContext() {
            if (plan != nullptr) fftw_destroy_plan(plan);
            fftw_cleanup();
        }
    };

    class NodeFFT : public AlgorithmNodeInterface<FFTContext, arma::vec> {
    public:
        bool mirror;

        explicit NodeFFT(bool mirror = false) : mirror(mirror) {}

        void apply(arma::vec &frame, FFTContext *context) override {
            auto *in = frame.memptr();
            auto *out = reinterpret_cast<fftw_complex *>(context->getOutput().memptr());
            fftw_execute_dft_r2c(context->plan, in, out);

            // Mirror the results since fft is symmetric
            if (mirror) {
                arma::cx_vec &output = context->getOutput();
                int N = static_cast<int>(context->getOutput().size());
                int offset = static_cast<int>(ceil(N / 2.));
                int n_elem = N - offset;
                for (int i = 0; i < n_elem; ++i) {
                    output(ACU(offset + i)) = std::conj(output(ACU(n_elem - i)));
                }
            }
        }
    };
}}

