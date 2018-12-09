//
// Created by egordm on 9-12-18.
//

#pragma once

#include "frame_processor.h"

namespace litsignal { namespace helpers {
    template<typename C, typename O>
    class WindowProcessor : public FrameProcessor<C, O> {
    protected:
        arma::vec window;

    public:
        WindowProcessor(const arma::vec &input, O &output, const arma::vec &window, int hop_size)
                : FrameProcessor<C, O>(input, output, static_cast<int>(window.size()), hop_size), window(window) {

        }

    protected:
        void processFrame(C *context, arma::vec &frame, int i) override {
            frame %= window; // Apply the window function
        }
    };
}}
