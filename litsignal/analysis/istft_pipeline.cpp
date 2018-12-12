//
// Created by egordm on 12-12-18.
//

#include "istft_pipeline.h"
#include "../algorithm_structure/frame_factory_mat.h"

using namespace litsignal::analysis;

vec litsignal::analysis::calculate_istft(const cx_mat &input, const vec &window, int hop_size) {
    vec X;
    ISTFTPipeline istft_processor(new ISTFTPipeline::SimpleRunner(), input, X, window, hop_size);
    istft_processor.process();
    return X;
}

ISTFTPipeline::ISTFTPipeline(AlgorthmPipelineInterface::RunnerType *runner, const cx_mat &input, vec &output,
                             const vec &window, int hop_size)
        : AlgorthmPipelineInterface(input, output, runner, new FrameFactoryMat<cx_double>()),
          node_window(window), node_ifft(), hop_size(hop_size), window_sq(square(window)) {
    if(hop_size < 0) hop_size = ACI(window.size() / 2);
}

void ISTFTPipeline::preProcess() {
    int signal_length = (getFrameFactory()->getFrameCount(input) - 1) * hop_size + node_window.getWindowSize();
    output.zeros(ACU(signal_length));
    window_overlap_sum.set_size(ACU(signal_length));
}

void ISTFTPipeline::processFrame(IFFTContext &context, cx_vec &frame, int i) {
    vec &frame_processed = context.ifft;
    node_ifft.apply(frame, &context);
    node_window.apply(frame_processed, &context);

    // TODO: This is NOT threadsave
    uint pos = static_cast<uint>(i * hop_size);
    output(span(pos, pos + node_window.getWindowSize() - 1)) += frame_processed;
    window_overlap_sum(span(pos, pos + node_window.getWindowSize() - 1)) += window_sq;
}

void ISTFTPipeline::postProcess() {
    // Divide by window. But also avoid division by zero
    window_overlap_sum.elem(find(window_overlap_sum < 0.0001)).fill(1);
    output /= window_overlap_sum;

    // Signal was padded during stft. Probably
    uint window_half_size = static_cast<uint>(node_window.getWindowSize() / 2);
    output = output(span(window_half_size, output.size() - window_half_size - 1));
}

IFFTContext ISTFTPipeline::createContext(cx_vec &frame) {
    return IFFTContext(ACI(node_window.getWindowSize()));
}
