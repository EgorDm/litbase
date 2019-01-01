//
// Created by egordm on 1-1-19.
//

#pragma once

#include <fileref.h>

namespace litaudiofile {
    class TagReader {
    protected:
        TagLib::FileRef file_ref;

    public:
        TagReader(const std::string &file_path) : file_ref(file_path.c_str()) {}

        TagLib::Tag *getTags() {
            return file_ref.tag();
        }

        TagLib::AudioProperties *getProperties() {
            return file_ref.audioProperties();
        }
    };
}
