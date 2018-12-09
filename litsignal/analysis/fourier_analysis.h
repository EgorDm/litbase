//
// Created by egordm on 8-12-18.
//

#pragma once

#include <armadillo>
#include <sigpack.h>
#include <fftw/fftw.h>

using namespace arma;

namespace litsignal { namespace analysis { namespace fourier_analysis {
    /**
       * returns the complex fourier coefficients of frequency f of the signal s
       * @param freqs
       * @param t
       * @param s time domain signal
       * @param window vector containing window function
       * @param n_overlap overlap given in samples
       * @param f vector of frequencies values of fourier coefficients, in Hz
       * @param sr sampling rate of signal s in Hz
       * @return complex fourier coefficients
       */
    cx_mat
    compute_fourier_coefficients(vec &t, const vec &s, const vec &window, int n_overlap, const vec &f, int sr);


}}}
