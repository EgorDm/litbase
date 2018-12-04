//
// Created by egordm on 2-12-18.
//

#include "audio_container_interface.h"

using namespace litaudio::structures;

int AudioContainerInterface::getSampleByteSize() const {
    return av_get_bytes_per_sample(format);
}

bool AudioContainerInterface::isSameFormat(const AudioContainerInterface *other) const {
    return sample_rate == other->sample_rate && channels == other->channels && format == other->format;
}

const uint8_t *AudioContainerInterface::getByteData() const {
    return getByteData(0);
}

uint8_t *AudioContainerInterface::getByteData(int channel) {
    return const_cast<uint8_t *>(static_cast<const AudioContainerInterface *>(this)->getByteData(channel));
}

bool AudioContainerInterface::isPlanar() const {
    return static_cast<bool>(av_sample_fmt_is_planar(format));
}

void AudioContainerInterface::copyUnfilledFormat(const AudioContainerInterface *src) {
    if (format == AV_SAMPLE_FMT_NONE) format = src->format;
    if (sample_rate < 0) sample_rate = src->sample_rate;
    if (channels < 0) channels = src->channels;
}

AVSampleFormat AudioContainerInterface::getFormat() const {
    return format;
}

void AudioContainerInterface::setFormat(AVSampleFormat format) {
    AudioContainerInterface::format = format;
}

int AudioContainerInterface::getSampleRate() const {
    return sample_rate;
}

void AudioContainerInterface::setSampleRate(int sample_rate) {
    AudioContainerInterface::sample_rate = sample_rate;
}

int AudioContainerInterface::getChannelCount() const {
    return channels;
}

int AudioContainerInterface::getChannelDimCount() const {
    return isPlanar() ? getChannelCount() : 1;
}

std::vector<uint8_t *> AudioContainerInterface::getByteChannelData() {
    std::vector<uint8_t *> ret;
    for (int i = 0; i < getChannelDimCount(); ++i) ret.push_back(getByteData(i));
    return ret;
}

const std::vector<const uint8_t *> AudioContainerInterface::getByteChannelData() const {
    std::vector<const uint8_t *> ret;
    for (int i = 0; i < getChannelDimCount(); ++i) ret.push_back(getByteData(i));
    return ret;
}
