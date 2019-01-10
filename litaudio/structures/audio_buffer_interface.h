//
// Created by egordm on 7-1-19.
//

#pragma once

#include <cstdint>
#include <vector>

extern "C" {
#include <libavutil/samplefmt.h>
}

namespace litaudio { namespace structures {
    class AudioBufferInterface {
    protected:
        int channel_count, capacity;
        AVSampleFormat format;
        int sample_size;

    public:
        AudioBufferInterface(int channel_count, int capacity, AVSampleFormat format)
                : channel_count(channel_count), capacity(capacity), format(format),
                  sample_size(av_get_bytes_per_sample(format)) {}

        int getChannelCount() const {
            return channel_count;
        }

        int getSampleCount() const {
            return capacity;
        }

        void setFormat(AVSampleFormat format) {
            AudioBufferInterface::format = format;
            sample_size = av_get_bytes_per_sample(format);
        }

        AVSampleFormat getFormat() const {
            return format;
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