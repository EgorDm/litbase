//
// Created by egordm on 2-12-18.
//

#include "audio_container_interface.h"

using namespace litaudio::structures;

int AudioContainerInterface::sample_byte_size() const {
    return av_get_bytes_per_sample(format);
}

bool AudioContainerInterface::same_format(const AudioContainerInterface *other) const {
    return sample_rate == other->sample_rate && channels == other->channels && format == other->format;
}

uint8_t *AudioContainerInterface::get_char_data() {
    return const_cast<uint8_t *>(static_cast<const AudioContainerInterface *>(this)->get_char_data());
}

void AudioContainerInterface::copy_unfilled_format(const AudioContainerInterface *src) {
    if (format == AV_SAMPLE_FMT_NONE) format = src->format;
    if (sample_rate < 0) sample_rate = src->sample_rate;
    if (channels < 0) channels = src->channels;
}
