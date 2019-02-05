//
// Created by egordm on 4-2-19.
//

#pragma once

#include <algorithm_structure/algorithm_interface.h>
#include <omp.h>

namespace litsignal { namespace algorithm {
    template<typename P>
    class AlgorithmParallelRunner : public AlgorithmRunnerInterface<P> {
    public:
        void run(P *pipeline) override {
            // TODO init frames & contexts
            // TODO use by thread id

            auto algorithm = pipeline->getAlgorithm();

            // Initialize per thread data
            int n_threads = omp_get_max_threads();
            std::vector<typename P::FrameType> frames(n_threads);
            std::vector<typename P::ContextType> contexts(n_threads);
            for (int i = 0; i < n_threads; ++i) {
                frames[i] = pipeline->getFrameFactory()->create();
                contexts[i] = algorithm->createContext(frames[i]);
            }

            algorithm->preProcess();

            int frameCount = pipeline->getFrameFactory()->getFrameCount();
            #pragma omp parallel for
            for (int i = 0; i < frameCount; ++i) {
                int tid = omp_get_thread_num();

                pipeline->getFrameFactory()->fill(frames[tid], i);
                algorithm->processFrame(contexts[tid], frames[tid], i);
                pipeline->getOutputBuilder()->fill(&contexts[tid], i);
            }

            algorithm->postProcess();
        }

    };
}}
