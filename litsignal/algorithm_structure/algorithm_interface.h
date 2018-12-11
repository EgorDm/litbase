//
// Created by egordm on 11-12-18.
//

#pragma once

namespace litsignal { namespace algorithm {
    template<typename I, typename F>
    class FrameFactoryInterface {
    public:
        virtual F create() = 0;

        virtual void fill(I &input, F &frame, int i) = 0;

        virtual int getFrameCount(I &input) = 0;
    };

    template<typename C, typename O, typename I>
    class AlgorithmNodeInterface {
    public:
        virtual O apply(I *input, C *context) = 0;
    };

    template<typename F, typename C, typename A>
    class AlgorithmRunnerInterface {
    public:
        virtual void run(A *algorithm) = 0;
    };

    template<typename I, typename F, typename O, typename C>
    class AlgorthmPipelineInterface {
    protected:
        using RunnerType = AlgorithmRunnerInterface<F, C, AlgorthmPipelineInterface<I, F, O, C>>;
        using FrameFactoryType = FrameFactoryInterface<I, F>;

        I &input;
        O &output;
        RunnerType *runner = nullptr;
        FrameFactoryType *frame_factory = nullptr;

    public:
        AlgorthmPipelineInterface(I &input, O &output, RunnerType *runner, FrameFactoryType *frame_factory)
                : input(input), output(output), runner(runner), frame_factory(frame_factory) {}

        virtual ~AlgorthmPipelineInterface() {
            if (runner != nullptr) delete runner;
            if (frame_factory != nullptr) delete frame_factory;
        }

        void run() {
            runner->run(this);
        }

        virtual void process(C &context, F &frame, int i) = 0;

        virtual C createContext(F &frame) = 0;

        FrameFactoryType *getFrameFactory() const {
            return frame_factory;
        }

        I &getInput() const {
            return input;
        }

        O &getOutput() const {
            return output;
        }
    };
}}