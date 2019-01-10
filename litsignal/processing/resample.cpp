//
// Created by egordm on 3-12-18.
//

#include <utils/debug.h>
#include "resample.h"
#include "resample_internal.h"
#include "../utils/mat_utils.h"

using namespace litcore;
using namespace litsignal::processing;

SignalResampler::SignalResampler(SignalContainer<double> *input, SignalContainer<double> *output)
        : input(input), output(output) {}

bool SignalResampler::resample() {
    try {
        const int p = (const int) roundf(1000 * output->getSampleRate() / input->getSampleRate());
        const int q = 1000;

        output->copyFormat(input);
        output->setSampleCount(ACI(input->getSampleCount() * (p / (float) q)));

        for (int c = 0; c < input->getChannelCount(); ++c) {
            output->getTypedBuffer()->getChannelContainer(c) =
                    detail::resample(input->getTypedBuffer()->getChannelContainer(c), p, q);
        }
    } catch (std::runtime_error e) {
        debug::log_error(SignalResampler_TAG, e.what());
        return false;
    }

    return true;
}
