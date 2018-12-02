//
// Created by egordm on 10-11-2018.
//

#pragma once

#include <string>
#include <utils/debug.h>
#include "ffmpeg_utils.h"
#include "../audio/structures/audio_container_interface.h"

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
}

using namespace litcore::utils;
using namespace litaudiofile;
using namespace litaudio;

#define AudioWrite_TAG "AudioWriter"

namespace litaudiofile {
    class AudioWriter {
    private:
        AVCodecContext *codec_context = nullptr;
        AVIOContext *output_io_context = nullptr;
        AVStream *stream = nullptr;
        AVCodec *codec = nullptr;
        AVFormatContext *format_context = nullptr;
        AVOutputFormat *format = nullptr;
        AVFrame *frame = nullptr;
        AVPacket packet;
        int error;
        bool writing_planar = false;
        int sample_byte_size = 0;
        int max_frame_size = 0;
        int sample_cursor = 0;
        uint8_t *data_cursor = nullptr;

        structures::AudioContainerInterface *src;
        structures::AudioContainerInterface *tmp;
        std::string path;

    public:
        AudioWriter(structures::AudioContainerInterface *src, const std::string &path) : src(src), path(path) {}

        virtual ~AudioWriter() {
            av_frame_free(&frame);
            if (output_io_context != nullptr) avio_closep(&output_io_context);
            avformat_free_context(format_context);
            avcodec_free_context(&codec_context);
        }

        bool open_file();

        bool write_file();

        bool write_frame(bool &finished, AVFrame *frame);

        bool fill_frame_planar(AVFrame *frame);

        bool fill_frame_packed(AVFrame *frame);

        bool pick_sample_format();

        bool write();
    };
}
