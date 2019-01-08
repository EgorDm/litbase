//
// Created by egordm on 7-1-19.
//

#pragma once

#include <cstdint>
#include <vector>

namespace litaudio { namespace structures {
    class AudioBufferInterface {
    protected:
        int channel_count, capacity;
        int sample_size;

    public:
        AudioBufferInterface(int channel_count, int capacity, int sample_size)
                : channel_count(channel_count), capacity(capacity), sample_size(sample_size) {}

        int getChannelCount() const {
            return channel_count;
        }

        int getSampleCount() const {
            return capacity;
        }

        void setSampleSize(int sample_size) {
            AudioBufferInterface::sample_size = sample_size;
        }

        int getSampleSize() const {
            return sample_size;
        }

        virtual void reset() = 0;

        virtual std::vector<uint8_t *> getDataFull() = 0;

        virtual std::vector<const uint8_t *> getDataFullC() const = 0;

        virtual bool copyData(const AudioBufferInterface *src) = 0;
    };

    class AudioBufferModifiableInterface {
    public:
        virtual void setChannelCount(int channel_count) = 0;

        virtual void setSampleCount(int sample_count) = 0;

        virtual void clear() = 0;
    };

}}