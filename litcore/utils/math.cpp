//
// Created by egordm on 21-11-2018.
//

#include <algorithm>
#include "math.h"

using namespace lit;

float math::clip(float n, float lower, float upper) {
    return std::max(lower, std::min(n, upper));
}

int math::calc_gcd(int n1, int n2) {
    int tmp = 0;
    while (n1 > 0) {
        tmp = n1;
        n1 = n2 % n1;
        n2 = tmp;
    }
    return n2;
}

int math::quotient_ceil(int n1, int n2) {
    if (n1 % n2 != 0) return n1 / n2 + 1;
    return n1 / n2;
}
