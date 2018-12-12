//
// Created by egordm on 12-12-18.
//

#pragma once

#include <armadillo>
#include "../algorithm_structure/algorithm_interface.h"
#include "../algorithm_structure/node_fft.h"
#include "../algorithm_structure/node_window_vec.h"
#include "../algorithm_structure/algorithm_simple_runner.h"

using namespace arma;
using namespace litsignal::algorithm;

namespace litsignal { namespace analysis {
    cx_mat calculate_stft(const vec &input, int &feature_rate, vec &t, vec &f, int sr, const vec &window, int hop_size,
            const span &coefficient_range = span(0, 0), bool mirror = false);

    class STFTPipeline : public AlgorthmPipelineInterface<vec, vec, cx_mat, FFTContext> {
    private:
        int sr;
        span coefficient_range;

        int &feature_rate;
        vec &t;
        vec &f;

        NodeWindowVec node_window;
        NodeFFT node_fft;

    public:
        using SimpleRunner = AlgorithmSimpleRunner<vec, FFTContext, OwnType>;

        STFTPipeline(RunnerType *runner, const vec &input, cx_mat &output, int &feature_rate, vec &t, vec &f, int sr,
                     const vec &window, int hop_size, const span &coefficient_range = span(0, 0), bool mirror = false);

        void preProcess() override;

        void processFrame(FFTContext &context, vec &frame, int i) override;

        void postProcess() override;

        FFTContext createContext(vec &frame) override;
    };
}}


