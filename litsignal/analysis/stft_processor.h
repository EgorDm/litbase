//
// Created by egordm on 9-12-18.
//

#pragma once

#include <armadillo>
#include "../processing_helpers/fft_processor.h"
#include "../litsignal_constants.h"

using namespace litsignal::helpers;
using namespace arma;

namespace litsignal { namespace analysis { namespace fourier_analysis {
    class STFTProcessor : public FFTProcessor<cx_mat> {
    private:
        int sr;
        span coefficient_range;

        int &feature_rate;
        vec &t;
        vec &f;

    public:
        STFTProcessor(vec &input, cx_mat &output, int &feature_rate, vec &t, vec &f, int sr, const vec &window,
                      int hop_size, const std::vector<int> &coefficient_range = std::vector<int>())
                : FFTProcessor(input, output, window, hop_size),
                  sr(sr), feature_rate(feature_rate), t(t), f(f) {

            if (coefficient_range.size() < 2) this->coefficient_range = span(0, ACU(frame_size / 2));
            else this->coefficient_range = span(ACU(coefficient_range.front()), ACU(coefficient_range.back()));

            // Disable mirroring if we dont use the second half
            if(this->coefficient_range.b <= frame_half_size) mirror = false;
        }

    protected:
        void preProcess() override {
            FrameProcessorInterface::preProcess();

            auto num_coeffs = coefficient_range.b - coefficient_range.a + 1;
            output.set_size(num_coeffs, ACU(frame_count));
        }

        void postProcess() override {
            FrameProcessorInterface::postProcess();

            feature_rate = sr / hop_size;
            t = regspace<vec>(0, frame_count - 1) * (hop_size / (double) sr);
            f = regspace<vec>(0, frame_half_size) / (double) frame_half_size * (sr / 2.);
            f = f(coefficient_range);
        }

        void processFrame(FFTProcessorContext *context, arma::vec &frame, int i) override {
            FFTProcessor::processFrame(context, frame, i);

            output(span::all, (const uword) i) = context->fft_res(coefficient_range);
        }
    };
}}}

