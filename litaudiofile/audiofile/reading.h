#include <utility>

//
// Created by egordm on 10-11-2018.
//

#pragma once

#include <string>
#include <utils/debug.h>
#include "ffmpeg_utils.h"
#include "conversion.h"
#include "../audio/structures/audio_container.h"

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
}

#define AudioReader_TAG "AudioReader"

using namespace litcore::utils;
using namespace litaudiofile;

namespace litaudiofile {
    template<typename O>
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
        int sample_byte_size = 0;


    private:
        structures::AudioContainer<uint8_t> tmp;
        structures::AudioContainer<O> *dst;
        std::string path;

        bool open_file();

        bool read_file();

        bool read_frame(bool &finished, int &sample_count);

        bool handle_frame_packed();

        bool handle_frame_planar();

    public:
        AudioReader(structures::AudioContainer<O> *dst, std::string path)
                : dst(dst), path(std::move(path)) {}

        virtual ~AudioReader() {
            av_frame_free(&frame);
            avcodec_close(codec_context);
            avcodec_free_context(&codec_context);
            avformat_close_input(&format_context);
        }

        bool read();
    };
}

#include "reading.cpp"
