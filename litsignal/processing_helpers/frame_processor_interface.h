//
// Created by egordm on 9-12-18.
//

#pragma once

namespace litsignal { namespace helpers {
    template<typename F, typename C, typename O>
    class FrameProcessorInterface {
    protected:
        int frame_count;
        int frame_size;
        int hop_size;

        const F &input;
        O &output;

    protected:
        virtual void preProcess() {}

        virtual void postProcess() {}

        virtual void fillFrame(F &frame, int i) = 0;

        virtual void processFrame(C *context, F &frame, int i) = 0;

        virtual F createFrame(int size) = 0;

        virtual C *createContext(F &frame) = 0;

    public:
        FrameProcessorInterface(const F &input, O &output, int frame_count, int frame_size, int hop_size)
                : frame_count(frame_count), frame_size(frame_size), hop_size(hop_size), input(input), output(output) {
        }

        virtual void process() {
            preProcess();

            F frame = createFrame(frame_size);
            C *context = createContext(frame);

            for (int i = 0; i < frame_count; ++i) {
                fillFrame(frame, i);
                processFrame(context, frame, i);
            }

            delete context;

            postProcess();
        }
    };
}}

