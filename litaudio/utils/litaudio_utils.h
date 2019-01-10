//
// Created by egordm on 10-1-19.
//

#pragma once

extern "C" {
#include <libavutil/samplefmt.h>
#include <libavutil/common.h>
}

namespace litaudio { namespace utils {

    template<typename T>
    inline AVSampleFormat deduce_sample_format() { return AV_SAMPLE_FMT_NONE; }

    template<>
    inline AVSampleFormat deduce_sample_format<int16_t>() { return AV_SAMPLE_FMT_S16; }

    template<>
    inline AVSampleFormat deduce_sample_format<int32_t>() { return AV_SAMPLE_FMT_S32; }

    template<>
    inline AVSampleFormat deduce_sample_format<float>() { return AV_SAMPLE_FMT_FLT; }

    template<>
    inline AVSampleFormat deduce_sample_format<double>() { return AV_SAMPLE_FMT_DBL; }

    template<typename T>
    inline AVSampleFormat deduce_format(bool planar) {
        return planar ? av_get_planar_sample_fmt(deduce_sample_format<T>()) : deduce_sample_format<T>();
    }
}}
