//
// Created by egordm on 23-12-18.
//

#pragma once

#include <armadillo>
#include <algorithm_structure/algorithm_interface.h>
#include <algorithm_structure/node_ifft.h>
#include <algorithm_structure/node_window_vec.h>
#include <algorithm_structure/frame_factory_mat.h>
#include <algorithm_structure/output_builder_vec.h>
#include <processing/phase_vocoder_pipeline.h>

using namespace arma;
using namespace litsignal::algorithm;

namespace litsignal { namespace analysis {
    class ISTFTAlgorithm : public AlgorithmInterface<arma::cx_vec, IFFTContext> {
    protected:
        NodeWindowVec<double> node_window;
        NodeIFFT node_ifft;

    public:
        /**
         * Warning: Applies the inverse of the window
         * @param window
         * @param hop_size
         */
        explicit ISTFTAlgorithm(const arma::vec &window, int hop_size)
                : node_ifft(),
                  node_window(window / sqrt((window.size() / (double) hop_size) / 2.)) {} // Take window inverse

        void processFrame(IFFTContext &context, arma::cx_vec &frame, int i) override {
            node_ifft.apply(frame, &context);
            node_window.apply(context.getOutput(), &context);
        }

        IFFTContext createContext(arma::cx_vec &frame) override {
            return IFFTContext(ACI(node_window.getWindowSize()));
        }
    };

    using ISTFTPipeline = AlgorithmPipeline<cx_mat, cx_vec, vec, vec, IFFTContext, ISTFTAlgorithm>;

    ISTFTPipeline build_istft_algorithm(const cx_mat &input, const vec &window, int hop_size) {
        ISTFTAlgorithm algorithm(window, hop_size);
        auto *frameFactory = new FrameFactoryMat<cx_double>(input);
        auto *outputBuilder = new OutputBuilderVec<double>(hop_size);
        int signal_length = (frameFactory->getFrameCount() - 1) * hop_size + ACI(window.size());
        outputBuilder->resize(signal_length);

        return ISTFTPipeline(frameFactory, outputBuilder, algorithm);
    }

    vec calculate_istft(const cx_mat &input, const vec &window, int hop_size) {
        ISTFTPipeline pipeline = build_istft_algorithm(input, window, hop_size);
        AlgorithmSimpleRunner<ISTFTPipeline> runner;
        runner.run(&pipeline);
        return pipeline.getOutputBuilder()->getOutput();
    }
}}