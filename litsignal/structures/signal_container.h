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
                : AudioBufferDeinterleavedInterface<T>(channel_count, capacity, sizeof(T)) {}

        explicit AudioSignalBuffer(const arma::Mat<T> &data)
                : AudioBufferDeinterleavedInterface<T>(data.n_cols, data.n_rows, sizeof(T)), buffers(data) {}

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
        explicit SignalContainer(AVSampleFormat format = AV_SAMPLE_FMT_NONE, int channel_count = -1,
                                 int sample_rate = -1, int capacity = 0)
                : Parent(new AudioSignalBuffer<T>(channel_count, capacity), format, sample_rate) {}

        explicit SignalContainer(const arma::Mat<T> &data, AVSampleFormat format = AV_SAMPLE_FMT_NONE,
                                 int sample_rate = -1)
                : Parent(new AudioSignalBuffer<T>(data), format, sample_rate) {}
    };

    class SignalContainerDbl : public SignalContainer<double> {
    public:
        explicit SignalContainerDbl(int channel_count = -1, int sample_rate = -1, int capacity = 0)
                : SignalContainer(AV_SAMPLE_FMT_DBLP, channel_count, sample_rate, capacity) {}

        SignalContainerDbl(const arma::Mat<double> &data, int sample_rate = -1)
                : SignalContainer(data, AV_SAMPLE_FMT_DBLP, sample_rate) {}
    };

    class SignalContainerFlt : public SignalContainer<float> {
    public:
        explicit SignalContainerFlt(int channel_count = -1, int sample_rate = -1, int capacity = 0)
                : SignalContainer(AV_SAMPLE_FMT_FLTP, channel_count, sample_rate, capacity) {}

        SignalContainerFlt(const arma::Mat<float> &data, int sample_rate = -1)
                : SignalContainer(data, AV_SAMPLE_FMT_FLTP, sample_rate) {}
    };
}}


