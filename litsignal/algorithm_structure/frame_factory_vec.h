//
// Created by egordm on 11-12-18.
//

#pragma once

#include <armadillo>
#include "algorithm_interface.h"

using namespace arma;

namespace litsignal { namespace algorithm {
    class FrameFactoryVec : public FrameFactoryInterface<vec, vec> {
    private:
        int frame_size;
        int frame_half_size;
        int hop_size;

    public:
        FrameFactoryVec(int frame_size, int hop_size)
                : frame_size(frame_size), hop_size(hop_size), frame_half_size(frame_size / 2) {}

        vec create() override {
            return arma::vec(ACU(frame_size));
        }

        void fill(const vec &input, vec &frame, int i) override {
            int start = -frame_half_size + i * hop_size;
            int offset = 0;
            int end = start + frame_size;
            double *frame_ptr = frame.memptr();

            // Pad the frame start
            if (start < 0) {
                offset = std::min(abs(start), end);
                start = start + offset;
                std::memset(frame_ptr, 0, offset * sizeof(double));
            }

            // Pad the frame end
            if (end > input.size()) {
                int diff = static_cast<int>(end - input.size());
                end = static_cast<int>(input.size());
                std::memset(frame_ptr + frame_size - diff, 0, diff * sizeof(double));
            }

            // Fill the frame
            std::memcpy(frame_ptr + offset, input.memptr() + start, (end - start) * sizeof(double));
        }

        int getFrameCount(const vec &input) override {
            return static_cast<int>(std::ceil(input.size() / (float) hop_size));
        }

        int getFrameSize() const {
            return frame_size;
        }

        int getFrameHalfSize() const {
            return frame_half_size;
        }

        int getHopSize() const {
            return hop_size;
        }
    };
}}