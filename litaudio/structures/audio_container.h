//
// Created by egordm on 7-1-19.
//

#pragma once

#include "audio_buffer_interface.h"
#include "audio_buffer_interface_deinterleaved.h"
#include "audio_buffer_interface_interleaved.h"
#include "audio_container_interface.h"


namespace litaudio {
    namespace structures {
        template<typename B, typename std::enable_if<std::is_base_of<AudioBufferInterface, B>::value>::type* = nullptr>
        class AudioContainer : public AudioContainerInterface {
        protected:
            B *buffer;

        public:
            explicit AudioContainer(B *buffer, int sample_rate = -1)
                    : AudioContainerInterface(sample_rate), buffer(buffer) {}

            virtual ~AudioContainer() {
                delete buffer;
            }

            B *getTypedBuffer() {
                return buffer;
            }

            AudioBufferInterface *getBuffer() override {
                return buffer;
            }

            AudioBufferModifiableInterface *getModifiableBuffer() override {
                return dynamic_cast<AudioBufferModifiableInterface *>(buffer);
            }

            bool isModifyable() {
                return getModifiableBuffer() != nullptr;
            }
        };

        template<class Buffer>
        class AudioContainerSimple : public AudioContainer<Buffer> {
        public:
            explicit AudioContainerSimple(int channel_count = -1, int sample_rate = -1, int capacity = 0)
                    : AudioContainer<Buffer>(new Buffer(channel_count, capacity), sample_rate) {}
        };

        template<typename T>
        using AudioContainerInterleaved = AudioContainerSimple<AudioBufferInterleaved<T>>;

        template<typename T>
        using AudioContainerDeinterleaved = AudioContainerSimple<AudioBufferDeinterleaved<T>>;

        using AbstractAudioContainer = AudioContainer<AudioBufferInterface>;
    }
}