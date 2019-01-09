//
// Created by egordm on 7-1-19.
//

#pragma once

#include "audio_buffer_interface.h"
#include "audio_buffer_interface_deinterleaved.h"
#include "audio_container_interface.h"


namespace litaudio { namespace structures {
    template<typename B>
    class AudioContainer : public AudioContainerInterface {
    protected:
        B *buffer;

    public:
        explicit AudioContainer(B *buffer, AVSampleFormat format = AV_SAMPLE_FMT_NONE, int sample_rate = -1)
                : AudioContainerInterface(format, sample_rate), buffer(buffer) {}

        virtual ~AudioContainer() {
            delete buffer;
        }

        AudioBufferInterface *getBuffer() override {
            return buffer;
        }

        AudioBufferModifiableInterface *getModifiableBuffer() override {
            return dynamic_cast<AudioBufferModifiableInterface *>(buffer);
        }
    };

    template<typename B>
    inline AudioContainer<B> *createAudioContainer(AVSampleFormat format = AV_SAMPLE_FMT_NONE, int channel_count = -1,
                                           int sample_rate = -1, int capacity = 0) {
        return new AudioContainer<B>(new B(channel_count, capacity, av_get_bytes_per_sample(format)), format, sample_rate);
    }

    template<typename B>
    inline AudioContainer<B> createAudioContainerI(AVSampleFormat format = AV_SAMPLE_FMT_NONE, int channel_count = -1,
                                            int sample_rate = -1, int capacity = 0) {
        return AudioContainer<B>(new B(channel_count, capacity, av_get_bytes_per_sample(format)), format, sample_rate);
    }

    using AbstractAudioContainer = AudioContainer<AudioBufferInterface>;
}}