//
// Created by egordm on 9-12-18.
//

#pragma once

#include <fftw3.h>
#include <litsignal_constants.h>
#include "window_processor.h"

namespace litsignal { namespace helpers {
    struct FFTProcessorContext {
        fftw_plan plan = nullptr;
        arma::cx_vec fft_res;

        explicit FFTProcessorContext(arma::vec &frame) : fft_res(frame.size()) {
            auto *in = frame.memptr();
            auto *out = reinterpret_cast<fftw_complex *>(fft_res.memptr());
            plan = fftw_plan_dft_r2c_1d(static_cast<int>(fft_res.size()), in, out, FFTW_ESTIMATE);
        }

        virtual ~FFTProcessorContext() {
            if (plan != nullptr) fftw_destroy_plan(plan);
            fftw_cleanup();
        }
    };

    template<typename O>
    class FFTProcessor : public WindowProcessor<FFTProcessorContext, O> {
    private:
        using parentClass = WindowProcessor<FFTProcessorContext, O>;

    protected:
        bool mirror;

    public:
        FFTProcessor(const vec &input, O &output, const vec &window, int hop_size, bool mirror = true)
                : parentClass(input, output, window, hop_size), mirror(mirror) {}

    protected:
        FFTProcessorContext *createContext(arma::vec &frame) override {
            return new FFTProcessorContext(frame);
        }

        void processFrame(FFTProcessorContext *context, arma::vec &frame, int i) override {
            parentClass::processFrame(context, frame, i);
            fftw_execute(context->plan);

            // Mirror the results since fft is symmetric
            if(mirror) {
                int N = static_cast<int>(context->fft_res.size());
                int offset = static_cast<int>(ceil(N / 2.f));
                int n_elem = N - offset;
                for (int i = 0; i < n_elem; ++i) {
                    context->fft_res(ACU(offset + i)) = std::conj(context->fft_res(ACU(n_elem - i)));
                }
            }
        }
    };

}}
