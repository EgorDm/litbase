//
// Created by egordm on 1-12-18.
//

#pragma once

#include "ibuffer.h"
#include <memory>

namespace litcore { namespace datastructures {
    template<typename T>
    class Buffer : public IBuffer<T> {
    protected:
        std::unique_ptr<T[]> buffer;
        int size = 0;

    public:
        explicit Buffer(int size) : size(size), buffer(std::unique_ptr<T[]>(new T[size])) {}

        Buffer(const Buffer<T> &other) : Buffer<T>(other.get_size()) {
            std::copy(other.get_data(), other.get_data() + other.get_size(), get_data());
        }

        virtual Buffer<T> &operator=(const Buffer<T> &other) {
            if (&other == this) return *this;

            if (get_size() != other.get_size()) {
                size = other.get_size();
                buffer = std::unique_ptr<T[]>(new T[size]);
            }

            std::copy(other.get_data(), other.get_data() + other.get_size(), get_data());
            return *this;
        }

        int get_size() const override {
            return size;
        }

        const T *get_data() const override {
            return buffer.get();
        }

        const T &operator[](size_t index) const override {
            return buffer[index];
        }
    };
}}
