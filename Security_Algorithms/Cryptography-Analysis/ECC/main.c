#include <openssl/ec.h>
#include <openssl/ecdsa.h>
#include <openssl/obj_mac.h>
#include <openssl/err.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <openssl/sha.h>

#include "../measure_performance.h"
#include "../utils.h"

struct s_performance_measurements {
    // Time measurements in seconds
    double key_generation_time;
    double signature_creation_time;
    double signature_verification_time;

    // Memory measurements in bytes
    struct s_memory_usage before_any_operations;
    struct s_memory_usage after_key_generation;
    struct s_memory_usage after_signature_creation;
    struct s_memory_usage after_signature_verification;
};

// Function to generate EC key pair
// curve names can be :
// NID_secp224r1
// NID_X9_62_prime256v1
// NID_secp384r1
// NID_secp521r1
EC_KEY* generate_EC_key_pair(int curve_name) {
    EC_KEY *key = EC_KEY_new_by_curve_name(curve_name);
    if (EC_KEY_generate_key(key) != 1) {
        EC_KEY_free(key);
        return NULL;
    }
    return key;
}

// Function to create a digital signature
ECDSA_SIG* create_signature(const char *message, EC_KEY *key) {
    unsigned char hash[32];
    SHA256((unsigned char*)message, strlen(message), hash);

    ECDSA_SIG *signature = ECDSA_do_sign(hash, sizeof(hash), key);
    return signature;
}

void print_curve_name(int curve_name) {
    printf("ECC with curve ");
    switch (curve_name) {
        case NID_secp224r1:
            printf("NID_secp224r1");
            break;
        case NID_X9_62_prime256v1:
            printf("NID_X9_62_prime256v1");
            break;
        case NID_secp384r1:
            printf("NID_secp384r1");
            break;
        case NID_secp521r1:
            printf("NID_secp521r1");
            break;
        default:
            printf("Unknown curve");
    }
    printf(" ");
}

struct s_performance_measurements run_ECC_performance_analysis(int curve_name) {
    struct s_performance_measurements measurements = {0};

    // Measure memory usage before any operations
    print_curve_name(curve_name);
    measurements.before_any_operations = measure_memory("Before any operations");

    // Generate the EC key pair
    start_timer();
    EC_KEY *key = generate_EC_key_pair(curve_name);
    if(key == NULL){
        fprintf(stderr, "Failed to generate EC key pair\n");
        return measurements;
    }
    measurements.key_generation_time = stop_timer("Key generation");
    print_curve_name(curve_name);
    measurements.after_key_generation = measure_memory("After key generation");

    // Create a digital signature
    char* message = "Secret text";
    start_timer();
    ECDSA_SIG *signature = create_signature(message, key);
    if (signature == NULL) {
        fprintf(stderr, "Failed to create signature\n");
        EC_KEY_free(key);
        return measurements;
    }
    measurements.signature_creation_time = stop_timer("Signature creation");
    print_curve_name(curve_name);
    measurements.after_signature_creation = measure_memory("After signature creation");

    // Verify the digital signature
    unsigned char hash[32] = {0};
    SHA256((unsigned char*)message, strlen(message), hash);
    start_timer();
    int verify_status = ECDSA_do_verify(hash, sizeof(hash), signature, key);
    measurements.signature_verification_time = stop_timer("Signature verification");
    print_curve_name(curve_name);
    measurements.after_signature_verification = measure_memory("After signature verification");

    puts("----------------------------------------");

    if (verify_status != 1) {
        fprintf(stderr, "Failed to verify signature\n");
        print_openssl_errors();
        ECDSA_SIG_free(signature);
        EC_KEY_free(key);
        return measurements;
    }

    // Free resources
    ECDSA_SIG_free(signature);
    EC_KEY_free(key);

    return measurements;
}

#define MEASURE_ITERATIONS 1000

