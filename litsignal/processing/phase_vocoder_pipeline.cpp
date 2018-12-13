//
// Created by egordm on 13-12-18.
//

#include "phase_vocoder_pipeline.h"
#include "../algorithm_structure/frame_factory_mat.h"
#include "../litsignal_constants.h"
#include "../utils/window_utils.h"

using namespace litsignal::processing;
using namespace litsignal;

vec litsignal::processing::calculate_phase_vocoder(const vec &input, float alpha, int window_size, int syn_hop_size) {
    vec X;
    PhaseVocoderPipeline processor(new PhaseVocoderPipeline::SimpleRunner(), input, X, window_size, alpha, syn_hop_size);
    processor.process();

    return X;
}

PhaseVocoderPipeline::PhaseVocoderPipeline(AlgorthmPipelineInterface::RunnerType *runner, const vec &input_signal,
                                           vec &output, int window_size, float alpha, int syn_hop_size)
        : PhaseVocoderPipeline(runner, input_signal, output, window::sinw(window_size, 1), alpha, syn_hop_size) {}

PhaseVocoderPipeline::PhaseVocoderPipeline(AlgorthmPipelineInterface::RunnerType *runner, const vec &input_signal,
                                           vec &output, const vec &window, float alpha, int syn_hop_size)
        : AlgorthmPipelineInterface(S, output, runner, new FrameFactoryMat<cx_double>()),
          input_signal(input_signal), alpha(alpha), syn_hop_size(syn_hop_size) {
    if(syn_hop_size <= 0) this->syn_hop_size = ACI(window.size() / 4);
}

/*PhaseVocoderPipeline::PhaseVocoderPipeline(const cx_mat &input, vec &output,
                                           AlgorthmPipelineInterface::RunnerType *runner,
                                           AlgorthmPipelineInterface::FrameFactoryType *frame_factory,
                                           const vec &input_signal) : AlgorthmPipelineInterface(input, output, runner,
                                                                                                frame_factory),
                                                                      input_signal(input_signal) {}*/

void PhaseVocoderPipeline::preProcess() {
    int output_signal_length = ACI(std::ceil(input_signal.size() * alpha));
    int input_signal_length = ACI(input_signal.size());
    int window_half_size = ACI(std::round(window.size() / 2.f));
    uvec output_hop_positions = regspace<uvec>(0, syn_hop_size, ACU(output_signal_length + window_half_size));

    input_hop_positions.resize(output_hop_positions.size());
    float gamma = input_signal_length / (float) output_signal_length;
    for (int i = 0; i < output_hop_positions.size(); ++i) {
        input_hop_positions[i] = ACU(std::round(output_hop_positions[i] * gamma));
    }

    input_hop_sizes.resize(input_hop_positions.size());
    input_hop_sizes(span(1, input_hop_sizes.size() - 1)) = input_hop_positions(span(1, input_hop_positions.size() - 1))
                                                           -
                                                           input_hop_positions(span(0, input_hop_positions.size() - 2));
    input_hop_sizes[0] = 0;

    std::cout << input_hop_positions(span(0, 100)).t() << std::endl;
    std::cout << input_hop_sizes(span(0, 100)).t() << std::endl;
    int i = 0;
}

void PhaseVocoderPipeline::processFrame(PhaseVocoderContext &context, cx_vec &frame, int i) {

}

void PhaseVocoderPipeline::postProcess() {

}

PhaseVocoderContext PhaseVocoderPipeline::createContext(cx_vec &frame) {
    return PhaseVocoderContext();
}
