//
// Created by egordm on 13-12-18.
//

#include "phase_vocoder_pipeline.h"
#include "../algorithm_structure/frame_factory_mat.h"
#include "../litsignal_constants.h"
#include "../utils/window_utils.h"
#include "../utils/frame_utils.h"
#include "../analysis/stft_pipeline.h"
#include "../algorithm_structure/frame_factory_vec.h"
#include "../analysis/istft_pipeline.h"

using namespace litsignal::processing;
using namespace litsignal;

vec litsignal::processing::calculate_phase_vocoder(const vec &input, int sr, float alpha, int window_size,
                                                   int syn_hop_size) {
    vec X;
    PhaseVocoderPipeline processor(new PhaseVocoderPipeline::SimpleRunner(), input, X, sr, window_size, alpha,
                                   syn_hop_size);
    processor.process();

    return X;
}

PhaseVocoderPipeline::PhaseVocoderPipeline(AlgorthmPipelineInterface::RunnerType *runner, const vec &input_signal,
                                           vec &output, int sr, int window_size, float alpha, int syn_hop_size)
        : PhaseVocoderPipeline(runner, input_signal, output, sr, window::sinw(window_size, 1), alpha, syn_hop_size) {}

PhaseVocoderPipeline::PhaseVocoderPipeline(AlgorthmPipelineInterface::RunnerType *runner, const vec &input_signal,
                                           vec &output, int sr, const vec &window, float alpha, int syn_hop_size)
        : AlgorthmPipelineInterface(S, output, runner, new FrameFactoryMat<cx_double>()),
          input_signal(input_signal), alpha(alpha), syn_hop_size(syn_hop_size), window(window), sr(sr) {
    if (syn_hop_size <= 0) this->syn_hop_size = ACI(window.size() / 4);
}

void PhaseVocoderPipeline::preProcess() {
    // Hop and hop size setting
    int output_signal_length = ACI(std::ceil(input_signal.size() * alpha));
    int input_signal_length = ACI(input_signal.size());
    int window_half_size = ACI(std::round(window.size() / 2.f));
    uvec output_hop_positions;
    input_hop_positions = utils::interpolate_hop_positions(input_signal_length, output_signal_length + window_half_size,
                                                           syn_hop_size, output_hop_positions);
    input_hop_sizes = utils::calculate_position_increments(input_hop_positions);

    // STFT
    int feature_rate;
    vec t, f;
    analysis::STFTPipeline processor(new FrameFactoryVecP<double>(ACI(window.size()), input_hop_positions),
                                     new analysis::STFTPipeline::SimpleRunner(), input_signal, S, feature_rate, t, f,
                                     sr, window);
    processor.process();

    // prepare processing
    Sy.set_size(size(S));
    N = ACI(window.size());
    vec k = regspace<vec>(0, N / 2);
    omega = (constants::PI2 * k) / N;

    Sy(span::all, 0) = S(span::all, 0);
}

void angle_calc(cx_double *input, double *output, int count) {
    double x;
    double y;
    for (int i = 0; i < count; ++i) {
        x = input[i].real();
        y = input[i].imag();
        output[i] = atan2(y, x);
    }
}

void PhaseVocoderPipeline::processFrame(PhaseVocoderContext &context, cx_vec &frame, int i) {
    angle_calc(frame.memptr(), context.phase_current.memptr(), frame.size());
    vec delta = (context.phase_current - context.phase_previous) - omega * input_hop_sizes[i];
    context.phase_previous = context.phase_current;

    delta = delta - constants::PI2 * round(delta / (constants::PI2));
    vec ipa_hop = (omega + delta / input_hop_sizes[i]) * syn_hop_size;

    if(i == 0) {
        context.phase_syn = context.phase_current;
    } else {
        context.phase_syn = context.phase_syn + ipa_hop;
    }

    vec theta = context.phase_syn;// - context.phase_current;
    vec mag = abs(S.col(i));

    for (int j = 0; j < mag.size(); ++j) {
        Sy(j, i) = cx_double(mag[j] * cos(context.phase_syn[j]), mag[j] * sin(context.phase_syn[j]));
    }
}

void PhaseVocoderPipeline::postProcess() {
    output = analysis::calculate_istft(Sy, window, syn_hop_size);
    output /= max(output);
}

PhaseVocoderContext PhaseVocoderPipeline::createContext(cx_vec &frame) {
    return PhaseVocoderContext(window.size());
}
