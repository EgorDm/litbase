//
// Created by egordm on 10-11-2018.
//

#pragma once

#include <string>
#include <utils/debug.h>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
}

#undef av_err2str
#define av_err2str(errnum) \
    av_make_error_string((char*)__builtin_alloca(AV_ERROR_MAX_STRING_SIZE), AV_ERROR_MAX_STRING_SIZE, errnum)

using namespace litcore;

namespace litaudiofile { namespace ffmpeg_utils {
    inline bool supports_sample_fmt(AVCodec *codec, enum AVSampleFormat sample_fmt) {
        const enum AVSampleFormat *p = codec->sample_fmts;

        while (*p != AV_SAMPLE_FMT_NONE) {
            if (*p == sample_fmt) return true;
            p++;
        }

        return false;
    }

    inline void log_error(const std::string &tag, const std::string &message, int error = 0) {
#ifdef DEBUG
        if(error == 0) {
            debug::log_error(tag, message);
        } else {
            debug::log_error(tag, message + " | AVERR: " + av_err2str(error));
        }
#endif
    }
}}