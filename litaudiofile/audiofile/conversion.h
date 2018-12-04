//
// Created by egordm on 10-11-2018.
//

#pragma once

#include <algorithm>
#include <utils/debug.h>
#include <structures/audio_container.h>

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

        virtual ~AudioConverter();

        bool convert();
    };
}}