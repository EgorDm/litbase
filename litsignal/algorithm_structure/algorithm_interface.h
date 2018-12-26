//
// Created by egordm on 11-12-18.
//

#pragma once

namespace litsignal { namespace algorithm {
    template<typename P>
    class AlgorithmRunnerInterface {
    public:
        virtual void run(P *pipeline) = 0;
    };

    template<typename SO>
    class AlgorithmContext {
    protected:
        SO output;

    public:
        using ContextType = AlgorithmContext<SO>;

        explicit AlgorithmContext(const SO &output) : output(output) {}

        virtual SO &getOutput() {
            return output;
        }
    };

    template<typename I, typename F>
    class FrameFactoryInterface {
    protected:
        const I &input;

    public:
        explicit FrameFactoryInterface(const I &input) : input(input) {}

        virtual F create() = 0;

        virtual void fill(F &frame, int i) = 0;

        virtual int getFrameCount() = 0;
    };

    template<typename O, typename OF>
    class AlgorithmOutputBuilder {
    protected:
        O output;

    public:
        explicit AlgorithmOutputBuilder(const O &output) : output(output) {}

        virtual void fill(AlgorithmContext<OF> *context, int i) = 0;

        virtual void resize(int capacity) = 0;

        virtual bool is_full() = 0;

        O &getOutput() {
            return output;
        }
    };

    template<typename C, typename I>
    class AlgorithmNodeInterface {
    public:
        virtual void apply(I &frame, C *context) = 0;
    };

    template<typename F, typename C>
    class AlgorithmInterface {
    public:
        virtual void preProcess() {};

        virtual void processFrame(C &context, F &frame, int i) = 0;

        virtual void postProcess() {};

        virtual C createContext(F &frame) = 0;
    };

    template<typename I, typename F, typename OF, typename O, typename C, typename A>
    class AlgorithmPipeline {
    public:
        using FrameFactoryType = FrameFactoryInterface<I, F>;
        using OutputBuilderType = AlgorithmOutputBuilder<O, OF>;
        using AlgorithmType = AlgorithmInterface<F, C>;

    protected:
        FrameFactoryType *frame_factory = nullptr;
        OutputBuilderType *output_builder = nullptr;
        A algorithm;

    public:
        AlgorithmPipeline(FrameFactoryType *frame_factory, OutputBuilderType *output_builder, A algorithm)
                : frame_factory(frame_factory), output_builder(output_builder), algorithm(algorithm) {}

        virtual ~AlgorithmPipeline() {
            if (frame_factory != nullptr) delete frame_factory;
            if (output_builder != nullptr) delete output_builder;
        }

        FrameFactoryType *getFrameFactory() {
            return frame_factory;
        }

        OutputBuilderType *getOutputBuilder() {
            return output_builder;
        }

        AlgorithmType *getAlgorithm() {
            return &algorithm;
        }
    };
}}