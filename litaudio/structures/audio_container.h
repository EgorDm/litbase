//
// Created by egordm on 17-11-2018.
//

#pragma once

#include <vector>
#include <memory>
#include "typed_audio_container_interface.h"

extern "C" {
#include <libavformat/avformat.h>
}

namespace litaudio { namespace structures {
    template<typename T>
    class AudioContainer : public TypedAudioContainerInterface<T> {
    public:
        using DataContainer = std::vector<std::vector<T>>;
    private:
        using parent_type = TypedAudioContainerInterface<T>;

        DataContainer data;
        int sample_count = 0;

    public:
        explicit AudioContainer(AVSampleFormat format = AV_SAMPLE_FMT_NONE, int channels = -1, int sample_rate = -1)
                : TypedAudioContainerInterface<T>(format, channels, sample_rate) {
            clear();
        }

        AudioContainer(AVSampleFormat format, const DataContainer &data, int sample_count, int sample_rate, int channels)
                : TypedAudioContainerInterface<T>(format, channels, sample_rate),
                  data(data),
                  sample_count(sample_count) {}

        void clear() override {
            AudioContainerInterface::clear();
            if(parent_type::getChannelCount() <= 0) return;
            data.clear();
            data.resize(parent_type::getChannelDimCount());
        }

        std::vector<T> &getDataContainer(int channel = 0) {
            return data[channel];
        }

        const T *getData(int channel) const override {
            return data[channel].data();
        }

        const uint8_t *getByteData(int channel) const override {
            return reinterpret_cast<const uint8_t *>(getData(channel));
        }

        int getSampleCount() const override {
            return sample_count;
        }

        void setSampleCount(int sample_count) override {
            // Resize arrays accordingly
            if (this->sample_count < sample_count) {
                int data_per_sample = parent_type::getSampleByteSize() / sizeof(T);
                if(!parent_type::isPlanar()) data_per_sample *= parent_type::getChannelCount();

                for (int i = 0; i < parent_type::getChannelDimCount(); ++i) {
                    data[i].resize(static_cast<unsigned long>(sample_count * data_per_sample));
                }
            }
            this->sample_count = sample_count;
        }

        template<typename O>
        inline AudioContainer<O> *clone() const {
            auto ret = new AudioContainer<O>(parent_type::format);
            ret->copyUnfilledFormat(this);
            ret->clear();
            return ret;
        }
    };
}}
