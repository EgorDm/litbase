//
// Created by egordm on 11-12-18.
//

#pragma once

#include "algorithm_interface.h"

namespace litsignal { namespace algorithm {
    template<typename F, typename C, typename A>
    class AlgorithmSimpleRunner : public AlgorithmRunnerInterface<F, C, A> {
    public:
        void run(A *algorithm) override {
            F frame = algorithm->getFrameFactory()->create();
            C context = algorithm->createContext(frame);

            algorithm->preProcess();

            int frameCount = algorithm->getFrameFactory()->getFrameCount(algorithm->getInput());
            for (int i = 0; i < frameCount; ++i) {
                algorithm->getFrameFactory()->fill(algorithm->getInput(), frame, i);
                algorithm->processFrame(context, frame, i);
            }

            algorithm->postProcess();
        }
    };
}}

