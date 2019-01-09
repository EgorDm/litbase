//
// Created by egordm on 3-12-18.
//

#pragma once

#include "../structures/signal_container.h"

using namespace litsignal::structures;

#define SignalResampler_TAG "SignalResampler"

namespace litsignal { namespace processing {
    class SignalResampler {
    private:
        SignalContainer<double> *input;
        SignalContainer<double> *output;

    public:
        SignalResampler(SignalContainer<double> *input, SignalContainer<double> *output);

        bool resample();
    };
}}

