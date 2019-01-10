//
// Created by egordm on 7-1-19.
//

#pragma once

#include <cstring>
#include <utils/litaudio_utils.h>
#include "audio_buffer_interface.h"

namespace litaudio { namespace structures {
    class AudioBufferDeinterleavedBaseInterface : public AudioBufferInterface {
    public:
        using AudioBufferInterface::AudioBufferInterface;

        const uint8_t *getChannelPtr(int channel) const {
            return const_cast<AudioBufferDeinterleavedBaseInterface *>(this)->getChannelPtr(channel);
        }

        virtual uint8_t *getChannelPtr(int channel) = 0;

        std::vector<uint8_t *> getDataFull() override {
            std::vector<uint8_t *> ret;
            for (int c = 0; c < getChannelCount(); ++c) ret.push_back(getChannelPtr(c));
            return ret;
        }

        std::vector<const uint8_t *> getDataFullC() const override {
            std::vector<const uint8_t *> ret;
            for (int c = 0; c < getChannelCount(); ++c) ret.push_back(getChannelPtr(c));
            return ret;
        }

        bool copyData(const AudioBufferInterface *srcr) override {
            auto src = dynamic_cast<const AudioBufferDeinterleavedBaseInterface *>(srcr);
            if (src == nullptr) return false; // Different buffer types unsupported
            for (int c = 0; c < src->getChannelCount(); ++c) {
                std::memcpy(this->getChannelPtr(c), src->getChannelPtr(c),
                            (size_t) this->getSampleCount() * getSampleSize());
            }
            return true;
        }
    };

    template<typename T>
    class AudioBufferDeinterleavedInterface : public AudioBufferDeinterleavedBaseInterface {
    public:
        AudioBufferDeinterleavedInterface(int channel_count, int capacity)
                : AudioBufferDeinterleavedBaseInterface(channel_count, capacity, utils::deduce_format<T>(true)) {}

        const T *getChannel(int channel) const {
            return const_cast<AudioBufferDeinterleavedInterface *>(this)->getChannel(channel);
        }

        virtual T *getChannel(int channel) = 0;

        uint8_t *getChannelPtr(int channel) override {
            return reinterpret_cast<uint8_t *>(getChannel(channel));
        }
    };

    template<typename T>
    class AudioBufferDeinterleaved
            : public AudioBufferDeinterleavedInterface<T>, public AudioBufferModifiableInterface {
    protected:
        std::vector<std::vector<T>> buffers;

    public:
        explicit AudioBufferDeinterleaved(int channel_count = -1, int capacity = 0)
                : AudioBufferDeinterleavedInterface<T>(channel_count, capacity) {
            int data_per_sample = this->getSampleSize() / sizeof(T);
            for (int i = 0; i < channel_count; ++i)
                buffers.emplace_back(std::vector<T>((unsigned long) this->getSampleCount() * data_per_sample));
        }

        T *getChannel(int channel) override {
            return buffers[channel].data();
        }

        void reset() override {
            for (int i = 0; i < this->getChannelCount(); ++i)
                std::memset(this->getChannel(i), 0, (size_t) (this->getSampleCount() * this->getSampleSize()));
        }

        void clear() override {
            for (auto &buffer : AudioBufferDeinterleaved<T>::buffers) buffer.clear();
        }

        void setChannelCount(int channel_count) override {
            AudioBufferInterface::channel_count = channel_count;
            buffers.resize(static_cast<unsigned long>(channel_count));
        }

        void setSampleCount(int sample_count) override {
            AudioBufferInterface::capacity = sample_count;
            int data_per_sample = this->getSampleSize() / sizeof(T);
            for (auto &buffer : AudioBufferDeinterleaved<T>::buffers) buffer.resize(sample_count * data_per_sample);
        }
    };
}}