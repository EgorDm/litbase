//
// Created by egordm on 6-1-19.
//

#pragma once

#include <armadillo>
#include <algorithm_structure/algorithm_interface.h>
#include "frame_factory_vec.h"

using namespace arma;

namespace litsignal { namespace algorithm {
    template<typename T>
    class FrameFactoryVecPtr : public FrameFactoryInterface<Col<T>>, public FrameHopInterface {
    private:
        const T *input;
        int input_size;

    public:
        FrameFactoryVecPtr(T *const &input, int input_size, int frame_size, int hop_size)
                : FrameHopInterface(frame_size, hop_size), input(input), input_size(input_size) {}

        Col<T> create() override {
            return Col<T>(ACU(frame_size));
        }

        void fill(Col<T> &frame, int i) override {
            utils::fill_frame<float>(input, frame.memptr(), input_size, frame_size, getPos(i));
        }

        virtual int getPos(int i) {
            return i * hop_size - frame_half_size;
        }

        int getFrameCount() override {
            return static_cast<int>(std::round((input_size + frame_size) / (float) hop_size));
        }

        int getInputSize() override {
            return input_size;
        }
    };
}}
