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
        SignalContainer *input;
        SignalContainer *output;

    public:
        SignalResampler(SignalContainer *input, SignalContainer *output);

        bool resample();
    };
}}

