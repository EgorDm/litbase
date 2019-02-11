//
// Created by egordm on 6-2-19.
//

#include "../litsignal_constants.h"
#include "fourier_coefficients_algorithm.h"

using namespace litsignal::analysis;

cx_mat litsignal::analysis::compute_fourier_coefficients(const vec &x, float sr, const vec &f, const vec &window,
                                                         int hop_length, float &out_fr) {
    int window_length = ACI(window.size());
    int frame_count = ACI(std::floor((x.size() - window_length + hop_length) / (float) hop_length));
    cx_mat S(static_cast<const uword>(frame_count), f.size(), fill::zeros);

    vec twoPiT = (2 * M_PI / sr) * regspace<vec>(0, window_length - 1);
    mat lutCosSin(ACU(window_length), f.size() * 2);

    // Create
    for (int f0 = 0; f0 < f.size(); ++f0) {
        vec twoPiFt = f[f0] * twoPiT;
        vec cosine = cos(twoPiFt);
        std::memcpy(lutCosSin.colptr(ACU(f0 * 2)), cosine.memptr(), window_length * sizeof(double));
        vec sine = sin(twoPiFt);
        std::memcpy(lutCosSin.colptr(ACU(f0 * 2 + 1)), sine.memptr(), window_length * sizeof(double));
    }

    //#pragma omp parallel for
    for (int i = 0; i < frame_count; ++i) {
        vec sig(ACU(window_length));
        std::memcpy(sig.memptr(), x.memptr() + i * hop_length, window_length * sizeof(double));

        double co, si;
        for (int f0 = 0; f0 < f.size(); ++f0) {
            co = sum(sig % lutCosSin.col(ACU(f0 * 2)));
            si = sum(sig % lutCosSin.col(ACU(f0 * 2 + 1)));
            S(ACU(i), ACU(f0)) = cx_double(co, si);
        }
    }

    out_fr = sr / hop_length;
    return S;
}
