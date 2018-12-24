#include <utility>

//
// Created by egordm on 11-12-18.
//

#pragma once

#include <armadillo>
#include "algorithm_interface.h"

using namespace arma;

namespace litsignal { namespace algorithm {
    template<typename T>
    class FrameFactoryVec : public FrameFactoryInterface<Col<T>, Col<T>> {
    private:
        using ParentType = FrameFactoryInterface<Col<T>, Col<T>>;

        int frame_size;
        int frame_half_size;
        int hop_size;

    public:
        FrameFactoryVec(const Col<T> &input, int frame_size, int hop_size)
                : FrameFactoryInterface<Col<T>, Col<T>>(input), frame_size(frame_size), hop_size(hop_size),
                  frame_half_size(frame_size / 2) {}

        Col<T> create() override {
            return Col<T>(ACU(frame_size));
        }

        void fill(Col<T> &frame, int i) override {
            int start = getPos(i);
            int offset = 0;
            int end = start + frame_size;
            T *frame_ptr = frame.memptr();

            // Pad the frame start
            if (start < 0) {
                offset = std::min(abs(start), end);
                start = start + offset;
                std::memset(frame_ptr, 0, offset * sizeof(T));
            }

            // Pad the frame end
            if (end > ParentType::input.size()) {
                int diff = static_cast<int>(end - ParentType::input.size());
                end = static_cast<int>(ParentType::input.size());
                std::memset(frame_ptr + frame_size - diff, 0, diff * sizeof(T));
            }

            // Fill the frame
            std::memcpy(frame_ptr + offset, ParentType::input.memptr() + start, (end - start) * sizeof(T));
        }

        virtual int getPos(int i) {
            return -frame_half_size + i * hop_size;
        }

        int getFrameCount() override {
            return static_cast<int>(std::ceil(ParentType::input.size() / (float) hop_size)) + 1;
        }

        int getFrameSize() const {
            return frame_size;
        }

        virtual int getHopSize() const {
            return hop_size;
        }
    };

    template<typename T>
    class FrameFactoryVecP : public FrameFactoryVec<T> {
    private:
        uvec hop_positions;

    public:
        FrameFactoryVecP(int frame_size, uvec hop_positions)
                : FrameFactoryVec<T>(frame_size, 0), hop_positions(std::move(hop_positions)) {}

        int getFrameCount(const Col<T> &input) override {
            return ACI(hop_positions.size());
        }

        int getHopSize() const override {
            return hop_positions.size() < 2 ? 0 : ACI(hop_positions[1] - hop_positions[0]);
        }

    protected:
        int getPos(int i) override {
            return ACI(hop_positions[i]);
        }
    };
}}