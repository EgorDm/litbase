//
// Created by egordm on 11-12-18.
//

#pragma once

#include "algorithm_structure/algorithm_interface.h"
#include "algorithm_parallel_runner.h"

namespace litsignal { namespace algorithm {
    template<typename P>
    class AlgorithmSimpleRunner : public AlgorithmRunnerInterface<P> {
    public:
        void run(P *pipeline) override {
            auto algorithm = pipeline->getAlgorithm();
            auto frame = pipeline->getFrameFactory()->create();
            auto context = algorithm->createContext(frame);

            algorithm->preProcess();

            int frameCount = pipeline->getFrameFactory()->getFrameCount();
            for (int i = 0; i < frameCount; ++i) {
                pipeline->getFrameFactory()->fill(frame, i);
                algorithm->processFrame(context, frame, i);
                pipeline->getOutputBuilder()->fill(&context, i);
            }

            algorithm->postProcess();
        }
    };
}}

