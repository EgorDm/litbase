//
// Created by egordm on 9-12-18.
//

#pragma once

#include <armadillo>
#include "frame_processor_interface.h"

namespace litsignal { namespace helpers {
    template<typename C, typename O>
    class FrameProcessor : public FrameProcessorInterface<arma::vec, C, O> {
    private:
        using parentClass = FrameProcessorInterface<arma::vec, C, O>;

    protected:
        int frame_half_size;

    public:
        FrameProcessor(const arma::vec &input, O &output, int frame_size, int hop_size)
                : parentClass(input, output, std::ceil(input.size() / (float) hop_size), frame_size, hop_size),
                  frame_half_size(frame_size / 2) {

        }

    protected:
        void fillFrame(arma::vec &frame, int i) override {
            int start = -frame_half_size + i * parentClass::hop_size;
            int offset = 0;
            int end = start + parentClass::frame_size;
            double *frame_ptr = frame.memptr();

            // Pad the frame start
            if (start < 0) {
                offset = std::min(abs(start), end);
                start = start + offset;
                std::memset(frame_ptr, 0, offset * sizeof(double));
            }

            // Pad the frame end
            if (end > parentClass::input.size()) {
                int diff = static_cast<int>(end - parentClass::input.size());
                end = static_cast<int>(parentClass::input.size());
                std::memset(frame_ptr + parentClass::frame_size - diff, 0, diff * sizeof(double));
            }

            // Fill the frame
            std::memcpy(frame_ptr + offset, parentClass::input.memptr() + start, (end - start) * sizeof(double));
        }

        arma::vec createFrame(int size) override {
            return arma::vec(size);
        }
    };
}}

