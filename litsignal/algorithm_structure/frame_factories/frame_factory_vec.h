#include <utility>

//
// Created by egordm on 11-12-18.
//

#pragma once

#include <armadillo>
#include <utils/frame_utils.h>
#include "algorithm_structure/algorithm_interface.h"

using namespace arma;

namespace litsignal { namespace algorithm {
    class FrameHopInterface {
    protected:
        int frame_size;
        int frame_half_size;
        int hop_size;

    public:
        FrameHopInterface(int frame_size, int hop_size)
                : frame_size(frame_size), hop_size(hop_size), frame_half_size(frame_size / 2) {}

        int getFrameSize() const {
            return frame_size;
        }

        void setFrameSize(int frame_size) {
            FrameHopInterface::frame_size = frame_size;
            FrameHopInterface::frame_half_size = frame_size / 2;
        }

        int getHopSize() const {
            return hop_size;
        }

        void setHopSize(int hop_size) {
            FrameHopInterface::hop_size = hop_size;
        }
    };

    template<typename T>
    class FrameFactoryVec : public FrameFactoryInterface<Col<T>>, public FrameHopInterface {
    private:
        const Col<T> &input;

    public:
        FrameFactoryVec(const Col<T> &input, int frame_size, int hop_size)
                : FrameHopInterface(frame_size, hop_size), input(input) {
            int i = 0;
        }

        Col<T> create() override {
            return Col<T>(ACU(frame_size));
        }

        void fill(Col<T> &frame, int i) override {
            utils::fill_frame<T>(input.memptr(), frame.memptr(), ACI(input.size()), frame_size, getPos(i));
        }

        virtual int getPos(int i) {
            return i * hop_size - frame_half_size;
        }

        int getFrameCount() override {
            return ACI(std::floor((input.size() + frame_half_size) / (float) hop_size));
        }

        int getInputSize() override {
            return ACI(input.size());
        }
    };
}}