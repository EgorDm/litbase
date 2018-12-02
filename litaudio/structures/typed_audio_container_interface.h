//
// Created by egordm on 2-12-18.
//

#pragma once

#include "audio_container_interface.h"

namespace litaudio { namespace structures {
    template<typename T>
    class TypedAudioContainerInterface : public AudioContainerInterface {
    public:
        TypedAudioContainerInterface() : AudioContainerInterface() {}

        explicit TypedAudioContainerInterface(const AVSampleFormat &format) : AudioContainerInterface(format) {}

        TypedAudioContainerInterface(AVSampleFormat format, int sample_rate, int channels)
                : AudioContainerInterface(format, sample_rate, channels) {}

        virtual const T *get_data() const = 0;

        virtual T *get_data() {
            return const_cast<T *>(static_cast<const TypedAudioContainerInterface<T> *>(this)->get_data());
        };
    };
}}

