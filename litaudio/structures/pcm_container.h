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
        PCMAudio() : AudioContainer<float>(AV_SAMPLE_FMT_FLT) {}

        PCMAudio(std::vector<float> &data, int sample_count, int sample_rate, int channels)
                : AudioContainer<float>(AV_SAMPLE_FMT_FLT, data, sample_count, sample_rate, channels) {}
    };
}};
