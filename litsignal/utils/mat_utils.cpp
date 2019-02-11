//
// Created by egordm on 3-12-18.
//

#include <cassert>
#include "mat_utils.h"
#include "window_utils.h"

using namespace litsignal;
using namespace arma;

mat mat_utils::mat_from_vecs(const std::vector<vec> &data) {
    assert(data.size() > 0);

    mat ret(data[0].size(), data.size());
    for (int i = 0; i < data.size(); ++i) {
        ret(span::all, static_cast<const uword>(i)) = data[i];
    }

    return ret;
}


mat mat_utils::pad_mat(const mat &data, unsigned int n_before, unsigned int n_after, bool repeat) {
    mat ret(data.n_rows, data.n_cols + n_before + n_after);
    if(repeat) {
        for (int k = 0; k < n_before; ++k) {
            ret(span::all, (const uword) k) = data(span::all, 0);
        }

        for(int k = 0; k < n_after; ++k) {
            ret(span::all, ret.n_cols - 1 - k) = data(span::all, data.n_cols - 1);
        }
    }
    ret(span::all, span((const uword)n_before, ret.n_cols - 1 - n_after)) = data;

    return ret;
}

vec mat_utils::pad_vec(const vec &x, unsigned int n_before, unsigned int n_after) {
    vec ret(x.size() + n_before + n_after, fill::zeros);
    ret(span((uword)n_before, n_before + x.size() - 1)) = x;
    return ret;
}

vec mat_utils::smooth_filter_subtract(const vec &novelty_curve, float smooth_length, int feature_rate) {
    vec smooth_filter = window::hanning(smooth_length * feature_rate);
    vec local_average = conv2(novelty_curve, smooth_filter / sum(smooth_filter), "same");
    return clamp(novelty_curve - local_average, 0, std::numeric_limits<double>::max());
}

cx_mat mat_utils::colwise_normalize_p1(const cx_mat &feature, unsigned int p, double threshold) {
    cx_mat ret(feature.n_rows, feature.n_cols, fill::zeros);

    // normalise the vectors according to the l^p norm
    cx_mat unit_vec(feature.n_rows, 1);
    unit_vec.ones();
    unit_vec = unit_vec / norm(unit_vec, p);

#pragma omp parallel for
    for (int k = 0; k < feature.n_cols; ++k) {
        double n = norm(feature(span::all, (const uword) k), p);

        if (n < threshold) ret(span::all, (const uword) k) = unit_vec;
        else ret(span::all, (const uword) k) = feature(span::all, (const uword) k) / n;
    }

    return ret;
}

