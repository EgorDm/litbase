//
// Created by egordm on 8-1-19.
//

#pragma once

#include "audio_container.h"

namespace litaudio { namespace structures {
    class PCMAudio : public AudioContainer<AudioBufferDeinterleaved < float>> {
        public:
        explicit PCMAudio(int channel_count = -1, int sample_rate = -1, int capacity = 0)
                : AudioContainer(new AudioBufferDeinterleaved<float>(channel_count, capacity), sample_rate) {}
    };
}}