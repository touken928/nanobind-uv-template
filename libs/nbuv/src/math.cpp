#include "nbuv/math.hpp"

#include <numeric>

namespace nbuv {

int add(int a, int b) {
    return a + b;
}

double sum(const std::vector<double>& values) {
    return std::accumulate(values.begin(), values.end(), 0.0);
}

} // namespace nbuv
