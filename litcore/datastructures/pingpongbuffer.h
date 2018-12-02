//
// Created by egordm on 1-12-18.
//

#pragma once

#include <utility>
#include "ibuffer.h"

namespace litcore { namespace datastructures {
    template<typename T, typename BT>
    class PingPongBuffer : public IBuffer<T> {
    protected:
        int buffer_count = 2;
        BT *buffers = nullptr;
        int current = 0;

    public:
        PingPongBuffer() = default;

        explicit PingPongBuffer(BT &buffer, int buffer_count = 2) {
            buffers = new BT[buffer_count];
            buffers[0] = std::move(buffer);
            for (int i = 1; i < buffer_count; ++i) {
                buffers[i] = buffers[0];
            }
        }

        virtual ~PingPongBuffer() {
            delete[] buffers;
        }

        int get_size() const override {
            return buffers[current].get_size();
        }

        const T *get_data() const override {
            return buffers[current].get_data();
        }

        const T &operator[](size_t index) const override {
            return buffers[current][index];
        }

        const BT &get_current() const {
            return buffers[current];
        }

        BT &get_current() {
            return const_cast<BT &>(static_cast<const PingPongBuffer *>(this)->get_current());
        }

        int get_current_index() const {
            return current;
        }

        BT &get_buffer_at(int i) {
            if (i > buffer_count) i = i % buffer_count;
            return buffers[i];
        }

        void switch_buffer() {
            current++;
            if (current >= buffer_count) current = 0;
        }
    };
}}
