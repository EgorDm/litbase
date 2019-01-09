//
// Created by egordm on 9-1-19.
//

#pragma once

extern "C" {
#include <libavutil/samplefmt.h>
}

namespace litaudio { namespace structures {
    class AudioContainerInterface {
    protected:
        AVSampleFormat format;
        int sample_rate;

    public:
        explicit AudioContainerInterface(AVSampleFormat format, int sample_rate)
                : format(format), sample_rate(sample_rate) {}

        int getSampleRate() const {
            return sample_rate;
        }

        void setSampleRate(int sample_rate) {
            AudioContainerInterface::sample_rate = sample_rate;
        }

        AVSampleFormat getFormat() const {
            return format;
        }

        void setFormat(AVSampleFormat format) {
            AudioContainerInterface::format = format;
            setSampleSize(av_get_bytes_per_sample(format));
        }

        int getChannelCount() const {
            return getBuffer()->getChannelCount();
        }

        void setChannelCount(int channel_count) {
            getModifiableBuffer()->setChannelCount(channel_count);
        }

        int getSampleCount() const {
            return getBuffer()->getSampleCount();
        }

        void setSampleCount(int sample_count) {
            getModifiableBuffer()->setSampleCount(sample_count);
        }

        void setSampleSize(int sample_size) {
            getBuffer()->setSampleSize(sample_size);
        }

        int getSampleSize() const {
            return getBuffer()->getSampleSize();
        }

        virtual AudioBufferInterface *getBuffer() = 0;

        const AudioBufferInterface *getBuffer() const {
            return const_cast<AudioContainerInterface *>(this)->getBuffer();
        }

        virtual AudioBufferModifiableInterface *getModifiableBuffer() = 0;

        const AudioBufferModifiableInterface *getModifiableBuffer() const {
            return const_cast<AudioContainerInterface *>(this)->getModifiableBuffer();
        }

        bool isSameFormat(const AudioContainerInterface *other) const {
            return sample_rate == other->getSampleRate() && getChannelCount() == other->getChannelCount() &&
                   format == other->getFormat();
        }

        virtual void copyFormat(const AudioContainerInterface *src) {
            if (getFormat() == AV_SAMPLE_FMT_NONE) setFormat(src->getFormat());
            if (getSampleRate() < 0) setSampleRate(src->getSampleRate());
            if (getChannelCount() < 0) setChannelCount(src->getChannelCount());
        }
    };
}}