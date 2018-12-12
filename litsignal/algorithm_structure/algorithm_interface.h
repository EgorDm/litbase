//
// Created by egordm on 11-12-18.
//

#pragma once

namespace litsignal { namespace algorithm {
    template<typename I, typename F>
    class FrameFactoryInterface {
    public:
        virtual F create() = 0;

        virtual void fill(const I &input, F &frame, int i) = 0;

        virtual int getFrameCount(const I &input) = 0;
    };

    template<typename C, typename O, typename I>
    class AlgorithmNodeInterface {
    public:
        virtual O apply(I &frame, C *context) = 0;
    };

    template<typename F, typename C, typename A>
    class AlgorithmRunnerInterface {
    public:
        virtual void run(A *algorithm) = 0;
    };

    template<typename I, typename F, typename O, typename C>
    class AlgorthmPipelineInterface {
    protected:
        using OwnType = AlgorthmPipelineInterface<I, F, O, C>;
        using RunnerType = AlgorithmRunnerInterface<F, C, AlgorthmPipelineInterface<I, F, O, C>>;
        using FrameFactoryType = FrameFactoryInterface<I, F>;

        const I &input;
        O &output;
        RunnerType *runner = nullptr;
        FrameFactoryType *frame_factory = nullptr;

    public:
        AlgorthmPipelineInterface(const I &input, O &output, RunnerType *runner, FrameFactoryType *frame_factory)
                : input(input), output(output), runner(runner), frame_factory(frame_factory) {}

        virtual ~AlgorthmPipelineInterface() {
            if (runner != nullptr) delete runner;
            if (frame_factory != nullptr) delete frame_factory;
        }

        void process() {
            runner->run(this);
        }

        virtual void preProcess() {};

        virtual void processFrame(C &context, F &frame, int i) = 0;

        virtual void postProcess() {};

        virtual C createContext(F &frame) = 0;

        FrameFactoryType *getFrameFactory() const {
            return frame_factory;
        }

        const I &getInput() const {
            return input;
        }

        O &getOutput() const {
            return output;
        }
    };
}}