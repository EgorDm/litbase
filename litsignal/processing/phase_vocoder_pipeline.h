//
// Created by egordm on 13-12-18.
//

#pragma once

#include <armadillo>
#include "../algorithm_structure/algorithm_interface.h"
#include "../algorithm_structure/algorithm_simple_runner.h"

using namespace arma;
using namespace litsignal::algorithm;

namespace litsignal { namespace processing {
    vec calculate_phase_vocoder(const vec &input_signal, int sr, float alpha = 0.5f, int window_size = 2048,
                                int syn_hop_size = -1);

    struct PhaseVocoderContext {
        vec phase_current;
        vec phase_previous;
        vec phase_syn;

        PhaseVocoderContext(int window_size)
                : phase_current(window_size/2 + 1), phase_previous(window_size/2 + 1), phase_syn(window_size/2 + 1) {
        }
    };

    class PhaseVocoderPipeline : public AlgorthmPipelineInterface<cx_mat, cx_vec, vec, PhaseVocoderContext> {
    private:
        float alpha;
        int syn_hop_size;
        int sr;
        uvec input_hop_positions;
        uvec input_hop_sizes;

        const vec input_signal;
        vec window;

        cx_mat S;
        cx_mat Sy;

        int N;
        vec omega;

    public:
        using SimpleRunner = AlgorithmSimpleRunner<cx_vec, PhaseVocoderContext, OwnType>;

        PhaseVocoderPipeline(RunnerType *runner, const vec &input_signal, vec &output, int sr, int window_size = 2048,
                             float alpha = 0.5f, int syn_hop_size = -1);

        PhaseVocoderPipeline(RunnerType *runner, const vec &input_signal, vec &output, int sr, const vec &window,
                             float alpha = 0.5f, int syn_hop_size = -1);

        void preProcess() override;

        void processFrame(PhaseVocoderContext &context, cx_vec &frame, int i) override;

        void postProcess() override;

        PhaseVocoderContext createContext(cx_vec &frame) override;
    };
}}
