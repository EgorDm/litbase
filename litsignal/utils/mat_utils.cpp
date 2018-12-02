//
// Created by egordm on 3-12-18.
//

#include <cassert>
#include "mat_utils.h"

using namespace litsignal::mat_utils;
using namespace arma;

mat litsignal::mat_utils::mat_from_vecs(const std::vector<vec> &data) {
    assert(data.size() > 0);

    mat ret(data[0].size(), data.size());
    for (int i = 0; i < data.size(); ++i) {
        ret(span::all, static_cast<const uword>(i)) = data[i];
    }

    return ret;
}
