//
// Created by egordm on 23-12-18.
//

#pragma once

#include <armadillo_bits/typedef_mat.hpp>
#include <algorithm_structure/node_fft.h>
#include <algorithm_structure/node_window_vec.h>
#include <algorithm_structure/frame_factory_vec.h>
#include <algorithm_structure/output_builder_mat.h>
#include <algorithm_structure/algorithm_simple_runner.h>

using namespace arma;
using namespace litsignal::algorithm;

namespace litsignal { namespace analysis {
    struct STFTContext : public AlgorithmContext<arma::cx_vec> {
        FFTContext fft_context;

        STFTContext(int output_size, int frame_size)
                : AlgorithmContext(cx_vec(output_size)), fft_context(frame_size) {}
    };

    class STFTAlgorithm : public AlgorithmInterface<arma::vec, STFTContext> {
    protected:
        int sr;
        int hop_size;
        int window_half_size;
        span coefficient_range;

        NodeWindowVec<double> node_window;
        NodeFFT node_fft;

    public:
        STFTAlgorithm(int sr, const vec &window, int hop_size, const span &coefficient_range = span(0, 0),
                      bool mirror = false)
                : sr(sr), coefficient_range(coefficient_range), hop_size(hop_size), node_window(window),
                  node_fft(mirror), window_half_size(ACI(window.size())) {
            if (coefficient_range.a == coefficient_range.b) this->coefficient_range = span(0, window.size() / 2);
        }

        void processFrame(STFTContext &context, vec &frame, int i) override {
            node_window.apply(frame, &context);
            node_fft.apply(frame, &context.fft_context);
            context.getOutput() = context.fft_context.getOutput()(coefficient_range);
        }

        STFTContext createContext(vec &frame) override {
            return STFTContext(getCoefficientCount(), ACI(frame.size()));
        }

        int getFeatureRate() {
            return sr / hop_size;
        }

        vec getTimeVec(int frame_count) {
            return regspace<vec>(0, frame_count - 1) * (hop_size / (double) sr);
        }

        vec getFreqVec() {
            vec f = regspace<vec>(0, window_half_size) / window_half_size * (sr / 2.);
            return f(coefficient_range);
        }

        int getCoefficientCount() {
            return ACI(coefficient_range.b - coefficient_range.a + 1);
        }
    };

    using STFTPipeline = AlgorithmPipeline<vec, vec, cx_vec, cx_mat, STFTContext, STFTAlgorithm>;

    STFTPipeline build_stft_algorithm(const vec &input, int sr, const vec &window, int hop_size,
                                      const span &coefficient_range = span(0, 0), bool mirror = false) {
        STFTAlgorithm algorithm(sr, window, hop_size, coefficient_range, mirror);
        auto *frameFactory = new FrameFactoryVec<double>(input, ACI(window.size()), hop_size);
        auto *outputBuilder = new OutputBuilderMat<cx_double>(algorithm.getCoefficientCount());
        outputBuilder->resize(frameFactory->getFrameCount());

        return STFTPipeline(frameFactory, outputBuilder, algorithm);
    }

    cx_mat calculate_stft(const vec &input, int &feature_rate, vec &t, vec &f, int sr, const vec &window, int hop_size,
                          const span &coefficient_range = span(0, 0), bool mirror = false) {
        STFTPipeline pipeline = build_stft_algorithm(input, sr, window, hop_size, coefficient_range, mirror);
        AlgorithmSimpleRunner<STFTPipeline> runner;
        runner.run(&pipeline);
        return pipeline.getOutputBuilder()->getOutput();
    }

}}
