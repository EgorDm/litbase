//
// Created by egordm on 12-12-18.
//

#pragma once

#include <armadillo>
#include <fftw3.h>
#include "../litsignal_constants.h"
#include "algorithm_interface.h"

namespace litsignal { namespace algorithm {
    struct IFFTContext : AlgorithmContext<arma::vec> {
        fftw_plan plan = nullptr;

        explicit IFFTContext(int size)
                : ContextType(arma::vec(ACU(size))),
                  plan(fftw_plan_dft_c2r_1d(size, nullptr, nullptr, FFTW_ESTIMATE | FFTW_PRESERVE_INPUT)) {}

        virtual ~IFFTContext() {
            if (plan != nullptr) fftw_destroy_plan(plan);
            fftw_cleanup();
        }
    };

    class NodeIFFT : public AlgorithmNodeInterface<IFFTContext, arma::cx_vec> {
    public:
        NodeIFFT() = default;

        void apply(arma::cx_vec &frame, IFFTContext *context) override {
            auto *in = reinterpret_cast<fftw_complex *>(frame.memptr());
            auto *out = context->getOutput().memptr();
            fftw_execute_dft_c2r(context->plan, in, out);
            context->getOutput() /= context->getOutput().size(); // Normalize output
        }
    };
}}
