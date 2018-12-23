//
// Created by egordm on 12-12-18.
//

#pragma once


#include <armadillo>
#include "../algorithm_structure/algorithm_interface.h"
#include "../algorithm_structure/node_ifft.h"
#include "../algorithm_structure/node_window_vec.h"
#include "../algorithm_structure/algorithm_simple_runner.h"

using namespace arma;
using namespace litsignal::algorithm;

namespace litsignal { namespace analysis {
    vec calculate_istft(const cx_mat &input, const vec &window, int hop_size = -1);

    class ISTFTPipeline : public AlgorthmPipelineInterface<cx_mat, cx_vec, vec, IFFTContext> {
    private:
        int hop_size;
        vec window_inv;

        NodeWindowVec node_window;
        NodeIFFT node_ifft;

        vec window_overlap_sum;

    public:
        using SimpleRunner = AlgorithmSimpleRunner<cx_vec, IFFTContext, OwnType>;

        ISTFTPipeline(RunnerType *runner, const cx_mat &input, vec &output, const vec &window, int hop_size = -1);

        void preProcess() override;

        void processFrame(IFFTContext &context, cx_vec &frame, int i) override;

        void postProcess() override;

        IFFTContext createContext(cx_vec &frame) override;
    };
}};
