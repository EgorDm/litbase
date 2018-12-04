//
// Created by egordm on 2-12-18.
//

#pragma once

#include <memory>
#include <vector>

extern "C" {
#include <libavutil/samplefmt.h>
}

namespace litaudio { namespace structures {
    class AudioContainerInterface {
    protected:
        AVSampleFormat format;
        int sample_rate;
        int channels;

    public:
        explicit AudioContainerInterface(AVSampleFormat format = AV_SAMPLE_FMT_NONE, int channels = -1, int sample_rate = -1)
                : format(format), sample_rate(sample_rate), channels(channels) {}

        virtual ~AudioContainerInterface() = default;

        const uint8_t *getByteData() const;

        uint8_t *getByteData(int channel = 0);

        virtual const uint8_t *getByteData(int channel) const = 0;

        std::vector<uint8_t *> getByteChannelData();

        const std::vector<const uint8_t *> getByteChannelData() const;

        int getSampleByteSize() const;

        virtual int getSampleCount() const = 0;

        virtual void setSampleCount(int sample_count) = 0;

        AVSampleFormat getFormat() const;

        void setFormat(AVSampleFormat format);

        int getSampleRate() const;

        void setSampleRate(int sample_rate);

        int getChannelCount() const;

        int getChannelDimCount() const;

        bool isSameFormat(const AudioContainerInterface *other) const;

        bool isPlanar() const;

        virtual void copyUnfilledFormat(const AudioContainerInterface *src);
    };
}}
