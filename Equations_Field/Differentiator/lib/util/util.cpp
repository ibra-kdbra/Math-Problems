#include "util.h"

#include <cmath>

bool is_equal(double alpha, double beta) {
    return alpha - beta < CMP_EPS && alpha - beta > -CMP_EPS;
}

void* cover_null(void* value, void* cover) {
    return value ? value : cover;
}

unsigned long long gcd(unsigned long long alpha, unsigned long long beta) {
    while (beta > 0) {
        alpha %= beta;

        alpha ^= beta;
        beta ^= alpha;
        alpha ^= beta;
    }
    return alpha;
}
