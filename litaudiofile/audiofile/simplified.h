//
// Created by egordm on 2-12-18.
//

#pragma once

#include <string>
#include <structures/audio_container.h>
#include <structures/pcm_container.h>
#include "reading.h"

#define LitAudioSimplified_TAG "LitAudio Simplified"

using namespace litaudio::structures;
using namespace litaudiofile;

namespace litaudiofile { namespace simplified {
    template <typename T>
    inline AudioContainer<AudioBufferDeinterleaved<T>> *read_audio(const std::string &path, const AVSampleFormat &format, int sample_rate = -1) {
        auto ret = new AudioContainerDeinterleaved<T>(format);
        ret->sample_rate = sample_rate;

        auto reader = AudioReader(ret, path);
        if (!reader.read()) {
            delete ret;
            debug::log_error(LitAudioSimplified_TAG, "Failed to read audio file: " + path);
            return nullptr;
        }

        return ret;
    }

    PCMAudio *read_audio_pcm(const std::string &path, int sample_rate = -1);

    bool write_audio(const std::string &path, AudioContainerInterface *audio);

}}


