//
// Created by egordm on 9-1-19.
//

#pragma once



namespace litaudio { namespace structures {
    class AudioContainerInterface {
    protected:
        int sample_rate;

    public:
        explicit AudioContainerInterface(int sample_rate) : sample_rate(sample_rate) {}

        int getSampleRate() const {
            return sample_rate;
        }

        void setSampleRate(int sample_rate) {
            AudioContainerInterface::sample_rate = sample_rate;
        }

        AVSampleFormat getFormat() const {
            return getBuffer()->getFormat();
        }

        void setFormat(AVSampleFormat format) {
            getBuffer()->setFormat(format);
        }

        int getChannelCount() const {
            return getBuffer()->getChannelCount();
        }

        void setChannelCount(int channel_count) {
            getModifiableBuffer()->setChannelCount(channel_count);
        }

        int getSampleCount() const {
            return getBuffer()->getSampleCount();
        }

        void setSampleCount(int sample_count) {
            getModifiableBuffer()->setSampleCount(sample_count);
        }

        int getSampleSize() const {
            return getBuffer()->getSampleSize();
        }

        virtual AudioBufferInterface *getBuffer() = 0;

        const AudioBufferInterface *getBuffer() const {
            return const_cast<AudioContainerInterface *>(this)->getBuffer();
        }

        virtual AudioBufferModifiableInterface *getModifiableBuffer() = 0;

        const AudioBufferModifiableInterface *getModifiableBuffer() const {
            return const_cast<AudioContainerInterface *>(this)->getModifiableBuffer();
        }

        bool isSameFormat(const AudioContainerInterface *other) const {
            return getSampleRate() == other->getSampleRate() && getChannelCount() == other->getChannelCount() &&
                   getFormat() == other->getFormat();
        }

        virtual void copyFormat(const AudioContainerInterface *src) {
            if (getFormat() == AV_SAMPLE_FMT_NONE) setFormat(src->getFormat());
            if (getSampleRate() < 0) setSampleRate(src->getSampleRate());
            if (getChannelCount() < 0) setChannelCount(src->getChannelCount());
        }
    };
}}