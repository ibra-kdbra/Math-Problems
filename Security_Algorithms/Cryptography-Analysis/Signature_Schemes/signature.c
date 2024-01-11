#include <oqs/oqs.h>
#include "../measure_performance.h"
#include "../utils.h"
#include "signature.h"

int run_signature_scheme_performance_analysis(const char *alg_name, uint8_t *message_to_sign) {
    // Measure memory usage before any operations
    measure_memory("Signature scheme %s - Before any operations", alg_name);

    OQS_SIG *sig = OQS_SIG_new(alg_name);
    if (!sig) {
        fprintf(stderr, "Failed to initialize %s\n", alg_name);
        return 1;
    }

    uint8_t public_key[sig->length_public_key];
    uint8_t secret_key[sig->length_secret_key];

    // Generate the key pair
    start_timer();
    OQS_SIG_keypair(sig, public_key, secret_key);
    stop_timer("Signature scheme %s - Key generation", alg_name);
    measure_memory("Signature scheme %s - After key generation", alg_name);

    // Sign a message
    size_t message_len = sizeof(message_to_sign);
    uint8_t signature[sig->length_signature];
    size_t signature_len;

    start_timer();
    OQS_SIG_sign(sig, signature, &signature_len, message_to_sign, message_len, secret_key);
    stop_timer("Signature scheme %s - Signature creation", alg_name);
    measure_memory("Signature scheme %s - After signature creation", alg_name);

    // Verify the signature
    start_timer();
    OQS_STATUS status = OQS_SIG_verify(sig, message_to_sign, message_len, signature, signature_len, public_key);
    stop_timer("Signature scheme %s - Signature verification", alg_name);
    measure_memory("Signature scheme %s - After signature verification", alg_name);

    puts("----------------------------------------");

    if (status != OQS_SUCCESS) {
        fprintf(stderr, "Failed to verify signature for %s\n", alg_name);
        OQS_SIG_free(sig);
        return 1;
    }

    // Free resources
    OQS_SIG_free(sig);

    return 0;
}

