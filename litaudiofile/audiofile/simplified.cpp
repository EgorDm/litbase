//
// Created by egordm on 2-12-18.
//

#include "simplified.h"
#include "writing.h"

using namespace litaudiofile;
using namespace litaudio;

PCMAudio *simplified::read_audio_pcm(const std::string &path, int sample_rate)  {
    auto ret = new PCMAudio();
    ret->setSampleRate(sample_rate);

    AudioReader reader(ret, path);
    if (!reader.read()) {
        delete ret;
        debug::log_error(LitAudioSimplified_TAG, "Failed to read audio file: " + path);
        return nullptr;
    }

    return ret;
}

bool simplified::write_audio(const std::string &path, AudioContainerInterface *audio) {
    auto writer = AudioWriter(audio, "dst.mp3");
    return writer.write();
}
