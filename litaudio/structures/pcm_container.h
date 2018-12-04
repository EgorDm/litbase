//
// Created by egordm on 17-11-2018.
//

#pragma once

extern "C" {
#include <libavformat/avformat.h>
}

#include "audio_container.h"

namespace litaudio { namespace structures {
    struct PCMAudio : public AudioContainer<float> {
        explicit PCMAudio(int channels = -1) : AudioContainer<float>(AV_SAMPLE_FMT_FLT, channels) {}

        PCMAudio(const AudioContainer<float>::DataContainer &data, int sample_count, int sample_rate, int channels)
                : AudioContainer<float>(AV_SAMPLE_FMT_FLT, data, sample_count, sample_rate, channels) {}
    };
}};
