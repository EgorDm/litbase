//
// Created by egordm on 2-12-18.
//

#pragma once

#include <armadillo>
#include <structures/audio_buffer_interface_deinterleaved.h>
#include <structures/audio_container.h>
#include "../litsignal_constants.h"

using namespace litaudio::structures;

namespace litsignal { namespace structures {
    template<typename T>
    class AudioSignalBuffer : public AudioBufferDeinterleavedInterface<T>, public AudioBufferModifiableInterface {
    protected:
        arma::Mat<T> buffers;

    public:
        explicit AudioSignalBuffer(int channel_count, int capacity = -1)
                : AudioBufferDeinterleavedInterface<T>(channel_count, capacity) {}

        explicit AudioSignalBuffer(const arma::Mat<T> &data)
                : AudioBufferDeinterleavedInterface<T>(data.n_cols, data.n_rows), buffers(data) {}

        T *getChannel(int channel) override {
            return buffers.colptr(channel);
        }

        // TODO: this should be used within an interface?
        auto getChannelContainer(int channel) {
            return buffers.col(channel);
        }

        arma::Mat<T> &getData() {
            return buffers;
        }

        void setChannelCount(int channel_count) override {
            AudioBufferInterface::channel_count = channel_count;
            buffers.resize(ACU(this->getSampleCount()), ACU(channel_count));
        }

        void setSampleCount(int sample_count) override {
            AudioBufferInterface::capacity = sample_count;
            buffers.resize(ACU(sample_count), ACU(this->getChannelCount()));
        }

        void reset() override {
            buffers.fill(0);
        }

        void clear() override {
            reset();
        }
    };

    template<typename T>
    class SignalContainer : public AudioContainer<AudioSignalBuffer<T>> {
    private:
        using Parent = AudioContainer<AudioSignalBuffer<T>>;
    public:
        explicit SignalContainer(int channel_count = -1, int sample_rate = -1, int capacity = 0)
                : Parent(new AudioSignalBuffer<T>(channel_count, capacity), sample_rate) {}

        explicit SignalContainer(const arma::Mat<T> &data, int sample_rate = -1)
                : Parent(new AudioSignalBuffer<T>(data), sample_rate) {}
    };

    using SignalContainerDbl = SignalContainer<double>;

    using SignalContainerFlt = SignalContainer<float>;
}}


