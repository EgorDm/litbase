//
// Created by egordm on 17-11-2018.
//

#pragma once

#include <vector>
#include "typed_audio_container_interface.h"

extern "C" {
#include <libavformat/avformat.h>
}

namespace litaudio { namespace structures {
    template<typename T>
    class AudioContainer : public TypedAudioContainerInterface<T> {
    private:
        using parent_type = TypedAudioContainerInterface<T>;

    public:
        std::vector<T> data;
        int sample_count;

        AudioContainer() : sample_count(0) {}

        explicit AudioContainer(const AVSampleFormat &format)
                : sample_count(0), TypedAudioContainerInterface<T>(format) {}

        AudioContainer(AVSampleFormat format, std::vector<T> data, int sample_count, int sample_rate, int channels)
                : TypedAudioContainerInterface<T>(format, sample_rate, channels), data(std::move(data)),
                  sample_count(sample_count) {}

        inline const T *get_data() const override {
            return &data[0];
        }

        inline const uint8_t *get_char_data() const override {
            return reinterpret_cast<const uint8_t *>(&data[0]);
        }

        int get_sample_count() const override {
            return sample_count;
        }

        void set_sample_count(int sample_count) override {
            if (this->sample_count < sample_count) {

                int data_per_sample = parent_type::sample_byte_size() / sizeof(T);
                this->data.resize(static_cast<unsigned long long int>(sample_count * parent_type::channels * data_per_sample));
            }
            this->sample_count = sample_count;
        }

        template<typename O>
        inline AudioContainer<O> *clone() const {
            auto ret = new AudioContainer<O>(parent_type::format);
            ret->copy_unfilled_format(this);
            return ret;
        }
    };
}}
