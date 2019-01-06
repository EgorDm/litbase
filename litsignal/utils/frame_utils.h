//
// Created by egordm on 14-12-18.
//

#pragma once

#include <armadillo>
#include "../litsignal_constants.h"

namespace litsignal { namespace utils {
    inline arma::uvec interpolate_hop_positions(int source_signal_length, int target_signal_length, int target_hop_size,
                                         arma::uvec &target_hop_positions) {
        target_hop_positions = arma::regspace<arma::uvec>(0, target_hop_size, ACU(target_signal_length));

        arma::uvec input_hop_positions(target_hop_positions.size());
        float gamma = source_signal_length / (float) target_signal_length;
        for (int i = 0; i < input_hop_positions.size(); ++i) {
            input_hop_positions[i] = ACU(std::round(target_hop_positions[i] * gamma));
        }

        return input_hop_positions;
    }

    inline arma::uvec calculate_position_increments(const arma::uvec &hop_positions) {
        arma::uvec hop_increments(hop_positions.size());
        hop_increments(arma::span(1, hop_positions.size() - 1)) =
                hop_positions(arma::span(1, hop_positions.size() - 1))
                - hop_positions(arma::span(0, hop_positions.size() - 2));
        hop_increments[0] = 0;

        return hop_increments;
    }

    template <typename T>
    inline void fill_frame(const T* in, T* out, int in_size, int frame_size, int cursor) {
        int start = cursor;
        int offset = 0;
        int end = start + frame_size;

        // Pad the frame start
        if (start < 0) {
            offset = std::min(abs(start), end);
            start = start + offset;
            std::memset(out, 0, offset * sizeof(T));
        }

        // Pad the frame end
        if (end > in_size) {
            int diff = end - in_size;
            end = in_size;
            std::memset(out + frame_size - diff, 0, diff * sizeof(T));
        }

        // Fill the frame
        std::memcpy(out + offset, in + start, (end - start) * sizeof(T));
    }
}}