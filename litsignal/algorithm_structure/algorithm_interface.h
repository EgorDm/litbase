//
// Created by egordm on 11-12-18.
//

#pragma once

#include "../litsignal_constants.h"

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

    template<typename F>
    class FrameFactoryInterface {
    public:
        virtual F create() = 0;

        virtual void fill(F &frame, int i) = 0;

        virtual int getFrameCount() = 0;

        virtual int getInputSize() = 0;
    };

    template<typename O, typename OF>
    class AlgorithmOutputBuilder {
    protected:
        O output;
        int cursor = 0;

    public:
        explicit AlgorithmOutputBuilder(const O &output) : output(output) {}

        virtual void fill(AlgorithmContext<OF> *context, int i) = 0;

        virtual void resize(int capacity) = 0;

        virtual int getSize() = 0;

        virtual int getCursor() {
            return cursor;
        }

        O &getOutput() {
            return output;
        }

        virtual void reset() {
            cursor = 0;
        };
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

    template<typename F, typename OF, typename O, typename C, typename A>
    class AlgorithmPipeline {
    public:
        using FrameFactoryType = FrameFactoryInterface<F>;
        using OutputBuilderType = AlgorithmOutputBuilder<O, OF>;
        using AlgorithmType = A;

    protected:
        FrameFactoryType *frame_factory = nullptr;
        OutputBuilderType *output_builder = nullptr;
        AlgorithmType algorithm;

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

        void setFrameFactory(FrameFactoryType *frame_factory) {
            AlgorithmPipeline::frame_factory = frame_factory;
        }

        OutputBuilderType *getOutputBuilder() {
            return output_builder;
        }

        void setOutputBuilder(OutputBuilderType *output_builder) {
            AlgorithmPipeline::output_builder = output_builder;
        }

        AlgorithmType *getAlgorithm() {
            return &algorithm;
        }
    };
}}