//
// Created by egordm on 2-12-18.
//

#pragma once

#include <armadillo>
#include <structures/audio_container_interfacez.h>
#include <structures/typed_audio_container_interface.h>

using namespace litaudio::structures;

namespace litsignal { namespace structures {
    class SignalContainer : public TypedAudioContainerInterface<double> {
    private:
        arma::mat data;

    public:
        SignalContainer();

        SignalContainer(int sample_rate, int channels);

        SignalContainer(arma::mat data, int sample_rate);

        void clear() override;

        const uint8_t *getByteData(int channel) const override;

        const double *getData(int channel) const override;

        int getSampleCount() const override;

        void setSampleCount(int sample_count) override;

        const arma::mat &get_data_vec() const;

        void set_data_vec(const arma::mat &data);
    };
}}


