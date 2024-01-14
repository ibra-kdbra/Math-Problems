#include <oqs/oqs.h>

#include "kem.h"
#include "../utils.h"

#define KYBER_KEM_VARIANTS_NUMBER 3

int main() {
    const uint8_t *message = short_message;

    KEM_performance_metrics kyber_512_performance_metrics = {0};
    KEM_performance_metrics kyber_768_performance_metrics = {0};
    KEM_performance_metrics kyber_1024_performance_metrics = {0};

    // Run performance analysis for all KEM variants with different plaintext sizes
    run_KEM_performance_analysis(OQS_KEM_alg_kyber_512, message, &kyber_512_performance_metrics);
    run_KEM_performance_analysis(OQS_KEM_alg_kyber_768, message, &kyber_768_performance_metrics);
    run_KEM_performance_analysis(OQS_KEM_alg_kyber_1024, message, &kyber_1024_performance_metrics);

    // Write performance metrics to CSV file
    FILE *file = fopen("../KEM/kyber_performance_metrics.csv", "w");
    if (file == NULL) {
        fprintf(stderr, "Failed to open file for writing\n");
        return 1;
    }

    // Print header
    fprintf(file, "KEM variant,Key generation time,Encapsulation time,Decapsulation time,Encryption time,Decryption time,Resident set size before any operations (kB),Virtual memory before any operations (kB),Resident set size after key generation (kB),Virtual memory after key generation (kB),Resident set size after encapsulation (kB),Virtual memory after encapsulation (kB),Resident set size after decapsulation (kB),Virtual memory after decapsulation (kB),Resident set size after all operations (kB),Virtual memory after all operations (kB)\n");

    // Print metrics for Kyber 512
    fprintf(file, "%s,%f,%f,%f,%f,%f,%zu,%zu,%zu,%zu,%zu,%zu,%zu,%zu,%zu,%zu\n",
            OQS_KEM_alg_kyber_512,
            kyber_512_performance_metrics.keypair_generation_time,
            kyber_512_performance_metrics.encapsulation_time,
            kyber_512_performance_metrics.decapsulation_time,
            kyber_512_performance_metrics.encryption_time,
            kyber_512_performance_metrics.decryption_time,
            kyber_512_performance_metrics.before_any_operations.resident_set_size,
            kyber_512_performance_metrics.before_any_operations.virtual_memory,
            kyber_512_performance_metrics.after_key_generation.resident_set_size,
            kyber_512_performance_metrics.after_key_generation.virtual_memory,
            kyber_512_performance_metrics.after_encapsulation.resident_set_size,
            kyber_512_performance_metrics.after_encapsulation.virtual_memory,
            kyber_512_performance_metrics.after_decapsulation.resident_set_size,
            kyber_512_performance_metrics.after_decapsulation.virtual_memory,
            kyber_512_performance_metrics.after_all_operations.resident_set_size,
            kyber_512_performance_metrics.after_all_operations.virtual_memory
    );

    // Print metrics for Kyber 768
    fprintf(file, "%s,%f,%f,%f,%f,%f,%zu,%zu,%zu,%zu,%zu,%zu,%zu,%zu,%zu,%zu\n",
            OQS_KEM_alg_kyber_768,
            kyber_768_performance_metrics.keypair_generation_time,
            kyber_768_performance_metrics.encapsulation_time,
            kyber_768_performance_metrics.decapsulation_time,
            kyber_768_performance_metrics.encryption_time,
            kyber_768_performance_metrics.decryption_time,
            kyber_768_performance_metrics.before_any_operations.resident_set_size,
            kyber_768_performance_metrics.before_any_operations.virtual_memory,
            kyber_768_performance_metrics.after_key_generation.resident_set_size,
            kyber_768_performance_metrics.after_key_generation.virtual_memory,
            kyber_768_performance_metrics.after_encapsulation.resident_set_size,
            kyber_768_performance_metrics.after_encapsulation.virtual_memory,
            kyber_768_performance_metrics.after_decapsulation.resident_set_size,
            kyber_768_performance_metrics.after_decapsulation.virtual_memory,
            kyber_768_performance_metrics.after_all_operations.resident_set_size,
            kyber_768_performance_metrics.after_all_operations.virtual_memory
    );

    // Print metrics for Kyber 1024
    fprintf(file, "%s,%f,%f,%f,%f,%f,%zu,%zu,%zu,%zu,%zu,%zu,%zu,%zu,%zu,%zu\n",
            OQS_KEM_alg_kyber_1024,
            kyber_1024_performance_metrics.keypair_generation_time,
            kyber_1024_performance_metrics.encapsulation_time,
            kyber_1024_performance_metrics.decapsulation_time,
            kyber_1024_performance_metrics.encryption_time,
            kyber_1024_performance_metrics.decryption_time,
            kyber_1024_performance_metrics.before_any_operations.resident_set_size,
            kyber_1024_performance_metrics.before_any_operations.virtual_memory,
            kyber_1024_performance_metrics.after_key_generation.resident_set_size,
            kyber_1024_performance_metrics.after_key_generation.virtual_memory,
            kyber_1024_performance_metrics.after_encapsulation.resident_set_size,
            kyber_1024_performance_metrics.after_encapsulation.virtual_memory,
            kyber_1024_performance_metrics.after_decapsulation.resident_set_size,
            kyber_1024_performance_metrics.after_decapsulation.virtual_memory,
            kyber_1024_performance_metrics.after_all_operations.resident_set_size,
            kyber_1024_performance_metrics.after_all_operations.virtual_memory
    );

    fclose(file);

    return 0;
}


