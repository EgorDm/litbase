//

// Created by egordm on 2-12-18.
//

#include "signal_container.h"

using namespace litsignal::structures;

SignalContainer::SignalContainer()
        : TypedAudioContainerInterface<double>(AV_SAMPLE_FMT_DBLP) {}

SignalContainer::SignalContainer(int sample_rate, int channels)
        : TypedAudioContainerInterface<double>(AV_SAMPLE_FMT_DBLP, channels, sample_rate), data() {}

SignalContainer::SignalContainer(arma::mat data, int sample_rate)
        : TypedAudioContainerInterface<double>(AV_SAMPLE_FMT_DBLP, (int) data.n_cols, sample_rate),
          data(std::move(data)) {}

void SignalContainer::clear() {
    AudioContainerInterfaceZ::clear();
    data = arma::mat(0, static_cast<const arma::uword>(getChannelCount()));
}

const uint8_t *SignalContainer::getByteData(int channel) const {
    return reinterpret_cast<const uint8_t *>(data.colptr(static_cast<const arma::uword>(channel)));
}

const double *SignalContainer::getData(int channel) const {
    return data.colptr(static_cast<const arma::uword>(channel));
}

int SignalContainer::getSampleCount() const {
    return (int) data.n_rows;
}

void SignalContainer::setSampleCount(int sample_count) {
    data.resize(static_cast<const arma::uword>(sample_count), getChannelCount());
}

const arma::mat &SignalContainer::get_data_vec() const {
    return data;
}

void SignalContainer::set_data_vec(const arma::mat &data) {
    SignalContainer::data = data;
    channels = (int) data.n_cols;
}