int main(void) {
    // We will run the performance analysis MEASURE_ITERATIONS times for each curve
    struct s_performance_measurements NID_secp224r1measurements[MEASURE_ITERATIONS] = {0};
    struct s_performance_measurements NID_X9_62_prime256v1measurements[MEASURE_ITERATIONS] = {0};
    struct s_performance_measurements NID_secp384r1measurements[MEASURE_ITERATIONS] = {0};
    struct s_performance_measurements NID_secp521r1measurements[MEASURE_ITERATIONS] = {0};

    for (int i = 0; i < MEASURE_ITERATIONS; i++) {
        NID_secp224r1measurements[i] = run_ECC_performance_analysis(NID_secp224r1);
        NID_X9_62_prime256v1measurements[i] = run_ECC_performance_analysis(NID_X9_62_prime256v1);
        NID_secp384r1measurements[i] = run_ECC_performance_analysis(NID_secp384r1);
        NID_secp521r1measurements[i] = run_ECC_performance_analysis(NID_secp521r1);
    }

    // calculate the means of each perfomance measurements
    struct s_performance_measurements NID_secp224r1mean = {0};
    struct s_performance_measurements NID_X9_62_prime256v1mean = {0};
    struct s_performance_measurements NID_secp384r1mean = {0};
    struct s_performance_measurements NID_secp521r1mean = {0};

    for (int i = 0; i < MEASURE_ITERATIONS; i++) {
        NID_secp224r1mean.key_generation_time += NID_secp224r1measurements[i].key_generation_time;
        NID_secp224r1mean.signature_creation_time += NID_secp224r1measurements[i].signature_creation_time;
        NID_secp224r1mean.signature_verification_time += NID_secp224r1measurements[i].signature_verification_time;
        NID_secp224r1mean.before_any_operations.resident_set_size += NID_secp224r1measurements[i].before_any_operations.resident_set_size;
        NID_secp224r1mean.before_any_operations.virtual_memory += NID_secp224r1measurements[i].before_any_operations.virtual_memory;
        NID_secp224r1mean.after_key_generation.resident_set_size += NID_secp224r1measurements[i].after_key_generation.resident_set_size;
        NID_secp224r1mean.after_key_generation.virtual_memory += NID_secp224r1measurements[i].after_key_generation.virtual_memory;
        NID_secp224r1mean.after_signature_creation.resident_set_size += NID_secp224r1measurements[i].after_signature_creation.resident_set_size;
        NID_secp224r1mean.after_signature_creation.virtual_memory += NID_secp224r1measurements[i].after_signature_creation.virtual_memory;
        NID_secp224r1mean.after_signature_verification.resident_set_size += NID_secp224r1measurements[i].after_signature_verification.resident_set_size;
        NID_secp224r1mean.after_signature_verification.virtual_memory += NID_secp224r1measurements[i].after_signature_verification.virtual_memory;

        NID_X9_62_prime256v1mean.key_generation_time += NID_X9_62_prime256v1measurements[i].key_generation_time;
        NID_X9_62_prime256v1mean.signature_creation_time += NID_X9_62_prime256v1measurements[i].signature_creation_time;
        NID_X9_62_prime256v1mean.signature_verification_time += NID_X9_62_prime256v1measurements[i].signature_verification_time;
        NID_X9_62_prime256v1mean.before_any_operations.resident_set_size += NID_X9_62_prime256v1measurements[i].before_any_operations.resident_set_size;
        NID_X9_62_prime256v1mean.before_any_operations.virtual_memory += NID_X9_62_prime256v1measurements[i].before_any_operations.virtual_memory;
        NID_X9_62_prime256v1mean.after_key_generation.resident_set_size += NID_X9_62_prime256v1measurements[i].after_key_generation.resident_set_size;
        NID_X9_62_prime256v1mean.after_key_generation.virtual_memory += NID_X9_62_prime256v1measurements[i].after_key_generation.virtual_memory;
        NID_X9_62_prime256v1mean.after_signature_creation.resident_set_size += NID_X9_62_prime256v1measurements[i].after_signature_creation.resident_set_size;
        NID_X9_62_prime256v1mean.after_signature_creation.virtual_memory += NID_X9_62_prime256v1measurements[i].after_signature_creation.virtual_memory;
        NID_X9_62_prime256v1mean.after_signature_verification.resident_set_size += NID_X9_62_prime256v1measurements[i].after_signature_verification.resident_set_size;
        NID_X9_62_prime256v1mean.after_signature_verification.virtual_memory += NID_X9_62_prime256v1measurements[i].after_signature_verification.virtual_memory;

        NID_secp384r1mean.key_generation_time += NID_secp384r1measurements[i].key_generation_time;
        NID_secp384r1mean.signature_creation_time += NID_secp384r1measurements[i].signature_creation_time;
        NID_secp384r1mean.signature_verification_time += NID_secp384r1measurements[i].signature_verification_time;
        NID_secp384r1mean.before_any_operations.resident_set_size += NID_secp384r1measurements[i].before_any_operations.resident_set_size;
        NID_secp384r1mean.before_any_operations.virtual_memory += NID_secp384r1measurements[i].before_any_operations.virtual_memory;
        NID_secp384r1mean.after_key_generation.resident_set_size += NID_secp384r1measurements[i].after_key_generation.resident_set_size;
        NID_secp384r1mean.after_key_generation.virtual_memory += NID_secp384r1measurements[i].after_key_generation.virtual_memory;
        NID_secp384r1mean.after_signature_creation.resident_set_size += NID_secp384r1measurements[i].after_signature_creation.resident_set_size;
        NID_secp384r1mean.after_signature_creation.virtual_memory += NID_secp384r1measurements[i].after_signature_creation.virtual_memory;
        NID_secp384r1mean.after_signature_verification.resident_set_size += NID_secp384r1measurements[i].after_signature_verification.resident_set_size;
        NID_secp384r1mean.after_signature_verification.virtual_memory += NID_secp384r1measurements[i].after_signature_verification.virtual_memory;

        NID_secp521r1mean.key_generation_time += NID_secp521r1measurements[i].key_generation_time;
        NID_secp521r1mean.signature_creation_time += NID_secp521r1measurements[i].signature_creation_time;
        NID_secp521r1mean.signature_verification_time += NID_secp521r1measurements[i].signature_verification_time;
        NID_secp521r1mean.before_any_operations.resident_set_size += NID_secp521r1measurements[i].before_any_operations.resident_set_size;
        NID_secp521r1mean.before_any_operations.virtual_memory += NID_secp521r1measurements[i].before_any_operations.virtual_memory;
        NID_secp521r1mean.after_key_generation.resident_set_size += NID_secp521r1measurements[i].after_key_generation.resident_set_size;
        NID_secp521r1mean.after_key_generation.virtual_memory += NID_secp521r1measurements[i].after_key_generation.virtual_memory;
        NID_secp521r1mean.after_signature_creation.resident_set_size += NID_secp521r1measurements[i].after_signature_creation.resident_set_size;
        NID_secp521r1mean.after_signature_creation.virtual_memory += NID_secp521r1measurements[i].after_signature_creation.virtual_memory;
        NID_secp521r1mean.after_signature_verification.resident_set_size += NID_secp521r1measurements[i].after_signature_verification.resident_set_size;
        NID_secp521r1mean.after_signature_verification.virtual_memory += NID_secp521r1measurements[i].after_signature_verification.virtual_memory;

    }

    NID_secp224r1mean.key_generation_time /= MEASURE_ITERATIONS;
    NID_secp224r1mean.signature_creation_time /= MEASURE_ITERATIONS;
    NID_secp224r1mean.signature_verification_time /= MEASURE_ITERATIONS;
    NID_secp224r1mean.before_any_operations.resident_set_size /= MEASURE_ITERATIONS;
    NID_secp224r1mean.before_any_operations.virtual_memory /= MEASURE_ITERATIONS;
    NID_secp224r1mean.after_key_generation.resident_set_size /= MEASURE_ITERATIONS;
    NID_secp224r1mean.after_key_generation.virtual_memory /= MEASURE_ITERATIONS;
    NID_secp224r1mean.after_signature_creation.resident_set_size /= MEASURE_ITERATIONS;
    NID_secp224r1mean.after_signature_creation.virtual_memory /= MEASURE_ITERATIONS;
    NID_secp224r1mean.after_signature_verification.resident_set_size /= MEASURE_ITERATIONS;
    NID_secp224r1mean.after_signature_verification.virtual_memory /= MEASURE_ITERATIONS;

    NID_X9_62_prime256v1mean.key_generation_time /= MEASURE_ITERATIONS;
    NID_X9_62_prime256v1mean.signature_creation_time /= MEASURE_ITERATIONS;
    NID_X9_62_prime256v1mean.signature_verification_time /= MEASURE_ITERATIONS;
    NID_X9_62_prime256v1mean.before_any_operations.resident_set_size /= MEASURE_ITERATIONS;
    NID_X9_62_prime256v1mean.before_any_operations.virtual_memory /= MEASURE_ITERATIONS;
    NID_X9_62_prime256v1mean.after_key_generation.resident_set_size /= MEASURE_ITERATIONS;
    NID_X9_62_prime256v1mean.after_key_generation.virtual_memory /= MEASURE_ITERATIONS;
    NID_X9_62_prime256v1mean.after_signature_creation.resident_set_size /= MEASURE_ITERATIONS;
    NID_X9_62_prime256v1mean.after_signature_creation.virtual_memory /= MEASURE_ITERATIONS;
    NID_X9_62_prime256v1mean.after_signature_verification.resident_set_size /= MEASURE_ITERATIONS;
    NID_X9_62_prime256v1mean.after_signature_verification.virtual_memory /= MEASURE_ITERATIONS;

    NID_secp384r1mean.key_generation_time /= MEASURE_ITERATIONS;
    NID_secp384r1mean.signature_creation_time /= MEASURE_ITERATIONS;
    NID_secp384r1mean.signature_verification_time /= MEASURE_ITERATIONS;
    NID_secp384r1mean.before_any_operations.resident_set_size /= MEASURE_ITERATIONS;
    NID_secp384r1mean.before_any_operations.virtual_memory /= MEASURE_ITERATIONS;
    NID_secp384r1mean.after_key_generation.resident_set_size /= MEASURE_ITERATIONS;
    NID_secp384r1mean.after_key_generation.virtual_memory /= MEASURE_ITERATIONS;
    NID_secp384r1mean.after_signature_creation.resident_set_size /= MEASURE_ITERATIONS;
    NID_secp384r1mean.after_signature_creation.virtual_memory /= MEASURE_ITERATIONS;
    NID_secp384r1mean.after_signature_verification.resident_set_size /= MEASURE_ITERATIONS;
    NID_secp384r1mean.after_signature_verification.virtual_memory /= MEASURE_ITERATIONS;

    NID_secp521r1mean.key_generation_time /= MEASURE_ITERATIONS;
    NID_secp521r1mean.signature_creation_time /= MEASURE_ITERATIONS;
    NID_secp521r1mean.signature_verification_time /= MEASURE_ITERATIONS;
    NID_secp521r1mean.before_any_operations.resident_set_size /= MEASURE_ITERATIONS;
    NID_secp521r1mean.before_any_operations.virtual_memory /= MEASURE_ITERATIONS;
    NID_secp521r1mean.after_key_generation.resident_set_size /= MEASURE_ITERATIONS;
    NID_secp521r1mean.after_key_generation.virtual_memory /= MEASURE_ITERATIONS;
    NID_secp521r1mean.after_signature_creation.resident_set_size /= MEASURE_ITERATIONS;
    NID_secp521r1mean.after_signature_creation.virtual_memory /= MEASURE_ITERATIONS;
    NID_secp521r1mean.after_signature_verification.resident_set_size /= MEASURE_ITERATIONS;
    NID_secp521r1mean.after_signature_verification.virtual_memory /= MEASURE_ITERATIONS;

    // write the means of each perfomance measurements time performance in a csv formatted file
    FILE *fp;
    fp = fopen("../ECC/ECC.csv", "w+");

    // Print the header
    fprintf(fp, "curve_name, key_generation_time, signature_creation_time, signature_verification_time, resident_set_size_memory, virtual_memory\n");

    // Print the means of each perfomance measurements
    fprintf(fp, "NID_secp224r1, %f, %f, %f, %zu, %zu\n", NID_secp224r1mean.key_generation_time, NID_secp224r1mean.signature_creation_time, NID_secp224r1mean.signature_verification_time, NID_secp224r1mean.after_signature_verification.resident_set_size, NID_secp224r1mean.after_signature_verification.virtual_memory);
    fprintf(fp, "NID_X9_62_prime256v1, %f, %f, %f, %zu, %zu\n", NID_X9_62_prime256v1mean.key_generation_time, NID_X9_62_prime256v1mean.signature_creation_time, NID_X9_62_prime256v1mean.signature_verification_time, NID_X9_62_prime256v1mean.after_signature_verification.resident_set_size, NID_X9_62_prime256v1mean.after_signature_verification.virtual_memory);
    fprintf(fp, "NID_secp384r1, %f, %f, %f, %zu, %zu\n", NID_secp384r1mean.key_generation_time, NID_secp384r1mean.signature_creation_time, NID_secp384r1mean.signature_verification_time, NID_secp384r1mean.after_signature_verification.resident_set_size, NID_secp384r1mean.after_signature_verification.virtual_memory);
    fprintf(fp, "NID_secp521r1, %f, %f, %f, %zu, %zu\n", NID_secp521r1mean.key_generation_time, NID_secp521r1mean.signature_creation_time, NID_secp521r1mean.signature_verification_time, NID_secp521r1mean.after_signature_verification.resident_set_size, NID_secp521r1mean.after_signature_verification.virtual_memory);

    fclose(fp);

    return 0;
}