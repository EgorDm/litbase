//

// Created by egordm on 2-12-18.
//

#include "signal_container.h"

using namespace litsignal::structures;

SignalContainer::SignalContainer(int sample_rate, int channels)
        : TypedAudioContainerInterface<double>(AV_SAMPLE_FMT_DBLP, sample_rate, channels), data() {}

SignalContainer::SignalContainer(arma::mat data, int sample_rate)
        : TypedAudioContainerInterface<double>(AV_SAMPLE_FMT_DBLP, sample_rate, (int) data.n_cols),
          data(std::move(data)) {}

const uint8_t *SignalContainer::get_char_data() const {
    return reinterpret_cast<const uint8_t *>(data.memptr());
}

const double *SignalContainer::get_data() const {
    return data.memptr();
}

int SignalContainer::get_sample_count() const {
    return (int) data.n_rows;
}

void SignalContainer::set_sample_count(int sample_count) {
    data.resize(static_cast<const arma::uword>(sample_count), data.n_cols);
}

const arma::mat &SignalContainer::get_data_vec() const {
    return data;
}

void SignalContainer::set_data_vec(const arma::mat &data) {
    SignalContainer::data = data;
    channels = (int) data.n_cols;
}
