//
// Created by egordm on 10-11-2018.
//

#pragma once

#include <algorithm>
#include <utils/debug.h>
#include "../audio/structures/audio_container_interface.h"

extern "C" {
#include <libavutil/samplefmt.h>
#include <libswresample/swresample.h>
}

using namespace litcore::utils;
using namespace litaudio;

#define AudioConverter_TAG "AudioConverter"

namespace litaudiofile { namespace processing {
    class AudioConverter {
    private:
        SwrContext *context = nullptr;
        const structures::AudioContainerInterface *src;
        structures::AudioContainerInterface *dst;

    public:
        AudioConverter(const structures::AudioContainerInterface *src, structures::AudioContainerInterface *dst)
                : src(src), dst(dst) {}

        virtual ~AudioConverter() {
            swr_free(&context);
        }

        bool convert() {
            // Check if format is already correct. Then just copy the data
            if(src->same_format(dst)) {
                dst->set_sample_count(src->get_sample_count());
                auto src_data = src->get_char_data();
                auto dst_data = dst->get_char_data();
                int copy_byte_count = src->get_sample_count() * src->sample_byte_size() * src->channels;
                std::copy (src_data, src_data + copy_byte_count, dst_data); // TODO: use memcpy
                return true;
            }

            // Create context
            auto src_channel_format = av_get_default_channel_layout(src->channels);
            auto dst_channel_format = av_get_default_channel_layout(dst->channels);

            context = swr_alloc_set_opts(nullptr,
                                         dst_channel_format, dst->format, dst->sample_rate,
                                         src_channel_format, src->format, src->sample_rate, 0, nullptr);

            if (context == nullptr) {
                debug::log_error(AudioConverter_TAG, "Couldn't create the SwrContext");
                return false;
            }

            if (swr_init(context) != 0) {
                debug::log_error(AudioConverter_TAG, "Couldn't initialize the SwrContext");
                return false;
            }

            // Set pcm infomation
            dst->set_sample_count(swr_get_out_samples(context, src->get_sample_count()));

            auto dst_data = dst->get_char_data();
            auto src_data = src->get_char_data();

            // Resample and set pcm data
            if (swr_convert(context, &dst_data, dst->get_sample_count(), &src_data, src->get_sample_count()) < 0) {
                debug::log_error(AudioConverter_TAG, "Error while converting");
                return false;
            }

            return true;
        }
    };
}}