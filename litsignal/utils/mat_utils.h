//
// Created by egordm on 3-12-18.
//

#pragma once

#include <armadillo>

using namespace arma;

namespace litsignal { namespace mat_utils {
    /**
     * Creates a matrix from multipel vectors
     * @param data
     * @return
     */
    mat mat_from_vecs(const std::vector<vec> &data);

    /*
     * Pads given mat by repeating its data before and after the pad
     */
    mat pad_mat(const mat &data, unsigned int n_before, unsigned int n_after, bool repeat = true);

    vec pad_vec(const vec &x, unsigned int n_before, unsigned int n_after);

    vec smooth_filter_subtract(const vec &novelty_curve, float smooth_length, int feature_rate);

    /**
   * Normalizes a feature sequence according to the l^p norm
   * If the norm falls below threshold for a feature vector, then the normalized feature vector is set to be the
   * unit vector.
   * @param feature
   * @param p
   * @param threshold
   * @return normalized feature
   */
    cx_mat colwise_normalize_p1(const cx_mat &feature, unsigned int p, double threshold);

    /**
     * Data interpolation
     * @tparam T
     * @param XG Input axis
     * @param YG Input values
     * @param XI Output axis
     * @param YI Output values
     */
    template<typename T>
    void mat_interp1_nearest(const Col<T>& XG, const Mat<T>& YG, const Col<T>& XI, Mat<T>& YI) {
        const T XG_min = XG.min();
        const T XG_max = XG.max();

        YI.set_size(XI.size(), YG.n_cols);

        const T* XG_mem = XG.memptr();
        const T* XI_mem = XI.memptr();

        const uword NG = XG.n_elem;
        const uword NI = XI.n_elem;

        uword best_j = 0;

        for(uword i=0; i<NI; ++i) {
            T best_err = Datum<T>::inf;
            const T XI_val = XI_mem[i];

            if(XI_val < XG_min) {
                YI(i, span::all) = YG(0, span::all);
            } else if (XI_val > XG_max) {
                YI(i, span::all) = YG(NG - 1, span::all);
            } else {
                for(uword j=best_j; j<NG; ++j)  {
                    const T tmp = XG_mem[j] - XI_val;
                    const T err = (tmp >= T(0)) ? tmp : -tmp;

                    if(err >= best_err)  {
                        // error is going up, so we have found the optimum position
                        break;
                    }
                    else {
                        best_err = err;
                        best_j   = j;   // remember the optimum position
                    }
                }
                YI(i, span::all) = YG(best_j, span::all);
            }
        }
    }
}}