//
// Created by egordm on 2-12-18.
//

#pragma once

#include <armadillo>
#include <structures/audio_container_interface.h>
#include <structures/typed_audio_container_interface.h>

using namespace litaudio::structures;

namespace litsignal { namespace structures {
    class SignalContainer : public TypedAudioContainerInterface<double> {
    private:
        arma::mat data;

    public:
        SignalContainer(int sample_rate, int channels);

        SignalContainer(arma::mat data, int sample_rate);

        const uint8_t *get_char_data() const override;

        const double *get_data() const override;

        int get_sample_count() const override;

        void set_sample_count(int sample_count) override;

        const arma::mat &get_data_vec() const;

        void set_data_vec(const arma::mat &data);
    };
}}


