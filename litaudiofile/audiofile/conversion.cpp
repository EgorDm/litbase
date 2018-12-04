//
// Created by egordm on 4-12-18.
//

#include <cstring>
#include "conversion.h"

using namespace litaudiofile::processing;

AudioConverter::~AudioConverter() {
    swr_free(&context);
}

bool AudioConverter::convert() {
    // Check if format is already correct. Then just copy the data
    if(src->isSameFormat(dst)) {
        dst->setSampleCount(src->getSampleCount());
        if(src->isPlanar()) {
            int copy_byte_count = src->getSampleCount() * src->getSampleByteSize() * src->getChannelCount();
            std::memcpy (dst->getByteData(), src->getByteData(), copy_byte_count);
        } else {
            for (int c = 0; c < src->getChannelCount(); ++c) {
                int copy_byte_count = src->getSampleCount() * src->getSampleByteSize();
                std::memcpy (dst->getByteData(c), src->getByteData(c), copy_byte_count);
            }
        }

        return true;
    }

    // Create context
    auto src_channel_format = av_get_default_channel_layout(src->getChannelCount());
    auto dst_channel_format = av_get_default_channel_layout(dst->getChannelCount());

    context = swr_alloc_set_opts(nullptr,
                                 dst_channel_format, dst->getFormat(), dst->getSampleRate(),
                                 src_channel_format, src->getFormat(), src->getSampleRate(), 0, nullptr);

    if (context == nullptr) {
        debug::log_error(AudioConverter_TAG, "Couldn't create the SwrContext");
        return false;
    }

    if (swr_init(context) != 0) {
        debug::log_error(AudioConverter_TAG, "Couldn't initialize the SwrContext");
        return false;
    }

    // Set pcm infomation
    dst->setSampleCount(swr_get_out_samples(context, src->getSampleCount()));

    auto dst_data = dst->getByteChannelData();
    auto src_data = src->getByteChannelData();

    // Resample and set pcm data
    if (swr_convert(context, &dst_data.data()[0], dst->getSampleCount(), &src_data.data()[0], src->getSampleCount()) < 0) {
        debug::log_error(AudioConverter_TAG, "Error while converting");
        return false;
    }

    return true;
}
