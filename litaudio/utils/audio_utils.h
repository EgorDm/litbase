//
// Created by egordm on 29-1-19.
//

#pragma once

#include <cmath>

namespace litaudio { namespace audio_utils {
    inline float dbToAmplitude(float db) {
        return powf(10, db / 20);
    }

    inline float amplitudeToDb(float amplitude) {
        return 6.02f * log2f(amplitude);
    }
}}
