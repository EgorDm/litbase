//
// Created by egordm on 29-12-18.
//

#pragma once

#include <memory>
#include <vector>
#include <cstring>

namespace litaudio { namespace structures {
    template <typename T>
    class AudioBufferDeinterleaved {
    protected:
        std::vector<std::unique_ptr<T[]>> buffers;
        int capacity = 0;
        int channel_count = 0;

    public:
        AudioBufferDeinterleaved(int capacity, int channel_count) : capacity(capacity), channel_count(channel_count) {
            for (int i = 0; i < channel_count; ++i) buffers.emplace_back(new T[capacity]);
        }

        const T* getChannel(int channel = 0) const {
            return buffers[channel].get();
        }

        T *getChannel(int channel = 0) {
            return const_cast<T *>(static_cast<const AudioBufferDeinterleaved *>(this)->getChannel(channel));
        };

        int getChannelCount() const {
            return channel_count;
        }

        void reset() {
            for (int i = 0; i < channel_count; ++i) memset(getChannel(i), 0, capacity * sizeof(T));
        }

        int getCapacity() const {
            return capacity;
        }
    };

}}
