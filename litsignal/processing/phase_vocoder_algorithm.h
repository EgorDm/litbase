//
// Created by egordm on 25-12-18.
//

#pragma once

#include <armadillo>
#include <algorithm_structure/algorithm_interface.h>
#include <algorithm_structure/node_fft.h>
#include <analysis/stft_algorithm.h>
#include <analysis/istft_algorithm.h>
#include <utils/window_utils.h>

using namespace arma;
using namespace litsignal::algorithm;
using namespace litsignal::analysis;

namespace litsignal { namespace processing {
    struct PVContext : public AlgorithmContext<vec> {
        STFTContext stft_context;
        IFFTContext istft_context;

        vec phase_current, phase_syn;
        cx_vec syn_data;

        PVContext(int window_size)
                : AlgorithmContext<vec>(vec()),
                  phase_current(ACU(window_size / 2 + 1), fill::zeros),
                  phase_syn(ACU(window_size / 2 + 1), fill::zeros),
                  syn_data(ACU(window_size / 2 + 1)),
                  stft_context(window_size, window_size),
                  istft_context(window_size) {
        }

        vec &getOutput() override {
            return istft_context.getOutput();
        }
    };

    class PhaseVocoderAlgorithm : public AlgorithmInterface<arma::vec, PVContext> {
    private:
        float alpha;
        int in_hop_size, syn_hop_size;
        int window_size_half;

        STFTAlgorithm node_stft;
        ISTFTAlgorithm node_istft;

        int N;
        vec omega;

    public:
        /**
         *
         * @param sr
         * @param window
         * @param syn_hop_size should be 1/4th of the window
         * @param alpha
         */
        PhaseVocoderAlgorithm(const vec &window, int syn_hop_size, float alpha)
                : alpha(alpha), syn_hop_size(syn_hop_size), node_stft(window), node_istft(window, syn_hop_size),
                  window_size_half(ACI(window.size() / 2 + 1)) {
            in_hop_size = ACI(roundf(syn_hop_size / alpha));
            N = ACI(window.size());
            vec k = regspace<vec>(0, N / 2);
            omega = (constants::PI2 * k) / N;
        }

        // TODO: suggestion for later. Variable window size by putting the size in context. But keeping max same
        void processFrame(PVContext &context, vec &frame, int k) override {
            node_stft.processFrame(context.stft_context, frame, k);

            double phase_previous, delta, y_unwrap, mag;
            cx_double *in = context.stft_context.getOutput().memptr();
            for (int i = 0; i < window_size_half; ++i) {
                phase_previous = context.phase_current[i];
                context.phase_current[i] = atan2(in->imag(), in->real()); // angle(x+yj) = atan2(y,x);
                delta = (context.phase_current[i] - phase_previous) - omega[i] * in_hop_size;
                delta = delta - constants::PI2 * round(delta / (constants::PI2)); // Put delta in 2PI range
                y_unwrap = omega[i] + delta / in_hop_size;

                if (k != 0) context.phase_syn[i] += y_unwrap * syn_hop_size;
                else context.phase_syn[i] = context.phase_current[i];

                mag = abs(*in);
                context.syn_data[i] = cx_double(mag * cos(context.phase_syn[i]), mag * sin(context.phase_syn[i]));

                in++;
            }

            node_istft.processFrame(context.istft_context, context.syn_data, k);
        }

        PVContext createContext(vec &frame) override {
            return PVContext(ACI(frame.size()));
        }

        int getInHopSize() const {
            return in_hop_size;
        }
    };

    using PhaseVocoderPipeline = AlgorithmPipeline<vec, vec, vec, vec, PVContext, PhaseVocoderAlgorithm>;

    PhaseVocoderPipeline build_pv_algorithm(const vec &input, const vec &window, int syn_hop_size, float alpha) {
        PhaseVocoderAlgorithm algorithm(window, syn_hop_size, alpha);
        auto *frameFactory = new FrameFactoryVec<double>(input, ACI(window.size()), algorithm.getInHopSize());
        int signal_length = (frameFactory->getFrameCount() - 1) * syn_hop_size + ACI(window.size());
        auto *outputBuilder = new OutputBuilderVec<double>(syn_hop_size);
        outputBuilder->resize(signal_length);

        return PhaseVocoderPipeline(frameFactory, outputBuilder, algorithm);
    }

    vec calculate_pv(const vec &input, int sr, float alpha = 0.5f, int window_size = 2048) {
        vec window = window::sinw(window_size, 1);
        PhaseVocoderPipeline pipeline = build_pv_algorithm(input, window, ACI(window.size() / 4), alpha);
        AlgorithmSimpleRunner<PhaseVocoderPipeline> runner;
        runner.run(&pipeline);
        return pipeline.getOutputBuilder()->getOutput();
    }
}}
