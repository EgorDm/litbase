//
// Created by egordm on 2-2-19.
//

#pragma once

#include <armadillo>

namespace litsignal { namespace processing { namespace stft {
    arma::mat normalize_stft(const arma::mat &S, double thresh = -74);

    arma::mat log_compress(const arma::mat &S, double compression_c = 1000);
}}}


