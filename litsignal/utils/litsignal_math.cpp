//
// Created by egordm on 2-12-18.
//

#include "litsignal_math.h"
#include "../litsignal_constants.h"

using namespace litsignal::constants;

vec lit::math::sinc_fac(const vec &x) {
    vec ret(x.size());
    for (uword i = 0; i < x.size(); ++i) {
        if (fabs(x.at(i)) < EPSILON) ret.at(i) = 1;
        else ret.at(i) = sin(M_PI * x.at(i)) / (M_PI * x.at(i));
    }

    return ret;
}

vec lit::math::my_hanning(const uword N) {
    vec h(N);
    for (uword i = 0; i < N; i++) {
        h[i] = 0.5 - 0.5 * std::cos(PI2 * i / (N - 1));
    }
    return h;
}
