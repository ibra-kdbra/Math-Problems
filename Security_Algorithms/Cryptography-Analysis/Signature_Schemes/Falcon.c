#include <oqs/oqs.h>
#include "signature.h"
#include "../utils.h"

#define FALCON_SIGNATURE_VARIANTS_NUMBER 2

int main() {
    // Run performance analysis for different signature algorithms
    const char *signature_algs[FALCON_SIGNATURE_VARIANTS_NUMBER] = {
            OQS_SIG_alg_falcon_512,
            OQS_SIG_alg_falcon_1024
    };

    // Test algorithms with every message of different sizes
    for (size_t i = 0; i < FALCON_SIGNATURE_VARIANTS_NUMBER; i++) {
        puts("Short message");
        run_signature_scheme_performance_analysis(signature_algs[i], short_message);
        puts("Medium message");
        run_signature_scheme_performance_analysis(signature_algs[i], medium_message);
        puts("Long message");
        run_signature_scheme_performance_analysis(signature_algs[i], long_message);
    }
}

