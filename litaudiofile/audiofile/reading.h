#include <utility>

//
// Created by egordm on 10-11-2018.
//

#pragma once

#include <string>
#include <utils/debug.h>
#include "ffmpeg_utils.h"
#include "conversion.h"

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
}

#define AudioReader_TAG "AudioReader"

using namespace litcore;
using namespace litaudiofile;

namespace litaudiofile {
    class AudioReader {
    private:
        AVFormatContext *format_context = nullptr;
        AVStream *stream = nullptr;
        AVCodecParameters *codec_params = nullptr;
        AVCodec *codec = nullptr;
        AVFrame *frame = nullptr;
        AVPacket packet;
        AVCodecContext *codec_context = nullptr;
        int error = 0;
        bool reading_planar = false;

    private:
        structures::AudioContainerInterface *tmp = nullptr;
        structures::AudioContainerInterface *dst;
        std::string path;

        bool open_file();

        bool read_file();

        bool read_frame(bool &finished, int &sample_count);

        bool handle_frame_packed();

        bool handle_frame_planar();

    public:
        AudioReader(structures::AudioContainerInterface *dst, std::string path) : dst(dst), path(std::move(path)) {}

        virtual ~AudioReader() {
            av_frame_free(&frame);
            avcodec_close(codec_context);
            avcodec_free_context(&codec_context);
            avformat_close_input(&format_context);
        }

        bool read();
    };
}
