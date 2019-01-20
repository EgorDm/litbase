//
// Created by egordm on 18-1-19.
//

#pragma once

#include <structures/audio_buffer_interface_deinterleaved.h>

using namespace litaudio::structures;

namespace litaudio { namespace utils {

    template<typename T>
    inline void add_buffers(AudioBufferDeinterleavedInterface<T> *dst, const AudioBufferDeinterleavedInterface<T> *src,
                            int offset_dst, int offset_src, int count) {
        const T *src_cursor;
        T *dst_cursor, *max_dst_cursor;
        for (int c = 0; c < dst->getChannelCount(); ++c) {
            src_cursor = src->getChannel(c) + offset_src;
            dst_cursor = dst->getChannel(c) + offset_dst;
            max_dst_cursor = dst_cursor + count;
            while (dst_cursor < max_dst_cursor) {
                *dst_cursor++ += *src_cursor++;
            }
        }
    }

    template<typename T>
    inline void mult_buffer(AudioBufferDeinterleavedInterface<T> *buffer, int offset, int count, T mult) {
        T *cursor, *max_cursor;
        for (int c = 0; c < buffer->getChannelCount(); ++c) {
            cursor = buffer->getChannel(c) + offset;
            max_cursor = cursor + count;
            while (cursor < max_cursor) *cursor++ *= mult;
        }
    }
}}
