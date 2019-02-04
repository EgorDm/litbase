//
// Created by egordm on 2-2-19.
//

#include "stft_processing.h"

using namespace litsignal::processing;

arma::mat stft::normalize_stft(const arma::mat &S, double thresh) {
    thresh = pow(10, thresh / 20);
    return arma::clamp(S / S.max(), thresh, 1);
}

arma::mat stft::log_compress(const arma::mat &S, double compression_c) {
    return arma::log(1 + S * compression_c) / log(1 + compression_c);;
}
