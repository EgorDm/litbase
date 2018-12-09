//
// Created by egordm on 8-12-18.
//

#include <utils/math.h>
#include "fourier_analysis.h"

using namespace litsignal::analysis;
using namespace litsignal::analysis::fourier_analysis;

cx_mat fourier_analysis::compute_fourier_coefficients
        (vec &t, const vec &s, const vec &window, int n_overlap, const vec &f, int sr) {
    int window_size = (int) window.size();
    int hop_size = window_size - n_overlap;

    vec twoPiT = (2 * M_PI / sr) * regspace<vec>(0, window_size - 1);
    int win_num = lit::math::fix((s.size() - n_overlap) / (double) (window_size - n_overlap));

    cx_mat x(static_cast<const uword>(win_num), f.size(), fill::zeros);
    t = linspace(window_size / 2., window_size / 2. + (win_num - 1) * hop_size, (const uword) win_num) / sr;

#pragma omp parallel for
    for (int f0 = 0; f0 < f.size(); ++f0) {
        vec twoPiFt = f[f0] * twoPiT;
        vec cosine = cos(twoPiFt);
        vec sine = sin(twoPiFt);

        for (int w = 0; w < win_num; ++w) {
            int start = w * hop_size;
            int stop = start + window_size;

            vec sig = s.subvec((const uword) start, (const uword) (stop - 1)) % window;
            auto co = sum(sig % cosine);
            auto si = sum(sig % sine);

            x((const uword) w, (const uword) f0) = cx_double(co, si);
        }
    }

    return x.st();
}
