//
// Created by egordm on 12-12-18.
//

#include "stft_pipeline.h"
#include "../algorithm_structure/frame_factory_vec.h"

using namespace litsignal::algorithm;
using namespace litsignal::analysis;

cx_mat litsignal::analysis::calculate_stft(const vec &input, int &feature_rate, vec &t, vec &f, int sr,
                                           const vec &window, int hop_size, const span &coefficient_range,
                                           bool mirror) {
    cx_mat S;
    STFTPipeline stft_processor(new STFTPipeline::SimpleRunner(), input, S, feature_rate, t, f, sr, window, hop_size,
                                coefficient_range, mirror);
    stft_processor.process();

    return S;
}

STFTPipeline::STFTPipeline(RunnerType *runner, const vec &input, cx_mat &output, int &feature_rate, vec &t, vec &f,
                           int sr, const vec &window, int hop_size, const span &coefficient_range, bool mirror)
        : STFTPipeline(new FrameFactoryVec<double>(ACI(window.size()), hop_size), runner, input, output, feature_rate,
                       t, f, sr, window, coefficient_range, mirror) {}

STFTPipeline::STFTPipeline(FrameFactoryType *frameFactory, RunnerType *runner, const vec &input, cx_mat &output,
                           int &feature_rate, vec &t, vec &f, int sr, const vec &window, const span &coefficient_range,
                           bool mirror)
        : AlgorthmPipelineInterface(input, output, runner, frameFactory), sr(sr), coefficient_range(coefficient_range),
          feature_rate(feature_rate), t(t), f(f), node_window(window), node_fft(mirror) {
    if (coefficient_range.a == coefficient_range.b) this->coefficient_range = span(0, window.size() / 2);
}

void STFTPipeline::preProcess() {
    auto num_coeffs = coefficient_range.b - coefficient_range.a + 1;
    output.set_size(num_coeffs, ACU(getFrameFactory()->getFrameCount(input)));
}

void STFTPipeline::processFrame(FFTContext &context, vec &frame, int i) {
    node_window.apply(frame, &context);
    node_fft.apply(frame, &context);

    output(span::all, (const uword) i) = context.fft(coefficient_range);
}

void STFTPipeline::postProcess() {
    auto *ffactory = dynamic_cast<FrameFactoryVec<double> *>(getFrameFactory());

    feature_rate = sr / ffactory->getHopSize();
    t = regspace<vec>(0, ffactory->getFrameCount(input) - 1) * (ffactory->getHopSize() / (double) sr);
    f = regspace<vec>(0, ffactory->getFrameHalfSize()) / ffactory->getFrameHalfSize() * (sr / 2.);
    f = f(coefficient_range);
}

FFTContext STFTPipeline::createContext(vec &frame) {
    return FFTContext(ACI(frame.size()));
}
