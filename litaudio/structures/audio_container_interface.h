//
// Created by egordm on 2-12-18.
//

#pragma once

extern "C" {
#include <libavutil/samplefmt.h>
}

namespace litaudio { namespace structures {
    class AudioContainerInterface {
    public:
        AVSampleFormat format;
        int sample_rate;
        int channels;

        AudioContainerInterface()
                : format(AV_SAMPLE_FMT_NONE), sample_rate(-1), channels(-1) {}

        explicit AudioContainerInterface(const AVSampleFormat &format)
                : format(format), sample_rate(-1), channels(-1) {}

        AudioContainerInterface(AVSampleFormat format, int sample_rate, int channels)
                : format(format), sample_rate(sample_rate), channels(channels) {}

        virtual ~AudioContainerInterface() = default;

        inline int sample_byte_size() const {
            return av_get_bytes_per_sample(format);
        }

        inline bool same_format(const AudioContainerInterface *other) const {
            return sample_rate == other->sample_rate && channels == other->channels && format == other->format;
        }

        virtual uint8_t *get_char_data() = 0;

        virtual const uint8_t *get_char_data() const = 0;

        virtual int get_sample_count() const = 0;

        virtual void set_sample_count(int sample_count) = 0;

        virtual void copy_unfilled_format(const AudioContainerInterface *src) {
            if (format == AV_SAMPLE_FMT_NONE) format = src->format;
            if (sample_rate < 0) sample_rate = src->sample_rate;
            if (channels < 0) channels = src->channels;
        }
    };
}}
