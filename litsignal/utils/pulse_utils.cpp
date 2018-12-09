//
// Created by egordm on 9-12-18.
//

#include "pulse_utils.h"

using namespace litsignal;

void
pulse_utils::generate_pulse(vec &cosine, vec &sine, double f, int size, int feature_rate, int shift_by) {
    vec T = regspace<vec>(-shift_by, size - 1 - shift_by) / feature_rate;
    vec twoPiFt = f * 2 * M_PI * T;

    cosine = cos(twoPiFt);
    sine = sin(twoPiFt);
}
