//
// Created by egordm on 11-11-2018.
//


#include "writing.h"
#include "conversion.h"

using namespace litcore::utils;
using namespace litaudiofile;

bool AudioWriter::write() {
    if(!open_file()) return false;

    return write_file();
}

bool AudioWriter::write_file() {
    if ((error = avformat_write_header(format_context, nullptr)) < 0) {
        ffmpeg_utils::log_error(AudioWrite_TAG, "Could not write output file header.", error);
        return false;
    }

    /* Create a new frame to store the audio samples. */
    if ((frame = av_frame_alloc()) == nullptr) {
        ffmpeg_utils::log_error(AudioWrite_TAG, "Could not allocate output frame");
        return false;
    }

    frame->channel_layout = codec_context->channel_layout;
    frame->format = codec_context->sample_fmt;
    frame->sample_rate = codec_context->sample_rate;

    av_init_packet(&packet);

    writing_planar = static_cast<bool>(av_sample_fmt_is_planar(codec_context->sample_fmt));
    max_frame_size = codec_context->frame_size == 0 ? AV_CODEC_CAP_VARIABLE_FRAME_SIZE : codec_context->frame_size;
    sample_byte_size = av_get_bytes_per_sample(codec_context->sample_fmt);

    if(writing_planar) { // Planar strategy is to copy everything into a temp buffer
        frame->nb_samples = max_frame_size;

        /* Allocate the samples of the created frame. This call will make
         * sure that the audio frame can hold as many samples as specified. */
        if ((error = av_frame_get_buffer(frame, 0)) < 0) {
            ffmpeg_utils::log_error(AudioWrite_TAG, "Could not allocate output frame samples.", error);
            return false;
        }
    }

    // Write frames
    data_cursor = tmp->get_char_data();

    bool finished = false;
    bool success = false;
    while(sample_cursor < tmp->get_sample_count() && !finished) {
        success = write_frame(finished, frame);
        av_packet_unref(&packet);

        if(!success) return false;
    }

    // Flush
    write_frame(finished, nullptr);
    av_packet_unref(&packet);

    frame->nb_samples = max_frame_size;

    if ((error = av_write_trailer(format_context)) < 0) {
        ffmpeg_utils::log_error(AudioWrite_TAG, "Could not write output file trailer.", error);
        return false;
    }

    avio_closep(&format_context->pb);
    output_io_context = nullptr;

    return true;
}

bool AudioWriter::pick_sample_format() {
    // try to pick a compatible format
    if(ffmpeg_utils::supports_sample_fmt(codec, src->format)) {
        codec_context->sample_fmt = src->format;
        tmp = src;
    } else if(ffmpeg_utils::supports_sample_fmt(codec, av_get_planar_sample_fmt(src->format))) {
        codec_context->sample_fmt = av_get_planar_sample_fmt(src->format);
        tmp = src;
    } else {
        codec_context->sample_fmt = codec->sample_fmts[0];

        // Do conversion first
        tmp = new structures::AudioContainer<uint8_t>(av_get_packed_sample_fmt(codec_context->sample_fmt));
        tmp->copy_unfilled_format(src);
        processing::AudioConverter converter(src, tmp);

        if(!converter.convert()) {
            ffmpeg_utils::log_error(AudioWrite_TAG, "Could not convert input data to the writing format.");
            return false;
        }
    }

    return true;
}

