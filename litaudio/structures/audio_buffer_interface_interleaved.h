//
// Created by egordm on 7-1-19.
//

#pragma once

#include "audio_buffer_interface.h"

namespace litaudio { namespace structures {
    template<typename T>
    class AudioBufferInterleavedInterface : public AudioBufferInterface {
    public:
        AudioBufferInterleavedInterface(int channel_count, int capacity, int sample_size = -1)
                : AudioBufferInterface(channel_count, capacity, sample_size > 0 ? sample_size : sizeof(T)) {}

        const T *getData() const {
            return const_cast<AudioBufferInterleavedInterface *>(this)->getData();
        }

        virtual T *getData() = 0;

        const uint8_t *getDataPtr() const {
            return const_cast<AudioBufferInterleavedInterface *>(this)->getDataPtr();
        }

        uint8_t *getDataPtr() {
            return reinterpret_cast<uint8_t *>(getData());
        }

        std::vector<uint8_t *> getDataFull() override {
            return {getDataPtr()};
        }

        std::vector<const uint8_t *> getDataFullC() const override {
            return {getDataPtr()};
        }

        bool copyData(const AudioBufferInterface *srcr) override {
            auto src = dynamic_cast<const AudioBufferInterleavedInterface<T>*>(srcr);
            if(src == nullptr) return false; // Different buffer types unsupported
            memcpy (this->getData(), src->getData(),
                    static_cast<size_t>(this->getSampleCount() * getChannelCount() * getSampleSize()));
            return true;
        }
    };

    template<typename T>
    class AudioBufferInterleaved : public AudioBufferInterleavedInterface<T>, public AudioBufferModifiableInterface {
    protected:
        std::vector<T> buffer;

    public:
        AudioBufferInterleaved(int channel_count, int capacity, int sample_size = -1)
                : AudioBufferInterleavedInterface<T>(channel_count, capacity, sample_size) {
            int data_per_sample = this->getSampleSize() / sizeof(T);
            buffer.resize((unsigned long) std::max(0, data_per_sample * this->getSampleCount() * channel_count));
        }

        T *getData() override {
            return buffer.data();
        }

        // TODO: remove
        std::vector<T> &getDataContainer() {
            return buffer;
        }

        const std::vector<T> &getDataContainer() const {
            return buffer;
        }

        void reset() override {
            memset(getData(), 0,
                   static_cast<size_t>(this->getSampleCount() * this->getChannelCount() * this->getSampleSize()));
        }

        void clear() override {
            buffer.clear();
        }

        void setChannelCount(int channel_count) override {
            AudioBufferInterface::channel_count = channel_count;
        }

        void setSampleCount(int sample_count) override {
            AudioBufferInterface::capacity = sample_count;
            int data_per_sample = this->getSampleSize() / sizeof(T);
            AudioBufferInterleaved<T>::buffer.resize(
                    static_cast<unsigned long>(this->getSampleCount() * this->getChannelCount() * data_per_sample));
        }
    };
}}
