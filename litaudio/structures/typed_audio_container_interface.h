//
// Created by egordm on 2-12-18.
//

#pragma once

#include "audio_container_interface.h"

namespace litaudio { namespace structures {
    template<typename T>
    class TypedAudioContainerInterface : public AudioContainerInterface {
    public:
        explicit TypedAudioContainerInterface(AVSampleFormat format = AV_SAMPLE_FMT_NONE, int channels = -1, int sample_rate = -1)
                : AudioContainerInterface(format, channels, sample_rate) {}

        virtual const T *getData() {
            return getData(0);
        }

        virtual const T *getData(int channel) const = 0;

        T *getData(int channel = 0) {
            return const_cast<T *>(static_cast<const TypedAudioContainerInterface<T> *>(this)->getData(channel));
        };
    };
}}

