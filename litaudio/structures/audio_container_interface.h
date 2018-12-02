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

        AudioContainerInterface() : format(AV_SAMPLE_FMT_NONE), sample_rate(-1), channels(-1) {}

        explicit AudioContainerInterface(const AVSampleFormat &format)
                : format(format), sample_rate(-1), channels(-1) {}

        AudioContainerInterface(AVSampleFormat format, int sample_rate, int channels)
                : format(format), sample_rate(sample_rate), channels(channels) {}

        virtual ~AudioContainerInterface() = default;

        int sample_byte_size() const;

        bool same_format(const AudioContainerInterface *other) const;

        virtual const uint8_t *get_char_data() const = 0;

        uint8_t *get_char_data();

        virtual int get_sample_count() const = 0;

        virtual void set_sample_count(int sample_count) = 0;

        virtual void copy_unfilled_format(const AudioContainerInterface *src);
    };
}}