bool AudioWriter::open_file() {
    /* Open the output file to write to it. */
    if (avio_open(&output_io_context, path.c_str(), AVIO_FLAG_READ_WRITE) < 0) {
        ffmpeg_utils::log_error(AudioWrite_TAG, "Failed to open output file!");
        return false;
    }

    /* Create a new format context for the output container format. */
    if (!(format_context = avformat_alloc_context())) {
        ffmpeg_utils::log_error(AudioWrite_TAG, "Could not allocate output format context");
        return false;
    }
    format_context->pb = output_io_context;

    /* Guess the desired container format based on the file extension. */
    if ((format = av_guess_format(nullptr, path.c_str(), nullptr)) == nullptr) {
        ffmpeg_utils::log_error(AudioWrite_TAG, "Could not find output file format");
        return false;
    }
    format_context->oformat = format;

    if ((format_context->url = av_strdup(path.c_str())) == nullptr) {
        ffmpeg_utils::log_error(AudioWrite_TAG, "Could not allocate url.");
        return false;
    }

    /* Find the encoder to be used by its name. */
    if ((codec = avcodec_find_encoder(format->audio_codec)) == nullptr) {
        ffmpeg_utils::log_error(AudioWrite_TAG, "Couldn't find a proper encoder.");
        return false;
    }

    /* Create a new audio stream in the output file container. */
    if ((stream = avformat_new_stream(format_context, nullptr)) == nullptr) {
        ffmpeg_utils::log_error(AudioWrite_TAG, "Could not create new stream");
        return false;
    }

    codec_context = avcodec_alloc_context3(codec);
    if (codec_context == nullptr) {
        ffmpeg_utils::log_error(AudioWrite_TAG, "Could not allocate an encoding context");
        return false;
    }

    /* Set the basic encoder parameters.
    * The input file's sample rate is used to avoid a sample rate conversion. */
    codec_context->channels = src->channels;
    codec_context->channel_layout = static_cast<uint64_t>(av_get_default_channel_layout(src->channels));
    codec_context->sample_rate = src->sample_rate;
    if(!pick_sample_format()) return false;

    /* Set the sample rate for the container. */
    stream->time_base.den = src->sample_rate;
    stream->time_base.num = 1;

    if (format->flags & AVFMT_GLOBALHEADER) codec_context->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;

    /* Open the encoder for the audio stream to use it later. */
    if ((error = avcodec_open2(codec_context, codec, nullptr)) < 0) {
        ffmpeg_utils::log_error(AudioWrite_TAG, "Could not open output codec.", error);
        return false;
    }

    if (avcodec_parameters_from_context(stream->codecpar, codec_context) < 0) {
        ffmpeg_utils::log_error(AudioWrite_TAG, "Could not initialize stream parameters.", error);
        return false;
    }

#ifdef DEBUG
    av_dump_format(format_context, 0, path.c_str(), 1);
#endif

    return true;
}

bool AudioWriter::write_frame(bool &finished, AVFrame *frame) {
    // Fill the frame with sample data
    if(frame != nullptr) {
        frame->pts = sample_cursor;
        frame->nb_samples = std::min(tmp->get_sample_count() - sample_cursor, max_frame_size);

        if(writing_planar) fill_frame_planar(frame);
        else fill_frame_packed(frame);
    }

    /* Send the audio frame stored in the temporary packet to the encoder.
     * The output audio stream encoder is used to do this. */
    if ((error = avcodec_send_frame(codec_context, frame)) < 0) {
        if(error == AVERROR_EOF) {
            error = 0;
            finished = true;
            return true;
        }

        ffmpeg_utils::log_error(AudioWrite_TAG, "Could not send packet for encoding.", error);
        return false;
    }

    /* Receive one encoded frame from the encoder. */
    if ((error = avcodec_receive_packet(codec_context, &packet)) < 0) {
        if(error == AVERROR(EAGAIN)) {
            error = 0;
            return true;
        } else if(error == AVERROR_EOF) {
            error = 0;
            finished = true;
            return true;
        }

        ffmpeg_utils::log_error(AudioWrite_TAG, "Could not encode frame.", error);
        return false;
    }

    /* Write one audio frame from the temporary packet to the output file. */
    if ((error = av_write_frame(format_context, &packet)) < 0) {
        ffmpeg_utils::log_error(AudioWrite_TAG, "Could not write frame.", error);
        return false;
    }

    return true;
}

bool AudioWriter::fill_frame_planar(AVFrame *frame) {
    int next_sample_cursor = sample_cursor + frame->nb_samples;
    int planar_cursor = 0;
    for(; sample_cursor < next_sample_cursor; sample_cursor++) {
        for (int c = 0; c < codec_context->channels; ++c) {
            std::copy(data_cursor, data_cursor + sample_byte_size, frame->data[c] + planar_cursor);
            data_cursor += sample_byte_size;
        }

        planar_cursor += sample_byte_size;
    }

    return true;
}

bool AudioWriter::fill_frame_packed(AVFrame *frame) {
    frame->data[0] = data_cursor;
    data_cursor = data_cursor + frame->nb_samples * sample_byte_size * codec_context->channels;
    sample_cursor += frame->nb_samples;
    return true;
}
