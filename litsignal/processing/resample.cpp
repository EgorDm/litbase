//
// Created by egordm on 3-12-18.
//

#include <utils/debug.h>
#include "resample.h"
#include "resample_internal.h"
#include "../utils/mat_utils.h"

using namespace litcore;
using namespace litsignal::processing;

SignalResampler::SignalResampler(SignalContainer *input, SignalContainer *output) : input(input), output(output) {}

bool SignalResampler::resample() {
    try {
        const int p = (const int) roundf(1000 * output->getSampleRate() / input->getSampleRate());
        const int q = 1000;

        std::vector<arma::vec> channels;
        channels.reserve(static_cast<unsigned long>(input->getChannelCount()));
        for (int c = 0; c < input->getChannelCount(); ++c) {
            channels.push_back(detail::resample(input->get_data_vec().col(c), p, q));
        }

        output->set_data_vec(mat_utils::mat_from_vecs(channels));
    } catch (std::runtime_error e) {
        debug::log_error(SignalResampler_TAG, e.what());
        return false;
    }

    return true;
}
