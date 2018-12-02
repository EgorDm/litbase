//
// Created by egordm on 1-12-18.
//

#pragma once

#include <cstddef>

namespace litcore { namespace datastructures {
    template<typename T>
    class IBuffer {
    public:
        virtual int get_size() const = 0;

        virtual const T *get_data() const = 0;

        virtual T *get_data() {
            return const_cast<T*>(static_cast<const IBuffer*>(this)->get_data());
        }

        virtual const T &operator[](size_t index) const = 0;

        virtual T &operator[](size_t index) {
            return const_cast<T&>(static_cast<const IBuffer*>(this)[index]);
        }

        virtual void reset() {};
    };
}}
