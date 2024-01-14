#include <oqs/oqs.h>

#include "kem.h"
#include "../utils.h"

#define FRODOKEM_KEM_VARIANTS_NUMBER 6

int main() {
    const uint8_t *message = medium_message;

    KEM_performance_metrics frodoKEM_640_aes_performance_metrics = {0};
    KEM_performance_metrics frodoKEM_640_shake_performance_metrics = {0};
    KEM_performance_metrics frodoKEM_976_aes_performance_metrics = {0};
    KEM_performance_metrics frodoKEM_976_shake_performance_metrics = {0};
    KEM_performance_metrics frodoKEM_1344_aes_performance_metrics = {0};
    KEM_performance_metrics frodoKEM_1344_shake_performance_metrics = {0};

    // Run performance analysis for all KEM variants
    run_KEM_performance_analysis(OQS_KEM_alg_frodokem_640_aes, message, &frodoKEM_640_aes_performance_metrics);
    run_KEM_performance_analysis(OQS_KEM_alg_frodokem_640_shake, message, &frodoKEM_640_shake_performance_metrics);
    run_KEM_performance_analysis(OQS_KEM_alg_frodokem_976_aes, message, &frodoKEM_976_aes_performance_metrics);
    run_KEM_performance_analysis(OQS_KEM_alg_frodokem_976_shake, message, &frodoKEM_976_shake_performance_metrics);
    run_KEM_performance_analysis(OQS_KEM_alg_frodokem_1344_aes, message, &frodoKEM_1344_aes_performance_metrics);
    run_KEM_performance_analysis(OQS_KEM_alg_frodokem_1344_shake, message, &frodoKEM_1344_shake_performance_metrics);

    // Write performance metrics to CSV file
    FILE *csv_file = fopen("../KEM/frodoKEM_performance_metrics.csv", "w");
    if (csv_file == NULL) {
        perror("Failed to open CSV file");
        return 1;
    }
    fprintf(csv_file, "KEM variant,Key generation time,Encapsulation time,Decapsulation time,Encryption time,Decryption time,Resident set size before any operations (kB),Virtual memory before any operations (kB),Resident set size after key generation (kB),Virtual memory after key generation (kB),Resident set size after encapsulation (kB),Virtual memory after encapsulation (kB),Resident set size after decapsulation (kB),Virtual memory after decapsulation (kB),Resident set size after all operations (kB),Virtual memory after all operations (kB)\n");
    fprintf(csv_file, "%s,%f,%f,%f,%f,%f,%zu,%zu,%zu,%zu,%zu,%zu,%zu,%zu,%zu,%zu\n",
            OQS_KEM_alg_frodokem_640_aes,
            frodoKEM_640_aes_performance_metrics.keypair_generation_time,
            frodoKEM_640_aes_performance_metrics.encapsulation_time,
            frodoKEM_640_aes_performance_metrics.decapsulation_time,
            frodoKEM_640_aes_performance_metrics.encryption_time,
            frodoKEM_640_aes_performance_metrics.decryption_time,
            frodoKEM_640_aes_performance_metrics.before_any_operations.resident_set_size,
            frodoKEM_640_aes_performance_metrics.before_any_operations.virtual_memory,
            frodoKEM_640_aes_performance_metrics.after_key_generation.resident_set_size,
            frodoKEM_640_aes_performance_metrics.after_key_generation.virtual_memory,
            frodoKEM_640_aes_performance_metrics.after_encapsulation.resident_set_size,
            frodoKEM_640_aes_performance_metrics.after_encapsulation.virtual_memory,
            frodoKEM_640_aes_performance_metrics.after_decapsulation.resident_set_size,
            frodoKEM_640_aes_performance_metrics.after_decapsulation.virtual_memory,
            frodoKEM_640_aes_performance_metrics.after_all_operations.resident_set_size,
            frodoKEM_640_aes_performance_metrics.after_all_operations.virtual_memory);

    fprintf(csv_file, "%s,%f,%f,%f,%f,%f,%zu,%zu,%zu,%zu,%zu,%zu,%zu,%zu,%zu,%zu\n",
            OQS_KEM_alg_frodokem_640_shake,
            frodoKEM_640_shake_performance_metrics.keypair_generation_time,
            frodoKEM_640_shake_performance_metrics.encapsulation_time,
            frodoKEM_640_shake_performance_metrics.decapsulation_time,
            frodoKEM_640_shake_performance_metrics.encryption_time,
            frodoKEM_640_shake_performance_metrics.decryption_time,
            frodoKEM_640_shake_performance_metrics.before_any_operations.resident_set_size,
            frodoKEM_640_shake_performance_metrics.before_any_operations.virtual_memory,
            frodoKEM_640_shake_performance_metrics.after_key_generation.resident_set_size,
            frodoKEM_640_shake_performance_metrics.after_key_generation.virtual_memory,
            frodoKEM_640_shake_performance_metrics.after_encapsulation.resident_set_size,
            frodoKEM_640_shake_performance_metrics.after_encapsulation.virtual_memory,
            frodoKEM_640_shake_performance_metrics.after_decapsulation.resident_set_size,
            frodoKEM_640_shake_performance_metrics.after_decapsulation.virtual_memory,
            frodoKEM_640_shake_performance_metrics.after_all_operations.resident_set_size,
            frodoKEM_640_shake_performance_metrics.after_all_operations.virtual_memory);

    fprintf(csv_file, "%s,%f,%f,%f,%f,%f,%zu,%zu,%zu,%zu,%zu,%zu,%zu,%zu,%zu,%zu\n",
            OQS_KEM_alg_frodokem_976_aes,
            frodoKEM_976_aes_performance_metrics.keypair_generation_time,
            frodoKEM_976_aes_performance_metrics.encapsulation_time,
            frodoKEM_976_aes_performance_metrics.decapsulation_time,
            frodoKEM_976_aes_performance_metrics.encryption_time,
            frodoKEM_976_aes_performance_metrics.decryption_time,
            frodoKEM_976_aes_performance_metrics.before_any_operations.resident_set_size,
            frodoKEM_976_aes_performance_metrics.before_any_operations.virtual_memory,
            frodoKEM_976_aes_performance_metrics.after_key_generation.resident_set_size,
            frodoKEM_976_aes_performance_metrics.after_key_generation.virtual_memory,
            frodoKEM_976_aes_performance_metrics.after_encapsulation.resident_set_size,
            frodoKEM_976_aes_performance_metrics.after_encapsulation.virtual_memory,
            frodoKEM_976_aes_performance_metrics.after_decapsulation.resident_set_size,
            frodoKEM_976_aes_performance_metrics.after_decapsulation.virtual_memory,
            frodoKEM_976_aes_performance_metrics.after_all_operations.resident_set_size,
            frodoKEM_976_aes_performance_metrics.after_all_operations.virtual_memory);

    fprintf(csv_file, "%s,%f,%f,%f,%f,%f,%zu,%zu,%zu,%zu,%zu,%zu,%zu,%zu,%zu,%zu\n",
            OQS_KEM_alg_frodokem_976_shake,
            frodoKEM_976_shake_performance_metrics.keypair_generation_time,
            frodoKEM_976_shake_performance_metrics.encapsulation_time,
            frodoKEM_976_shake_performance_metrics.decapsulation_time,
            frodoKEM_976_shake_performance_metrics.encryption_time,
            frodoKEM_976_shake_performance_metrics.decryption_time,
            frodoKEM_976_shake_performance_metrics.before_any_operations.resident_set_size,
            frodoKEM_976_shake_performance_metrics.before_any_operations.virtual_memory,
            frodoKEM_976_shake_performance_metrics.after_key_generation.resident_set_size,
            frodoKEM_976_shake_performance_metrics.after_key_generation.virtual_memory,
            frodoKEM_976_shake_performance_metrics.after_encapsulation.resident_set_size,
            frodoKEM_976_shake_performance_metrics.after_encapsulation.virtual_memory,
            frodoKEM_976_shake_performance_metrics.after_decapsulation.resident_set_size,
            frodoKEM_976_shake_performance_metrics.after_decapsulation.virtual_memory,
            frodoKEM_976_shake_performance_metrics.after_all_operations.resident_set_size,
            frodoKEM_976_shake_performance_metrics.after_all_operations.virtual_memory);

    fprintf(csv_file, "%s,%f,%f,%f,%f,%f,%zu,%zu,%zu,%zu,%zu,%zu,%zu,%zu,%zu,%zu\n",
            OQS_KEM_alg_frodokem_1344_aes,
            frodoKEM_1344_aes_performance_metrics.keypair_generation_time,
            frodoKEM_1344_aes_performance_metrics.encapsulation_time,
            frodoKEM_1344_aes_performance_metrics.decapsulation_time,
            frodoKEM_1344_aes_performance_metrics.encryption_time,
            frodoKEM_1344_aes_performance_metrics.decryption_time,
            frodoKEM_1344_aes_performance_metrics.before_any_operations.resident_set_size,
            frodoKEM_1344_aes_performance_metrics.before_any_operations.virtual_memory,
            frodoKEM_1344_aes_performance_metrics.after_key_generation.resident_set_size,
            frodoKEM_1344_aes_performance_metrics.after_key_generation.virtual_memory,
            frodoKEM_1344_aes_performance_metrics.after_encapsulation.resident_set_size,
            frodoKEM_1344_aes_performance_metrics.after_encapsulation.virtual_memory,
            frodoKEM_1344_aes_performance_metrics.after_decapsulation.resident_set_size,
            frodoKEM_1344_aes_performance_metrics.after_decapsulation.virtual_memory,
            frodoKEM_1344_aes_performance_metrics.after_all_operations.resident_set_size,
            frodoKEM_1344_aes_performance_metrics.after_all_operations.virtual_memory);

    fprintf(csv_file, "%s,%f,%f,%f,%f,%f,%zu,%zu,%zu,%zu,%zu,%zu,%zu,%zu,%zu,%zu\n",
            OQS_KEM_alg_frodokem_1344_shake,
            frodoKEM_1344_shake_performance_metrics.keypair_generation_time,
            frodoKEM_1344_shake_performance_metrics.encapsulation_time,
            frodoKEM_1344_shake_performance_metrics.decapsulation_time,
            frodoKEM_1344_shake_performance_metrics.encryption_time,
            frodoKEM_1344_shake_performance_metrics.decryption_time,
            frodoKEM_1344_shake_performance_metrics.before_any_operations.resident_set_size,
            frodoKEM_1344_shake_performance_metrics.before_any_operations.virtual_memory,
            frodoKEM_1344_shake_performance_metrics.after_key_generation.resident_set_size,
            frodoKEM_1344_shake_performance_metrics.after_key_generation.virtual_memory,
            frodoKEM_1344_shake_performance_metrics.after_encapsulation.resident_set_size,
            frodoKEM_1344_shake_performance_metrics.after_encapsulation.virtual_memory,
            frodoKEM_1344_shake_performance_metrics.after_decapsulation.resident_set_size,
            frodoKEM_1344_shake_performance_metrics.after_decapsulation.virtual_memory,
            frodoKEM_1344_shake_performance_metrics.after_all_operations.resident_set_size,
            frodoKEM_1344_shake_performance_metrics.after_all_operations.virtual_memory);

    fclose(csv_file);
    return 0;
}