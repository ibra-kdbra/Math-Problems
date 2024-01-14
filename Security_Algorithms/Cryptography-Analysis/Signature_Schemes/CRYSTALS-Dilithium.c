#include <oqs/oqs.h>
#include "signature.h"
#include "../utils.h"

#define DILITHIUM_SIGNATURE_VARIANTS_NUMBER 3

int main() {
    // Run performance analysis for different signature algorithms
    const char *signature_algs[DILITHIUM_SIGNATURE_VARIANTS_NUMBER] = {
            OQS_SIG_alg_dilithium_2,
            OQS_SIG_alg_dilithium_3,
            OQS_SIG_alg_dilithium_5,
    };

    // Test algorithms with every message of different sizes
    for (size_t i = 0; i < DILITHIUM_SIGNATURE_VARIANTS_NUMBER; i++) {
        puts("Short message");
        run_signature_scheme_performance_analysis(signature_algs[i], short_message);
        puts("Medium message");
        run_signature_scheme_performance_analysis(signature_algs[i], medium_message);
        puts("Long message");
        run_signature_scheme_performance_analysis(signature_algs[i], long_message);
    }
}

