#include <oqs/oqs.h>

#include "kem.h"
#include "../utils.h"

#define BIKE_KEM_VARIANTS_NUMBER 3
#define MEASURE_ITERATIONS 1000

int main() {

    // Declare structure to store performance metrics. We will run the performance analysis MEASURE_ITERATIONS times for each KEM variant.
    KEM_performance_metrics BIKE_L1_performance_metrics_short_message[MEASURE_ITERATIONS] = {0};
    KEM_performance_metrics BIKE_L1_performance_metrics_medium_message[MEASURE_ITERATIONS] = {0};
    KEM_performance_metrics BIKE_L1_performance_metrics_long_message[MEASURE_ITERATIONS] = {0};
    KEM_performance_metrics BIKE_L3_performance_metrics_short_mesage[MEASURE_ITERATIONS] = {0};
    KEM_performance_metrics BIKE_L3_performance_metrics_medium_message[MEASURE_ITERATIONS] = {0};
    KEM_performance_metrics BIKE_L3_performance_metrics_long_message[MEASURE_ITERATIONS] = {0};
    KEM_performance_metrics BIKE_L5_performance_metrics_short_mesage[MEASURE_ITERATIONS] = {0};
    KEM_performance_metrics BIKE_L5_performance_metrics_medium_message[MEASURE_ITERATIONS] = {0};
    KEM_performance_metrics BIKE_L5_performance_metrics_long_message[MEASURE_ITERATIONS] = {0};

    // Run performance analysis for all KEM variants with different plaintext sizes, MEASURE_ITERATIONS times.
    for (size_t j = 0; j < MEASURE_ITERATIONS; j++) {
        // BIKE-L1
        puts("Short message");
        BIKE_L1_performance_metrics_short_message[j].message_lengthyness = SHORT_MESSAGE;
        run_KEM_performance_analysis("BIKE-L1", short_message, &BIKE_L1_performance_metrics_short_message[j]);
        puts("Medium message");
        BIKE_L1_performance_metrics_medium_message[j].message_lengthyness = MEDIUM_MESSAGE;
        run_KEM_performance_analysis("BIKE-L1", medium_message, &BIKE_L1_performance_metrics_medium_message[j]);
        puts("Long message");
        BIKE_L1_performance_metrics_long_message[j].message_lengthyness = LONG_MESSAGE;
        run_KEM_performance_analysis("BIKE-L1", long_message, &BIKE_L1_performance_metrics_long_message[j]);

        // BIKE-L3
        puts("Short message");
        BIKE_L3_performance_metrics_short_mesage[j].message_lengthyness = SHORT_MESSAGE;
        run_KEM_performance_analysis("BIKE-L3", short_message, &BIKE_L3_performance_metrics_short_mesage[j]);
        puts("Medium message");
        BIKE_L3_performance_metrics_medium_message[j].message_lengthyness = MEDIUM_MESSAGE;
        run_KEM_performance_analysis("BIKE-L3", medium_message, &BIKE_L3_performance_metrics_medium_message[j]);
        puts("Long message");
        BIKE_L3_performance_metrics_long_message[j].message_lengthyness = LONG_MESSAGE;
        run_KEM_performance_analysis("BIKE-L3", long_message, &BIKE_L3_performance_metrics_long_message[j]);

        // BIKE-L5
        puts("Short message");
        BIKE_L5_performance_metrics_short_mesage[j].message_lengthyness = SHORT_MESSAGE;
        run_KEM_performance_analysis("BIKE-L5", short_message, &BIKE_L5_performance_metrics_short_mesage[j]);
        puts("Medium message");
        BIKE_L5_performance_metrics_medium_message[j].message_lengthyness = MEDIUM_MESSAGE;
        run_KEM_performance_analysis("BIKE-L5", medium_message, &BIKE_L5_performance_metrics_medium_message[j]);
        puts("Long message");
        BIKE_L5_performance_metrics_long_message[j].message_lengthyness = LONG_MESSAGE;
        run_KEM_performance_analysis("BIKE-L5", long_message, &BIKE_L5_performance_metrics_long_message[j]);
    }

    // Calculate the means of all metrics
    KEM_performance_metrics BIKE_L1_performance_metrics_mean_short_message = {0};
    KEM_performance_metrics BIKE_L1_performance_metrics_mean_medium_message = {0};
    KEM_performance_metrics BIKE_L1_performance_metrics_mean_long_message = {0};

    KEM_performance_metrics BIKE_L3_performance_metrics_mean_short_message = {0};
    KEM_performance_metrics BIKE_L3_performance_metrics_mean_medium_message = {0};
    KEM_performance_metrics BIKE_L3_performance_metrics_mean_long_message = {0};

    KEM_performance_metrics BIKE_L5_performance_metrics_mean_short_message = {0};
    KEM_performance_metrics BIKE_L5_performance_metrics_mean_medium_message = {0};
    KEM_performance_metrics BIKE_L5_performance_metrics_mean_long_message = {0};

    for (size_t i = 0; i < MEASURE_ITERATIONS; i++) {
        // BIKE-L1
        // Short message
        BIKE_L1_performance_metrics_mean_short_message.keypair_generation_time += BIKE_L1_performance_metrics_short_message[i].keypair_generation_time;
        BIKE_L1_performance_metrics_mean_short_message.encapsulation_time += BIKE_L1_performance_metrics_short_message[i].encapsulation_time;
        BIKE_L1_performance_metrics_mean_short_message.decapsulation_time += BIKE_L1_performance_metrics_short_message[i].decapsulation_time;
        BIKE_L1_performance_metrics_mean_short_message.encryption_time += BIKE_L1_performance_metrics_short_message[i].encryption_time;
        BIKE_L1_performance_metrics_mean_short_message.decryption_time += BIKE_L1_performance_metrics_short_message[i].decryption_time;
        BIKE_L1_performance_metrics_mean_short_message.before_any_operations.resident_set_size += BIKE_L1_performance_metrics_short_message[i].before_any_operations.resident_set_size;
        BIKE_L1_performance_metrics_mean_short_message.before_any_operations.virtual_memory += BIKE_L1_performance_metrics_short_message[i].before_any_operations.virtual_memory;
        BIKE_L1_performance_metrics_mean_short_message.after_key_generation.resident_set_size += BIKE_L1_performance_metrics_short_message[i].after_key_generation.resident_set_size;
        BIKE_L1_performance_metrics_mean_short_message.after_key_generation.virtual_memory += BIKE_L1_performance_metrics_short_message[i].after_key_generation.virtual_memory;
        BIKE_L1_performance_metrics_mean_short_message.after_encapsulation.resident_set_size += BIKE_L1_performance_metrics_short_message[i].after_encapsulation.resident_set_size;
        BIKE_L1_performance_metrics_mean_short_message.after_encapsulation.virtual_memory += BIKE_L1_performance_metrics_short_message[i].after_encapsulation.virtual_memory;
        BIKE_L1_performance_metrics_mean_short_message.after_decapsulation.resident_set_size += BIKE_L1_performance_metrics_short_message[i].after_decapsulation.resident_set_size;
        BIKE_L1_performance_metrics_mean_short_message.after_decapsulation.virtual_memory += BIKE_L1_performance_metrics_short_message[i].after_decapsulation.virtual_memory;
        BIKE_L1_performance_metrics_mean_short_message.after_encryption.resident_set_size += BIKE_L1_performance_metrics_short_message[i].after_encryption.resident_set_size;
        BIKE_L1_performance_metrics_mean_short_message.after_encryption.virtual_memory += BIKE_L1_performance_metrics_short_message[i].after_encryption.virtual_memory;
        BIKE_L1_performance_metrics_mean_short_message.after_decryption.resident_set_size += BIKE_L1_performance_metrics_short_message[i].after_decryption.resident_set_size;
        BIKE_L1_performance_metrics_mean_short_message.after_decryption.virtual_memory += BIKE_L1_performance_metrics_short_message[i].after_decryption.virtual_memory;
        BIKE_L1_performance_metrics_mean_short_message.after_all_operations.resident_set_size += BIKE_L1_performance_metrics_short_message[i].after_all_operations.resident_set_size;
        BIKE_L1_performance_metrics_mean_short_message.after_all_operations.virtual_memory += BIKE_L1_performance_metrics_short_message[i].after_all_operations.virtual_memory;
        // Medium message
        BIKE_L1_performance_metrics_mean_medium_message.keypair_generation_time += BIKE_L1_performance_metrics_medium_message[i].keypair_generation_time;
        BIKE_L1_performance_metrics_mean_medium_message.encapsulation_time += BIKE_L1_performance_metrics_medium_message[i].encapsulation_time;
        BIKE_L1_performance_metrics_mean_medium_message.decapsulation_time += BIKE_L1_performance_metrics_medium_message[i].decapsulation_time;
        BIKE_L1_performance_metrics_mean_medium_message.encryption_time += BIKE_L1_performance_metrics_medium_message[i].encryption_time;
        BIKE_L1_performance_metrics_mean_medium_message.decryption_time += BIKE_L1_performance_metrics_medium_message[i].decryption_time;
        BIKE_L1_performance_metrics_mean_medium_message.before_any_operations.resident_set_size += BIKE_L1_performance_metrics_medium_message[i].before_any_operations.resident_set_size;
        BIKE_L1_performance_metrics_mean_medium_message.before_any_operations.virtual_memory += BIKE_L1_performance_metrics_medium_message[i].before_any_operations.virtual_memory;
        BIKE_L1_performance_metrics_mean_medium_message.after_key_generation.resident_set_size += BIKE_L1_performance_metrics_medium_message[i].after_key_generation.resident_set_size;
        BIKE_L1_performance_metrics_mean_medium_message.after_key_generation.virtual_memory += BIKE_L1_performance_metrics_medium_message[i].after_key_generation.virtual_memory;
        BIKE_L1_performance_metrics_mean_medium_message.after_encapsulation.resident_set_size += BIKE_L1_performance_metrics_medium_message[i].after_encapsulation.resident_set_size;
        BIKE_L1_performance_metrics_mean_medium_message.after_encapsulation.virtual_memory += BIKE_L1_performance_metrics_medium_message[i].after_encapsulation.virtual_memory;
        BIKE_L1_performance_metrics_mean_medium_message.after_decapsulation.resident_set_size += BIKE_L1_performance_metrics_medium_message[i].after_decapsulation.resident_set_size;
        BIKE_L1_performance_metrics_mean_medium_message.after_decapsulation.virtual_memory += BIKE_L1_performance_metrics_medium_message[i].after_decapsulation.virtual_memory;
        BIKE_L1_performance_metrics_mean_medium_message.after_encryption.resident_set_size += BIKE_L1_performance_metrics_medium_message[i].after_encryption.resident_set_size;
        BIKE_L1_performance_metrics_mean_medium_message.after_encryption.virtual_memory += BIKE_L1_performance_metrics_medium_message[i].after_encryption.virtual_memory;
        BIKE_L1_performance_metrics_mean_medium_message.after_decryption.resident_set_size += BIKE_L1_performance_metrics_medium_message[i].after_decryption.resident_set_size;
        BIKE_L1_performance_metrics_mean_medium_message.after_decryption.virtual_memory += BIKE_L1_performance_metrics_medium_message[i].after_decryption.virtual_memory;
        BIKE_L1_performance_metrics_mean_medium_message.after_all_operations.resident_set_size += BIKE_L1_performance_metrics_medium_message[i].after_all_operations.resident_set_size;
        BIKE_L1_performance_metrics_mean_medium_message.after_all_operations.virtual_memory += BIKE_L1_performance_metrics_medium_message[i].after_all_operations.virtual_memory;
        // Long message
        BIKE_L1_performance_metrics_mean_long_message.keypair_generation_time += BIKE_L1_performance_metrics_long_message[i].keypair_generation_time;
        BIKE_L1_performance_metrics_mean_long_message.encapsulation_time += BIKE_L1_performance_metrics_long_message[i].encapsulation_time;
        BIKE_L1_performance_metrics_mean_long_message.decapsulation_time += BIKE_L1_performance_metrics_long_message[i].decapsulation_time;
        BIKE_L1_performance_metrics_mean_long_message.encryption_time += BIKE_L1_performance_metrics_long_message[i].encryption_time;
        BIKE_L1_performance_metrics_mean_long_message.decryption_time += BIKE_L1_performance_metrics_long_message[i].decryption_time;
        BIKE_L1_performance_metrics_mean_long_message.before_any_operations.resident_set_size += BIKE_L1_performance_metrics_long_message[i].before_any_operations.resident_set_size;
        BIKE_L1_performance_metrics_mean_long_message.before_any_operations.virtual_memory += BIKE_L1_performance_metrics_long_message[i].before_any_operations.virtual_memory;
        BIKE_L1_performance_metrics_mean_long_message.after_key_generation.resident_set_size += BIKE_L1_performance_metrics_long_message[i].after_key_generation.resident_set_size;
        BIKE_L1_performance_metrics_mean_long_message.after_key_generation.virtual_memory += BIKE_L1_performance_metrics_long_message[i].after_key_generation.virtual_memory;
        BIKE_L1_performance_metrics_mean_long_message.after_encapsulation.resident_set_size += BIKE_L1_performance_metrics_long_message[i].after_encapsulation.resident_set_size;
        BIKE_L1_performance_metrics_mean_long_message.after_encapsulation.virtual_memory += BIKE_L1_performance_metrics_long_message[i].after_encapsulation.virtual_memory;
        BIKE_L1_performance_metrics_mean_long_message.after_decapsulation.resident_set_size += BIKE_L1_performance_metrics_long_message[i].after_decapsulation.resident_set_size;
        BIKE_L1_performance_metrics_mean_long_message.after_decapsulation.virtual_memory += BIKE_L1_performance_metrics_long_message[i].after_decapsulation.virtual_memory;
        BIKE_L1_performance_metrics_mean_long_message.after_encryption.resident_set_size += BIKE_L1_performance_metrics_long_message[i].after_encryption.resident_set_size;
        BIKE_L1_performance_metrics_mean_long_message.after_encryption.virtual_memory += BIKE_L1_performance_metrics_long_message[i].after_encryption.virtual_memory;
        BIKE_L1_performance_metrics_mean_long_message.after_decryption.resident_set_size += BIKE_L1_performance_metrics_long_message[i].after_decryption.resident_set_size;
        BIKE_L1_performance_metrics_mean_long_message.after_decryption.virtual_memory += BIKE_L1_performance_metrics_long_message[i].after_decryption.virtual_memory;
        BIKE_L1_performance_metrics_mean_long_message.after_all_operations.resident_set_size += BIKE_L1_performance_metrics_long_message[i].after_all_operations.resident_set_size;
        BIKE_L1_performance_metrics_mean_long_message.after_all_operations.virtual_memory += BIKE_L1_performance_metrics_long_message[i].after_all_operations.virtual_memory;

        // BIKE-L3
        // Short message
        BIKE_L3_performance_metrics_mean_short_message.keypair_generation_time += BIKE_L3_performance_metrics_short_mesage[i].keypair_generation_time;
        BIKE_L3_performance_metrics_mean_short_message.encapsulation_time += BIKE_L3_performance_metrics_short_mesage[i].encapsulation_time;
        BIKE_L3_performance_metrics_mean_short_message.decapsulation_time += BIKE_L3_performance_metrics_short_mesage[i].decapsulation_time;
        BIKE_L3_performance_metrics_mean_short_message.encryption_time += BIKE_L3_performance_metrics_short_mesage[i].encryption_time;
        BIKE_L3_performance_metrics_mean_short_message.decryption_time += BIKE_L3_performance_metrics_short_mesage[i].decryption_time;
        BIKE_L3_performance_metrics_mean_short_message.before_any_operations.resident_set_size += BIKE_L3_performance_metrics_short_mesage[i].before_any_operations.resident_set_size;
        BIKE_L3_performance_metrics_mean_short_message.before_any_operations.virtual_memory += BIKE_L3_performance_metrics_short_mesage[i].before_any_operations.virtual_memory;
        BIKE_L3_performance_metrics_mean_short_message.after_key_generation.resident_set_size += BIKE_L3_performance_metrics_short_mesage[i].after_key_generation.resident_set_size;
        BIKE_L3_performance_metrics_mean_short_message.after_key_generation.virtual_memory += BIKE_L3_performance_metrics_short_mesage[i].after_key_generation.virtual_memory;
        BIKE_L3_performance_metrics_mean_short_message.after_encapsulation.resident_set_size += BIKE_L3_performance_metrics_short_mesage[i].after_encapsulation.resident_set_size;
        BIKE_L3_performance_metrics_mean_short_message.after_encapsulation.virtual_memory += BIKE_L3_performance_metrics_short_mesage[i].after_encapsulation.virtual_memory;
        BIKE_L3_performance_metrics_mean_short_message.after_decapsulation.resident_set_size += BIKE_L3_performance_metrics_short_mesage[i].after_decapsulation.resident_set_size;
        BIKE_L3_performance_metrics_mean_short_message.after_decapsulation.virtual_memory += BIKE_L3_performance_metrics_short_mesage[i].after_decapsulation.virtual_memory;
        BIKE_L3_performance_metrics_mean_short_message.after_encryption.resident_set_size += BIKE_L3_performance_metrics_short_mesage[i].after_encryption.resident_set_size;
        BIKE_L3_performance_metrics_mean_short_message.after_encryption.virtual_memory += BIKE_L3_performance_metrics_short_mesage[i].after_encryption.virtual_memory;
        BIKE_L3_performance_metrics_mean_short_message.after_decryption.resident_set_size += BIKE_L3_performance_metrics_short_mesage[i].after_decryption.resident_set_size;
        BIKE_L3_performance_metrics_mean_short_message.after_decryption.virtual_memory += BIKE_L3_performance_metrics_short_mesage[i].after_decryption.virtual_memory;
        BIKE_L3_performance_metrics_mean_short_message.after_all_operations.resident_set_size += BIKE_L3_performance_metrics_short_mesage[i].after_all_operations.resident_set_size;
        BIKE_L3_performance_metrics_mean_short_message.after_all_operations.virtual_memory += BIKE_L3_performance_metrics_short_mesage[i].after_all_operations.virtual_memory;
        // Medium message
        BIKE_L3_performance_metrics_mean_medium_message.keypair_generation_time += BIKE_L3_performance_metrics_medium_message[i].keypair_generation_time;
        BIKE_L3_performance_metrics_mean_medium_message.encapsulation_time += BIKE_L3_performance_metrics_medium_message[i].encapsulation_time;
        BIKE_L3_performance_metrics_mean_medium_message.decapsulation_time += BIKE_L3_performance_metrics_medium_message[i].decapsulation_time;
        BIKE_L3_performance_metrics_mean_medium_message.encryption_time += BIKE_L3_performance_metrics_medium_message[i].encryption_time;
        BIKE_L3_performance_metrics_mean_medium_message.decryption_time += BIKE_L3_performance_metrics_medium_message[i].decryption_time;
        BIKE_L3_performance_metrics_mean_medium_message.before_any_operations.resident_set_size += BIKE_L3_performance_metrics_medium_message[i].before_any_operations.resident_set_size;
        BIKE_L3_performance_metrics_mean_medium_message.before_any_operations.virtual_memory += BIKE_L3_performance_metrics_medium_message[i].before_any_operations.virtual_memory;
        BIKE_L3_performance_metrics_mean_medium_message.after_key_generation.resident_set_size += BIKE_L3_performance_metrics_medium_message[i].after_key_generation.resident_set_size;
        BIKE_L3_performance_metrics_mean_medium_message.after_key_generation.virtual_memory += BIKE_L3_performance_metrics_medium_message[i].after_key_generation.virtual_memory;
        BIKE_L3_performance_metrics_mean_medium_message.after_encapsulation.resident_set_size += BIKE_L3_performance_metrics_medium_message[i].after_encapsulation.resident_set_size;
        BIKE_L3_performance_metrics_mean_medium_message.after_encapsulation.virtual_memory += BIKE_L3_performance_metrics_medium_message[i].after_encapsulation.virtual_memory;
        BIKE_L3_performance_metrics_mean_medium_message.after_decapsulation.resident_set_size += BIKE_L3_performance_metrics_medium_message[i].after_decapsulation.resident_set_size;
        BIKE_L3_performance_metrics_mean_medium_message.after_decapsulation.virtual_memory += BIKE_L3_performance_metrics_medium_message[i].after_decapsulation.virtual_memory;
        BIKE_L3_performance_metrics_mean_medium_message.after_encryption.resident_set_size += BIKE_L3_performance_metrics_medium_message[i].after_encryption.resident_set_size;
        BIKE_L3_performance_metrics_mean_medium_message.after_encryption.virtual_memory += BIKE_L3_performance_metrics_medium_message[i].after_encryption.virtual_memory;
        BIKE_L3_performance_metrics_mean_medium_message.after_decryption.resident_set_size += BIKE_L3_performance_metrics_medium_message[i].after_decryption.resident_set_size;
        BIKE_L3_performance_metrics_mean_medium_message.after_decryption.virtual_memory += BIKE_L3_performance_metrics_medium_message[i].after_decryption.virtual_memory;
        BIKE_L3_performance_metrics_mean_medium_message.after_all_operations.resident_set_size += BIKE_L3_performance_metrics_medium_message[i].after_all_operations.resident_set_size;
        BIKE_L3_performance_metrics_mean_medium_message.after_all_operations.virtual_memory += BIKE_L3_performance_metrics_medium_message[i].after_all_operations.virtual_memory;
        // Long message
        BIKE_L3_performance_metrics_mean_long_message.keypair_generation_time += BIKE_L3_performance_metrics_long_message[i].keypair_generation_time;
        BIKE_L3_performance_metrics_mean_long_message.encapsulation_time += BIKE_L3_performance_metrics_long_message[i].encapsulation_time;
        BIKE_L3_performance_metrics_mean_long_message.decapsulation_time += BIKE_L3_performance_metrics_long_message[i].decapsulation_time;
        BIKE_L3_performance_metrics_mean_long_message.encryption_time += BIKE_L3_performance_metrics_long_message[i].encryption_time;
        BIKE_L3_performance_metrics_mean_long_message.decryption_time += BIKE_L3_performance_metrics_long_message[i].decryption_time;
        BIKE_L3_performance_metrics_mean_long_message.before_any_operations.resident_set_size += BIKE_L3_performance_metrics_long_message[i].before_any_operations.resident_set_size;
        BIKE_L3_performance_metrics_mean_long_message.before_any_operations.virtual_memory += BIKE_L3_performance_metrics_long_message[i].before_any_operations.virtual_memory;
        BIKE_L3_performance_metrics_mean_long_message.after_key_generation.resident_set_size += BIKE_L3_performance_metrics_long_message[i].after_key_generation.resident_set_size;
        BIKE_L3_performance_metrics_mean_long_message.after_key_generation.virtual_memory += BIKE_L3_performance_metrics_long_message[i].after_key_generation.virtual_memory;
        BIKE_L3_performance_metrics_mean_long_message.after_encapsulation.resident_set_size += BIKE_L3_performance_metrics_long_message[i].after_encapsulation.resident_set_size;
        BIKE_L3_performance_metrics_mean_long_message.after_encapsulation.virtual_memory += BIKE_L3_performance_metrics_long_message[i].after_encapsulation.virtual_memory;
        BIKE_L3_performance_metrics_mean_long_message.after_decapsulation.resident_set_size += BIKE_L3_performance_metrics_long_message[i].after_decapsulation.resident_set_size;
        BIKE_L3_performance_metrics_mean_long_message.after_decapsulation.virtual_memory += BIKE_L3_performance_metrics_long_message[i].after_decapsulation.virtual_memory;
        BIKE_L3_performance_metrics_mean_long_message.after_encryption.resident_set_size += BIKE_L3_performance_metrics_long_message[i].after_encryption.resident_set_size;
        BIKE_L3_performance_metrics_mean_long_message.after_encryption.virtual_memory += BIKE_L3_performance_metrics_long_message[i].after_encryption.virtual_memory;
        BIKE_L3_performance_metrics_mean_long_message.after_decryption.resident_set_size += BIKE_L3_performance_metrics_long_message[i].after_decryption.resident_set_size;
        BIKE_L3_performance_metrics_mean_long_message.after_decryption.virtual_memory += BIKE_L3_performance_metrics_long_message[i].after_decryption.virtual_memory;
        BIKE_L3_performance_metrics_mean_long_message.after_all_operations.resident_set_size += BIKE_L3_performance_metrics_long_message[i].after_all_operations.resident_set_size;
        BIKE_L3_performance_metrics_mean_long_message.after_all_operations.virtual_memory += BIKE_L3_performance_metrics_long_message[i].after_all_operations.virtual_memory;

        // BIKE-L5
        // Short message
        BIKE_L5_performance_metrics_mean_short_message.keypair_generation_time += BIKE_L5_performance_metrics_short_mesage[i].keypair_generation_time;
        BIKE_L5_performance_metrics_mean_short_message.encapsulation_time += BIKE_L5_performance_metrics_short_mesage[i].encapsulation_time;
        BIKE_L5_performance_metrics_mean_short_message.decapsulation_time += BIKE_L5_performance_metrics_short_mesage[i].decapsulation_time;
        BIKE_L5_performance_metrics_mean_short_message.encryption_time += BIKE_L5_performance_metrics_short_mesage[i].encryption_time;
        BIKE_L5_performance_metrics_mean_short_message.decryption_time += BIKE_L5_performance_metrics_short_mesage[i].decryption_time;
        BIKE_L5_performance_metrics_mean_short_message.before_any_operations.resident_set_size += BIKE_L5_performance_metrics_short_mesage[i].before_any_operations.resident_set_size;
        BIKE_L5_performance_metrics_mean_short_message.before_any_operations.virtual_memory += BIKE_L5_performance_metrics_short_mesage[i].before_any_operations.virtual_memory;
        BIKE_L5_performance_metrics_mean_short_message.after_key_generation.resident_set_size += BIKE_L5_performance_metrics_short_mesage[i].after_key_generation.resident_set_size;
        BIKE_L5_performance_metrics_mean_short_message.after_key_generation.virtual_memory += BIKE_L5_performance_metrics_short_mesage[i].after_key_generation.virtual_memory;
        BIKE_L5_performance_metrics_mean_short_message.after_encapsulation.resident_set_size += BIKE_L5_performance_metrics_short_mesage[i].after_encapsulation.resident_set_size;
        BIKE_L5_performance_metrics_mean_short_message.after_encapsulation.virtual_memory += BIKE_L5_performance_metrics_short_mesage[i].after_encapsulation.virtual_memory;
        BIKE_L5_performance_metrics_mean_short_message.after_decapsulation.resident_set_size += BIKE_L5_performance_metrics_short_mesage[i].after_decapsulation.resident_set_size;
        BIKE_L5_performance_metrics_mean_short_message.after_decapsulation.virtual_memory += BIKE_L5_performance_metrics_short_mesage[i].after_decapsulation.virtual_memory;
        BIKE_L5_performance_metrics_mean_short_message.after_encryption.resident_set_size += BIKE_L5_performance_metrics_short_mesage[i].after_encryption.resident_set_size;
        BIKE_L5_performance_metrics_mean_short_message.after_encryption.virtual_memory += BIKE_L5_performance_metrics_short_mesage[i].after_encryption.virtual_memory;
        BIKE_L5_performance_metrics_mean_short_message.after_decryption.resident_set_size += BIKE_L5_performance_metrics_short_mesage[i].after_decryption.resident_set_size;
        BIKE_L5_performance_metrics_mean_short_message.after_decryption.virtual_memory += BIKE_L5_performance_metrics_short_mesage[i].after_decryption.virtual_memory;
        BIKE_L5_performance_metrics_mean_short_message.after_all_operations.resident_set_size += BIKE_L5_performance_metrics_short_mesage[i].after_all_operations.resident_set_size;
        BIKE_L5_performance_metrics_mean_short_message.after_all_operations.virtual_memory += BIKE_L5_performance_metrics_short_mesage[i].after_all_operations.virtual_memory;
        // Medium message
        BIKE_L5_performance_metrics_mean_medium_message.keypair_generation_time += BIKE_L5_performance_metrics_medium_message[i].keypair_generation_time;
        BIKE_L5_performance_metrics_mean_medium_message.encapsulation_time += BIKE_L5_performance_metrics_medium_message[i].encapsulation_time;
        BIKE_L5_performance_metrics_mean_medium_message.decapsulation_time += BIKE_L5_performance_metrics_medium_message[i].decapsulation_time;
        BIKE_L5_performance_metrics_mean_medium_message.encryption_time += BIKE_L5_performance_metrics_medium_message[i].encryption_time;
        BIKE_L5_performance_metrics_mean_medium_message.decryption_time += BIKE_L5_performance_metrics_medium_message[i].decryption_time;
        BIKE_L5_performance_metrics_mean_medium_message.before_any_operations.resident_set_size += BIKE_L5_performance_metrics_medium_message[i].before_any_operations.resident_set_size;
        BIKE_L5_performance_metrics_mean_medium_message.before_any_operations.virtual_memory += BIKE_L5_performance_metrics_medium_message[i].before_any_operations.virtual_memory;
        BIKE_L5_performance_metrics_mean_medium_message.after_key_generation.resident_set_size += BIKE_L5_performance_metrics_medium_message[i].after_key_generation.resident_set_size;
        BIKE_L5_performance_metrics_mean_medium_message.after_key_generation.virtual_memory += BIKE_L5_performance_metrics_medium_message[i].after_key_generation.virtual_memory;
        BIKE_L5_performance_metrics_mean_medium_message.after_encapsulation.resident_set_size += BIKE_L5_performance_metrics_medium_message[i].after_encapsulation.resident_set_size;
        BIKE_L5_performance_metrics_mean_medium_message.after_encapsulation.virtual_memory += BIKE_L5_performance_metrics_medium_message[i].after_encapsulation.virtual_memory;
        BIKE_L5_performance_metrics_mean_medium_message.after_decapsulation.resident_set_size += BIKE_L5_performance_metrics_medium_message[i].after_decapsulation.resident_set_size;
        BIKE_L5_performance_metrics_mean_medium_message.after_decapsulation.virtual_memory += BIKE_L5_performance_metrics_medium_message[i].after_decapsulation.virtual_memory;
        BIKE_L5_performance_metrics_mean_medium_message.after_encryption.resident_set_size += BIKE_L5_performance_metrics_medium_message[i].after_encryption.resident_set_size;
        BIKE_L5_performance_metrics_mean_medium_message.after_encryption.virtual_memory += BIKE_L5_performance_metrics_medium_message[i].after_encryption.virtual_memory;
        BIKE_L5_performance_metrics_mean_medium_message.after_decryption.resident_set_size += BIKE_L5_performance_metrics_medium_message[i].after_decryption.resident_set_size;
        BIKE_L5_performance_metrics_mean_medium_message.after_decryption.virtual_memory += BIKE_L5_performance_metrics_medium_message[i].after_decryption.virtual_memory;
        BIKE_L5_performance_metrics_mean_medium_message.after_all_operations.resident_set_size += BIKE_L5_performance_metrics_medium_message[i].after_all_operations.resident_set_size;
        BIKE_L5_performance_metrics_mean_medium_message.after_all_operations.virtual_memory += BIKE_L5_performance_metrics_medium_message[i].after_all_operations.virtual_memory;
        // Long message
        BIKE_L5_performance_metrics_mean_long_message.keypair_generation_time += BIKE_L5_performance_metrics_long_message[i].keypair_generation_time;
        BIKE_L5_performance_metrics_mean_long_message.encapsulation_time += BIKE_L5_performance_metrics_long_message[i].encapsulation_time;
        BIKE_L5_performance_metrics_mean_long_message.decapsulation_time += BIKE_L5_performance_metrics_long_message[i].decapsulation_time;
        BIKE_L5_performance_metrics_mean_long_message.encryption_time += BIKE_L5_performance_metrics_long_message[i].encryption_time;
        BIKE_L5_performance_metrics_mean_long_message.decryption_time += BIKE_L5_performance_metrics_long_message[i].decryption_time;
        BIKE_L5_performance_metrics_mean_long_message.before_any_operations.resident_set_size += BIKE_L5_performance_metrics_long_message[i].before_any_operations.resident_set_size;
        BIKE_L5_performance_metrics_mean_long_message.before_any_operations.virtual_memory += BIKE_L5_performance_metrics_long_message[i].before_any_operations.virtual_memory;
        BIKE_L5_performance_metrics_mean_long_message.after_key_generation.resident_set_size += BIKE_L5_performance_metrics_long_message[i].after_key_generation.resident_set_size;
        BIKE_L5_performance_metrics_mean_long_message.after_key_generation.virtual_memory += BIKE_L5_performance_metrics_long_message[i].after_key_generation.virtual_memory;
        BIKE_L5_performance_metrics_mean_long_message.after_encapsulation.resident_set_size += BIKE_L5_performance_metrics_long_message[i].after_encapsulation.resident_set_size;
        BIKE_L5_performance_metrics_mean_long_message.after_encapsulation.virtual_memory += BIKE_L5_performance_metrics_long_message[i].after_encapsulation.virtual_memory;
        BIKE_L5_performance_metrics_mean_long_message.after_decapsulation.resident_set_size += BIKE_L5_performance_metrics_long_message[i].after_decapsulation.resident_set_size;
        BIKE_L5_performance_metrics_mean_long_message.after_decapsulation.virtual_memory += BIKE_L5_performance_metrics_long_message[i].after_decapsulation.virtual_memory;
        BIKE_L5_performance_metrics_mean_long_message.after_encryption.resident_set_size += BIKE_L5_performance_metrics_long_message[i].after_encryption.resident_set_size;
        BIKE_L5_performance_metrics_mean_long_message.after_encryption.virtual_memory += BIKE_L5_performance_metrics_long_message[i].after_encryption.virtual_memory;
        BIKE_L5_performance_metrics_mean_long_message.after_decryption.resident_set_size += BIKE_L5_performance_metrics_long_message[i].after_decryption.resident_set_size;
        BIKE_L5_performance_metrics_mean_long_message.after_decryption.virtual_memory += BIKE_L5_performance_metrics_long_message[i].after_decryption.virtual_memory;
        BIKE_L5_performance_metrics_mean_long_message.after_all_operations.resident_set_size += BIKE_L5_performance_metrics_long_message[i].after_all_operations.resident_set_size;
        BIKE_L5_performance_metrics_mean_long_message.after_all_operations.virtual_memory += BIKE_L5_performance_metrics_long_message[i].after_all_operations.virtual_memory;

    }

    // Calculate the means
    BIKE_L1_performance_metrics_mean_short_message.keypair_generation_time /= MEASURE_ITERATIONS;
    BIKE_L1_performance_metrics_mean_short_message.encapsulation_time /= MEASURE_ITERATIONS;
    BIKE_L1_performance_metrics_mean_short_message.decapsulation_time /= MEASURE_ITERATIONS;
    BIKE_L1_performance_metrics_mean_short_message.encryption_time /= MEASURE_ITERATIONS;
    BIKE_L1_performance_metrics_mean_short_message.decryption_time /= MEASURE_ITERATIONS;
    BIKE_L1_performance_metrics_mean_short_message.before_any_operations.resident_set_size /= MEASURE_ITERATIONS;
    BIKE_L1_performance_metrics_mean_short_message.before_any_operations.virtual_memory /= MEASURE_ITERATIONS;
    BIKE_L1_performance_metrics_mean_short_message.after_key_generation.resident_set_size /= MEASURE_ITERATIONS;
    BIKE_L1_performance_metrics_mean_short_message.after_key_generation.virtual_memory /= MEASURE_ITERATIONS;
    BIKE_L1_performance_metrics_mean_short_message.after_encapsulation.resident_set_size /= MEASURE_ITERATIONS;
    BIKE_L1_performance_metrics_mean_short_message.after_encapsulation.virtual_memory /= MEASURE_ITERATIONS;
    BIKE_L1_performance_metrics_mean_short_message.after_decapsulation.resident_set_size /= MEASURE_ITERATIONS;
    BIKE_L1_performance_metrics_mean_short_message.after_decapsulation.virtual_memory /= MEASURE_ITERATIONS;
    BIKE_L1_performance_metrics_mean_short_message.after_encryption.resident_set_size /= MEASURE_ITERATIONS;
    BIKE_L1_performance_metrics_mean_short_message.after_encryption.virtual_memory /= MEASURE_ITERATIONS;
    BIKE_L1_performance_metrics_mean_short_message.after_decryption.resident_set_size /= MEASURE_ITERATIONS;
    BIKE_L1_performance_metrics_mean_short_message.after_decryption.virtual_memory /= MEASURE_ITERATIONS;
    BIKE_L1_performance_metrics_mean_short_message.after_all_operations.resident_set_size /= MEASURE_ITERATIONS;
    BIKE_L1_performance_metrics_mean_short_message.after_all_operations.virtual_memory /= MEASURE_ITERATIONS;

    BIKE_L1_performance_metrics_mean_medium_message.keypair_generation_time /= MEASURE_ITERATIONS;
    BIKE_L1_performance_metrics_mean_medium_message.encapsulation_time /= MEASURE_ITERATIONS;
    BIKE_L1_performance_metrics_mean_medium_message.decapsulation_time /= MEASURE_ITERATIONS;
    BIKE_L1_performance_metrics_mean_medium_message.encryption_time /= MEASURE_ITERATIONS;
    BIKE_L1_performance_metrics_mean_medium_message.decryption_time /= MEASURE_ITERATIONS;
    BIKE_L1_performance_metrics_mean_medium_message.before_any_operations.resident_set_size /= MEASURE_ITERATIONS;
    BIKE_L1_performance_metrics_mean_medium_message.before_any_operations.virtual_memory /= MEASURE_ITERATIONS;
    BIKE_L1_performance_metrics_mean_medium_message.after_key_generation.resident_set_size /= MEASURE_ITERATIONS;
    BIKE_L1_performance_metrics_mean_medium_message.after_key_generation.virtual_memory /= MEASURE_ITERATIONS;
    BIKE_L1_performance_metrics_mean_medium_message.after_encapsulation.resident_set_size /= MEASURE_ITERATIONS;
    BIKE_L1_performance_metrics_mean_medium_message.after_encapsulation.virtual_memory /= MEASURE_ITERATIONS;
    BIKE_L1_performance_metrics_mean_medium_message.after_decapsulation.resident_set_size /= MEASURE_ITERATIONS;
    BIKE_L1_performance_metrics_mean_medium_message.after_decapsulation.virtual_memory /= MEASURE_ITERATIONS;
    BIKE_L1_performance_metrics_mean_medium_message.after_encryption.resident_set_size /= MEASURE_ITERATIONS;
    BIKE_L1_performance_metrics_mean_medium_message.after_encryption.virtual_memory /= MEASURE_ITERATIONS;
    BIKE_L1_performance_metrics_mean_medium_message.after_decryption.resident_set_size /= MEASURE_ITERATIONS;
    BIKE_L1_performance_metrics_mean_medium_message.after_decryption.virtual_memory /= MEASURE_ITERATIONS;
    BIKE_L1_performance_metrics_mean_medium_message.after_all_operations.resident_set_size /= MEASURE_ITERATIONS;
    BIKE_L1_performance_metrics_mean_medium_message.after_all_operations.virtual_memory /= MEASURE_ITERATIONS;

    BIKE_L1_performance_metrics_mean_long_message.keypair_generation_time /= MEASURE_ITERATIONS;
    BIKE_L1_performance_metrics_mean_long_message.encapsulation_time /= MEASURE_ITERATIONS;
    BIKE_L1_performance_metrics_mean_long_message.decapsulation_time /= MEASURE_ITERATIONS;
    BIKE_L1_performance_metrics_mean_long_message.encryption_time /= MEASURE_ITERATIONS;
    BIKE_L1_performance_metrics_mean_long_message.decryption_time /= MEASURE_ITERATIONS;
    BIKE_L1_performance_metrics_mean_long_message.before_any_operations.resident_set_size /= MEASURE_ITERATIONS;
    BIKE_L1_performance_metrics_mean_long_message.before_any_operations.virtual_memory /= MEASURE_ITERATIONS;
    BIKE_L1_performance_metrics_mean_long_message.after_key_generation.resident_set_size /= MEASURE_ITERATIONS;
    BIKE_L1_performance_metrics_mean_long_message.after_key_generation.virtual_memory /= MEASURE_ITERATIONS;
    BIKE_L1_performance_metrics_mean_long_message.after_encapsulation.resident_set_size /= MEASURE_ITERATIONS;
    BIKE_L1_performance_metrics_mean_long_message.after_encapsulation.virtual_memory /= MEASURE_ITERATIONS;
    BIKE_L1_performance_metrics_mean_long_message.after_decapsulation.resident_set_size /= MEASURE_ITERATIONS;
    BIKE_L1_performance_metrics_mean_long_message.after_decapsulation.virtual_memory /= MEASURE_ITERATIONS;
    BIKE_L1_performance_metrics_mean_long_message.after_encryption.resident_set_size /= MEASURE_ITERATIONS;
    BIKE_L1_performance_metrics_mean_long_message.after_encryption.virtual_memory /= MEASURE_ITERATIONS;
    BIKE_L1_performance_metrics_mean_long_message.after_decryption.resident_set_size /= MEASURE_ITERATIONS;
    BIKE_L1_performance_metrics_mean_long_message.after_decryption.virtual_memory /= MEASURE_ITERATIONS;
    BIKE_L1_performance_metrics_mean_long_message.after_all_operations.resident_set_size /= MEASURE_ITERATIONS;
    BIKE_L1_performance_metrics_mean_long_message.after_all_operations.virtual_memory /= MEASURE_ITERATIONS;

    BIKE_L3_performance_metrics_mean_short_message.keypair_generation_time /= MEASURE_ITERATIONS;
    BIKE_L3_performance_metrics_mean_short_message.encapsulation_time /= MEASURE_ITERATIONS;
    BIKE_L3_performance_metrics_mean_short_message.decapsulation_time /= MEASURE_ITERATIONS;
    BIKE_L3_performance_metrics_mean_short_message.encryption_time /= MEASURE_ITERATIONS;
    BIKE_L3_performance_metrics_mean_short_message.decryption_time /= MEASURE_ITERATIONS;
    BIKE_L3_performance_metrics_mean_short_message.before_any_operations.resident_set_size /= MEASURE_ITERATIONS;
    BIKE_L3_performance_metrics_mean_short_message.before_any_operations.virtual_memory /= MEASURE_ITERATIONS;
    BIKE_L3_performance_metrics_mean_short_message.after_key_generation.resident_set_size /= MEASURE_ITERATIONS;
    BIKE_L3_performance_metrics_mean_short_message.after_key_generation.virtual_memory /= MEASURE_ITERATIONS;
    BIKE_L3_performance_metrics_mean_short_message.after_encapsulation.resident_set_size /= MEASURE_ITERATIONS;
    BIKE_L3_performance_metrics_mean_short_message.after_encapsulation.virtual_memory /= MEASURE_ITERATIONS;
    BIKE_L3_performance_metrics_mean_short_message.after_decapsulation.resident_set_size /= MEASURE_ITERATIONS;
    BIKE_L3_performance_metrics_mean_short_message.after_decapsulation.virtual_memory /= MEASURE_ITERATIONS;
    BIKE_L3_performance_metrics_mean_short_message.after_encryption.resident_set_size /= MEASURE_ITERATIONS;
    BIKE_L3_performance_metrics_mean_short_message.after_encryption.virtual_memory /= MEASURE_ITERATIONS;
    BIKE_L3_performance_metrics_mean_short_message.after_decryption.resident_set_size /= MEASURE_ITERATIONS;
    BIKE_L3_performance_metrics_mean_short_message.after_decryption.virtual_memory /= MEASURE_ITERATIONS;
    BIKE_L3_performance_metrics_mean_short_message.after_all_operations.resident_set_size /= MEASURE_ITERATIONS;
    BIKE_L3_performance_metrics_mean_short_message.after_all_operations.virtual_memory /= MEASURE_ITERATIONS;

    BIKE_L3_performance_metrics_mean_medium_message.keypair_generation_time /= MEASURE_ITERATIONS;
    BIKE_L3_performance_metrics_mean_medium_message.encapsulation_time /= MEASURE_ITERATIONS;
    BIKE_L3_performance_metrics_mean_medium_message.decapsulation_time /= MEASURE_ITERATIONS;
    BIKE_L3_performance_metrics_mean_medium_message.encryption_time /= MEASURE_ITERATIONS;
    BIKE_L3_performance_metrics_mean_medium_message.decryption_time /= MEASURE_ITERATIONS;
    BIKE_L3_performance_metrics_mean_medium_message.before_any_operations.resident_set_size /= MEASURE_ITERATIONS;
    BIKE_L3_performance_metrics_mean_medium_message.before_any_operations.virtual_memory /= MEASURE_ITERATIONS;
    BIKE_L3_performance_metrics_mean_medium_message.after_key_generation.resident_set_size /= MEASURE_ITERATIONS;
    BIKE_L3_performance_metrics_mean_medium_message.after_key_generation.virtual_memory /= MEASURE_ITERATIONS;
    BIKE_L3_performance_metrics_mean_medium_message.after_encapsulation.resident_set_size /= MEASURE_ITERATIONS;
    BIKE_L3_performance_metrics_mean_medium_message.after_encapsulation.virtual_memory /= MEASURE_ITERATIONS;
    BIKE_L3_performance_metrics_mean_medium_message.after_decapsulation.resident_set_size /= MEASURE_ITERATIONS;
    BIKE_L3_performance_metrics_mean_medium_message.after_decapsulation.virtual_memory /= MEASURE_ITERATIONS;
    BIKE_L3_performance_metrics_mean_medium_message.after_encryption.resident_set_size /= MEASURE_ITERATIONS;
    BIKE_L3_performance_metrics_mean_medium_message.after_encryption.virtual_memory /= MEASURE_ITERATIONS;
    BIKE_L3_performance_metrics_mean_medium_message.after_decryption.resident_set_size /= MEASURE_ITERATIONS;
    BIKE_L3_performance_metrics_mean_medium_message.after_decryption.virtual_memory /= MEASURE_ITERATIONS;
    BIKE_L3_performance_metrics_mean_medium_message.after_all_operations.resident_set_size /= MEASURE_ITERATIONS;
    BIKE_L3_performance_metrics_mean_medium_message.after_all_operations.virtual_memory /= MEASURE_ITERATIONS;

    BIKE_L3_performance_metrics_mean_long_message.keypair_generation_time /= MEASURE_ITERATIONS;
    BIKE_L3_performance_metrics_mean_long_message.encapsulation_time /= MEASURE_ITERATIONS;
    BIKE_L3_performance_metrics_mean_long_message.decapsulation_time /= MEASURE_ITERATIONS;
    BIKE_L3_performance_metrics_mean_long_message.encryption_time /= MEASURE_ITERATIONS;
    BIKE_L3_performance_metrics_mean_long_message.decryption_time /= MEASURE_ITERATIONS;
    BIKE_L3_performance_metrics_mean_long_message.before_any_operations.resident_set_size /= MEASURE_ITERATIONS;
    BIKE_L3_performance_metrics_mean_long_message.before_any_operations.virtual_memory /= MEASURE_ITERATIONS;
    BIKE_L3_performance_metrics_mean_long_message.after_key_generation.resident_set_size /= MEASURE_ITERATIONS;
    BIKE_L3_performance_metrics_mean_long_message.after_key_generation.virtual_memory /= MEASURE_ITERATIONS;
    BIKE_L3_performance_metrics_mean_long_message.after_encapsulation.resident_set_size /= MEASURE_ITERATIONS;
    BIKE_L3_performance_metrics_mean_long_message.after_encapsulation.virtual_memory /= MEASURE_ITERATIONS;
    BIKE_L3_performance_metrics_mean_long_message.after_decapsulation.resident_set_size /= MEASURE_ITERATIONS;
    BIKE_L3_performance_metrics_mean_long_message.after_decapsulation.virtual_memory /= MEASURE_ITERATIONS;
    BIKE_L3_performance_metrics_mean_long_message.after_encryption.resident_set_size /= MEASURE_ITERATIONS;
    BIKE_L3_performance_metrics_mean_long_message.after_encryption.virtual_memory /= MEASURE_ITERATIONS;
    BIKE_L3_performance_metrics_mean_long_message.after_decryption.resident_set_size /= MEASURE_ITERATIONS;
    BIKE_L3_performance_metrics_mean_long_message.after_decryption.virtual_memory /= MEASURE_ITERATIONS;
    BIKE_L3_performance_metrics_mean_long_message.after_all_operations.resident_set_size /= MEASURE_ITERATIONS;
    BIKE_L3_performance_metrics_mean_long_message.after_all_operations.virtual_memory /= MEASURE_ITERATIONS;

    BIKE_L5_performance_metrics_mean_short_message.keypair_generation_time /= MEASURE_ITERATIONS;
    BIKE_L5_performance_metrics_mean_short_message.encapsulation_time /= MEASURE_ITERATIONS;
    BIKE_L5_performance_metrics_mean_short_message.decapsulation_time /= MEASURE_ITERATIONS;
    BIKE_L5_performance_metrics_mean_short_message.encryption_time /= MEASURE_ITERATIONS;
    BIKE_L5_performance_metrics_mean_short_message.decryption_time /= MEASURE_ITERATIONS;
    BIKE_L5_performance_metrics_mean_short_message.before_any_operations.resident_set_size /= MEASURE_ITERATIONS;
    BIKE_L5_performance_metrics_mean_short_message.before_any_operations.virtual_memory /= MEASURE_ITERATIONS;
    BIKE_L5_performance_metrics_mean_short_message.after_key_generation.resident_set_size /= MEASURE_ITERATIONS;
    BIKE_L5_performance_metrics_mean_short_message.after_key_generation.virtual_memory /= MEASURE_ITERATIONS;
    BIKE_L5_performance_metrics_mean_short_message.after_encapsulation.resident_set_size /= MEASURE_ITERATIONS;
    BIKE_L5_performance_metrics_mean_short_message.after_encapsulation.virtual_memory /= MEASURE_ITERATIONS;
    BIKE_L5_performance_metrics_mean_short_message.after_decapsulation.resident_set_size /= MEASURE_ITERATIONS;
    BIKE_L5_performance_metrics_mean_short_message.after_decapsulation.virtual_memory /= MEASURE_ITERATIONS;
    BIKE_L5_performance_metrics_mean_short_message.after_encryption.resident_set_size /= MEASURE_ITERATIONS;
    BIKE_L5_performance_metrics_mean_short_message.after_encryption.virtual_memory /= MEASURE_ITERATIONS;
    BIKE_L5_performance_metrics_mean_short_message.after_decryption.resident_set_size /= MEASURE_ITERATIONS;
    BIKE_L5_performance_metrics_mean_short_message.after_decryption.virtual_memory /= MEASURE_ITERATIONS;
    BIKE_L5_performance_metrics_mean_short_message.after_all_operations.resident_set_size /= MEASURE_ITERATIONS;
    BIKE_L5_performance_metrics_mean_short_message.after_all_operations.virtual_memory /= MEASURE_ITERATIONS;

    BIKE_L5_performance_metrics_mean_medium_message.keypair_generation_time /= MEASURE_ITERATIONS;
    BIKE_L5_performance_metrics_mean_medium_message.encapsulation_time /= MEASURE_ITERATIONS;
    BIKE_L5_performance_metrics_mean_medium_message.decapsulation_time /= MEASURE_ITERATIONS;
    BIKE_L5_performance_metrics_mean_medium_message.encryption_time /= MEASURE_ITERATIONS;
    BIKE_L5_performance_metrics_mean_medium_message.decryption_time /= MEASURE_ITERATIONS;
    BIKE_L5_performance_metrics_mean_medium_message.before_any_operations.resident_set_size /= MEASURE_ITERATIONS;
    BIKE_L5_performance_metrics_mean_medium_message.before_any_operations.virtual_memory /= MEASURE_ITERATIONS;
    BIKE_L5_performance_metrics_mean_medium_message.after_key_generation.resident_set_size /= MEASURE_ITERATIONS;
    BIKE_L5_performance_metrics_mean_medium_message.after_key_generation.virtual_memory /= MEASURE_ITERATIONS;
    BIKE_L5_performance_metrics_mean_medium_message.after_encapsulation.resident_set_size /= MEASURE_ITERATIONS;
    BIKE_L5_performance_metrics_mean_medium_message.after_encapsulation.virtual_memory /= MEASURE_ITERATIONS;
    BIKE_L5_performance_metrics_mean_medium_message.after_decapsulation.resident_set_size /= MEASURE_ITERATIONS;
    BIKE_L5_performance_metrics_mean_medium_message.after_decapsulation.virtual_memory /= MEASURE_ITERATIONS;
    BIKE_L5_performance_metrics_mean_medium_message.after_encryption.resident_set_size /= MEASURE_ITERATIONS;
    BIKE_L5_performance_metrics_mean_medium_message.after_encryption.virtual_memory /= MEASURE_ITERATIONS;
    BIKE_L5_performance_metrics_mean_medium_message.after_decryption.resident_set_size /= MEASURE_ITERATIONS;
    BIKE_L5_performance_metrics_mean_medium_message.after_decryption.virtual_memory /= MEASURE_ITERATIONS;
    BIKE_L5_performance_metrics_mean_medium_message.after_all_operations.resident_set_size /= MEASURE_ITERATIONS;
    BIKE_L5_performance_metrics_mean_medium_message.after_all_operations.virtual_memory /= MEASURE_ITERATIONS;

    BIKE_L5_performance_metrics_mean_long_message.keypair_generation_time /= MEASURE_ITERATIONS;
    BIKE_L5_performance_metrics_mean_long_message.encapsulation_time /= MEASURE_ITERATIONS;
    BIKE_L5_performance_metrics_mean_long_message.decapsulation_time /= MEASURE_ITERATIONS;
    BIKE_L5_performance_metrics_mean_long_message.encryption_time /= MEASURE_ITERATIONS;
    BIKE_L5_performance_metrics_mean_long_message.decryption_time /= MEASURE_ITERATIONS;
    BIKE_L5_performance_metrics_mean_long_message.before_any_operations.resident_set_size /= MEASURE_ITERATIONS;
    BIKE_L5_performance_metrics_mean_long_message.before_any_operations.virtual_memory /= MEASURE_ITERATIONS;
    BIKE_L5_performance_metrics_mean_long_message.after_key_generation.resident_set_size /= MEASURE_ITERATIONS;
    BIKE_L5_performance_metrics_mean_long_message.after_key_generation.virtual_memory /= MEASURE_ITERATIONS;
    BIKE_L5_performance_metrics_mean_long_message.after_encapsulation.resident_set_size /= MEASURE_ITERATIONS;
    BIKE_L5_performance_metrics_mean_long_message.after_encapsulation.virtual_memory /= MEASURE_ITERATIONS;
    BIKE_L5_performance_metrics_mean_long_message.after_decapsulation.resident_set_size /= MEASURE_ITERATIONS;
    BIKE_L5_performance_metrics_mean_long_message.after_decapsulation.virtual_memory /= MEASURE_ITERATIONS;
    BIKE_L5_performance_metrics_mean_long_message.after_encryption.resident_set_size /= MEASURE_ITERATIONS;
    BIKE_L5_performance_metrics_mean_long_message.after_encryption.virtual_memory /= MEASURE_ITERATIONS;
    BIKE_L5_performance_metrics_mean_long_message.after_decryption.resident_set_size /= MEASURE_ITERATIONS;
    BIKE_L5_performance_metrics_mean_long_message.after_decryption.virtual_memory /= MEASURE_ITERATIONS;
    BIKE_L5_performance_metrics_mean_long_message.after_all_operations.resident_set_size /= MEASURE_ITERATIONS;
    BIKE_L5_performance_metrics_mean_long_message.after_all_operations.virtual_memory /= MEASURE_ITERATIONS;

    // write the means of each perfomance measurements in a csv formatted file
    FILE *fp = fopen("../KEM/BIKE_performance_metrics.csv", "w");
    if (fp == NULL) {
        perror("Failed to open file");
        return 1;
    }
    // Print header
    fprintf(fp, "KEM Variant,Message length,Key generation,Encapsulation,Decapsulation,Encryption,Decryption,Before any operations (RSS),Before any operations (VM),After key generation (RSS),After key generation (VM),After encapsulation (RSS),After encapsulation (VM),After decapsulation (RSS),After decapsulation (VM),After encryption (RSS),After encryption (VM),After decryption (RSS),After decryption (VM),After all operations (RSS),After all operations (VM)\n");
    // Print data

    // BIKE-L1 short message
    fprintf(fp, "BIKE-L1,%s,%f,%f,%f,%f,%f,%zu,%zu,%zu,%zu,%zu,%zu,%zu,%zu,%zu,%zu,%zu,%zu,%zu,%zu\n",
            "Short",
            BIKE_L1_performance_metrics_mean_short_message.keypair_generation_time,
            BIKE_L1_performance_metrics_mean_short_message.encapsulation_time,
            BIKE_L1_performance_metrics_mean_short_message.decapsulation_time,
            BIKE_L1_performance_metrics_mean_short_message.encryption_time,
            BIKE_L1_performance_metrics_mean_short_message.decryption_time,
            BIKE_L1_performance_metrics_mean_short_message.before_any_operations.resident_set_size,
            BIKE_L1_performance_metrics_mean_short_message.before_any_operations.virtual_memory,
            BIKE_L1_performance_metrics_mean_short_message.after_key_generation.resident_set_size,
            BIKE_L1_performance_metrics_mean_short_message.after_key_generation.virtual_memory,
            BIKE_L1_performance_metrics_mean_short_message.after_encapsulation.resident_set_size,
            BIKE_L1_performance_metrics_mean_short_message.after_encapsulation.virtual_memory,
            BIKE_L1_performance_metrics_mean_short_message.after_decapsulation.resident_set_size,
            BIKE_L1_performance_metrics_mean_short_message.after_decapsulation.virtual_memory,
            BIKE_L1_performance_metrics_mean_short_message.after_encryption.resident_set_size,
            BIKE_L1_performance_metrics_mean_short_message.after_encryption.virtual_memory,
            BIKE_L1_performance_metrics_mean_short_message.after_decryption.resident_set_size,
            BIKE_L1_performance_metrics_mean_short_message.after_decryption.virtual_memory,
            BIKE_L1_performance_metrics_mean_short_message.after_all_operations.resident_set_size,
            BIKE_L1_performance_metrics_mean_short_message.after_all_operations.virtual_memory);

    // BIKE-L1 medium message
    fprintf(fp, "BIKE-L1,%s,%f,%f,%f,%f,%f,%zu,%zu,%zu,%zu,%zu,%zu,%zu,%zu,%zu,%zu,%zu,%zu,%zu,%zu\n",
            "Medium",
            BIKE_L1_performance_metrics_mean_medium_message.keypair_generation_time,
            BIKE_L1_performance_metrics_mean_medium_message.encapsulation_time,
            BIKE_L1_performance_metrics_mean_medium_message.decapsulation_time,
            BIKE_L1_performance_metrics_mean_medium_message.encryption_time,
            BIKE_L1_performance_metrics_mean_medium_message.decryption_time,
            BIKE_L1_performance_metrics_mean_medium_message.before_any_operations.resident_set_size,
            BIKE_L1_performance_metrics_mean_medium_message.before_any_operations.virtual_memory,
            BIKE_L1_performance_metrics_mean_medium_message.after_key_generation.resident_set_size,
            BIKE_L1_performance_metrics_mean_medium_message.after_key_generation.virtual_memory,
            BIKE_L1_performance_metrics_mean_medium_message.after_encapsulation.resident_set_size,
            BIKE_L1_performance_metrics_mean_medium_message.after_encapsulation.virtual_memory,
            BIKE_L1_performance_metrics_mean_medium_message.after_decapsulation.resident_set_size,
            BIKE_L1_performance_metrics_mean_medium_message.after_decapsulation.virtual_memory,
            BIKE_L1_performance_metrics_mean_medium_message.after_encryption.resident_set_size,
            BIKE_L1_performance_metrics_mean_medium_message.after_encryption.virtual_memory,
            BIKE_L1_performance_metrics_mean_medium_message.after_decryption.resident_set_size,
            BIKE_L1_performance_metrics_mean_medium_message.after_decryption.virtual_memory,
            BIKE_L1_performance_metrics_mean_medium_message.after_all_operations.resident_set_size,
            BIKE_L1_performance_metrics_mean_medium_message.after_all_operations.virtual_memory);

    // BIKE-L1 long message
    fprintf(fp, "BIKE-L1,%s,%f,%f,%f,%f,%f,%zu,%zu,%zu,%zu,%zu,%zu,%zu,%zu,%zu,%zu,%zu,%zu,%zu,%zu\n",
            "Long",
            BIKE_L1_performance_metrics_mean_long_message.keypair_generation_time,
            BIKE_L1_performance_metrics_mean_long_message.encapsulation_time,
            BIKE_L1_performance_metrics_mean_long_message.decapsulation_time,
            BIKE_L1_performance_metrics_mean_long_message.encryption_time,
            BIKE_L1_performance_metrics_mean_long_message.decryption_time,
            BIKE_L1_performance_metrics_mean_long_message.before_any_operations.resident_set_size,
            BIKE_L1_performance_metrics_mean_long_message.before_any_operations.virtual_memory,
            BIKE_L1_performance_metrics_mean_long_message.after_key_generation.resident_set_size,
            BIKE_L1_performance_metrics_mean_long_message.after_key_generation.virtual_memory,
            BIKE_L1_performance_metrics_mean_long_message.after_encapsulation.resident_set_size,
            BIKE_L1_performance_metrics_mean_long_message.after_encapsulation.virtual_memory,
            BIKE_L1_performance_metrics_mean_long_message.after_decapsulation.resident_set_size,
            BIKE_L1_performance_metrics_mean_long_message.after_decapsulation.virtual_memory,
            BIKE_L1_performance_metrics_mean_long_message.after_encryption.resident_set_size,
            BIKE_L1_performance_metrics_mean_long_message.after_encryption.virtual_memory,
            BIKE_L1_performance_metrics_mean_long_message.after_decryption.resident_set_size,
            BIKE_L1_performance_metrics_mean_long_message.after_decryption.virtual_memory,
            BIKE_L1_performance_metrics_mean_long_message.after_all_operations.resident_set_size,
            BIKE_L1_performance_metrics_mean_long_message.after_all_operations.virtual_memory);

    // BIKE-L3 short message
    fprintf(fp, "BIKE-L3,%s,%f,%f,%f,%f,%f,%zu,%zu,%zu,%zu,%zu,%zu,%zu,%zu,%zu,%zu,%zu,%zu,%zu,%zu\n",
            "Short",
            BIKE_L3_performance_metrics_mean_short_message.keypair_generation_time,
            BIKE_L3_performance_metrics_mean_short_message.encapsulation_time,
            BIKE_L3_performance_metrics_mean_short_message.decapsulation_time,
            BIKE_L3_performance_metrics_mean_short_message.encryption_time,
            BIKE_L3_performance_metrics_mean_short_message.decryption_time,
            BIKE_L3_performance_metrics_mean_short_message.before_any_operations.resident_set_size,
            BIKE_L3_performance_metrics_mean_short_message.before_any_operations.virtual_memory,
            BIKE_L3_performance_metrics_mean_short_message.after_key_generation.resident_set_size,
            BIKE_L3_performance_metrics_mean_short_message.after_key_generation.virtual_memory,
            BIKE_L3_performance_metrics_mean_short_message.after_encapsulation.resident_set_size,
            BIKE_L3_performance_metrics_mean_short_message.after_encapsulation.virtual_memory,
            BIKE_L3_performance_metrics_mean_short_message.after_decapsulation.resident_set_size,
            BIKE_L3_performance_metrics_mean_short_message.after_decapsulation.virtual_memory,
            BIKE_L3_performance_metrics_mean_short_message.after_encryption.resident_set_size,
            BIKE_L3_performance_metrics_mean_short_message.after_encryption.virtual_memory,
            BIKE_L3_performance_metrics_mean_short_message.after_decryption.resident_set_size,
            BIKE_L3_performance_metrics_mean_short_message.after_decryption.virtual_memory,
            BIKE_L3_performance_metrics_mean_short_message.after_all_operations.resident_set_size,
            BIKE_L3_performance_metrics_mean_short_message.after_all_operations.virtual_memory);

    // BIKE-L3 medium message
    fprintf(fp, "BIKE-L3,%s,%f,%f,%f,%f,%f,%zu,%zu,%zu,%zu,%zu,%zu,%zu,%zu,%zu,%zu,%zu,%zu,%zu,%zu\n",
            "Medium",
            BIKE_L3_performance_metrics_mean_medium_message.keypair_generation_time,
            BIKE_L3_performance_metrics_mean_medium_message.encapsulation_time,
            BIKE_L3_performance_metrics_mean_medium_message.decapsulation_time,
            BIKE_L3_performance_metrics_mean_medium_message.encryption_time,
            BIKE_L3_performance_metrics_mean_medium_message.decryption_time,
            BIKE_L3_performance_metrics_mean_medium_message.before_any_operations.resident_set_size,
            BIKE_L3_performance_metrics_mean_medium_message.before_any_operations.virtual_memory,
            BIKE_L3_performance_metrics_mean_medium_message.after_key_generation.resident_set_size,
            BIKE_L3_performance_metrics_mean_medium_message.after_key_generation.virtual_memory,
            BIKE_L3_performance_metrics_mean_medium_message.after_encapsulation.resident_set_size,
            BIKE_L3_performance_metrics_mean_medium_message.after_encapsulation.virtual_memory,
            BIKE_L3_performance_metrics_mean_medium_message.after_decapsulation.resident_set_size,
            BIKE_L3_performance_metrics_mean_medium_message.after_decapsulation.virtual_memory,
            BIKE_L3_performance_metrics_mean_medium_message.after_encryption.resident_set_size,
            BIKE_L3_performance_metrics_mean_medium_message.after_encryption.virtual_memory,
            BIKE_L3_performance_metrics_mean_medium_message.after_decryption.resident_set_size,
            BIKE_L3_performance_metrics_mean_medium_message.after_decryption.virtual_memory,
            BIKE_L3_performance_metrics_mean_medium_message.after_all_operations.resident_set_size,
            BIKE_L3_performance_metrics_mean_medium_message.after_all_operations.virtual_memory);

    // BIKE-L3 long message
    fprintf(fp, "BIKE-L3,%s,%f,%f,%f,%f,%f,%zu,%zu,%zu,%zu,%zu,%zu,%zu,%zu,%zu,%zu,%zu,%zu,%zu,%zu\n",
            "Long",
            BIKE_L3_performance_metrics_mean_long_message.keypair_generation_time,
            BIKE_L3_performance_metrics_mean_long_message.encapsulation_time,
            BIKE_L3_performance_metrics_mean_long_message.decapsulation_time,
            BIKE_L3_performance_metrics_mean_long_message.encryption_time,
            BIKE_L3_performance_metrics_mean_long_message.decryption_time,
            BIKE_L3_performance_metrics_mean_long_message.before_any_operations.resident_set_size,
            BIKE_L3_performance_metrics_mean_long_message.before_any_operations.virtual_memory,
            BIKE_L3_performance_metrics_mean_long_message.after_key_generation.resident_set_size,
            BIKE_L3_performance_metrics_mean_long_message.after_key_generation.virtual_memory,
            BIKE_L3_performance_metrics_mean_long_message.after_encapsulation.resident_set_size,
            BIKE_L3_performance_metrics_mean_long_message.after_encapsulation.virtual_memory,
            BIKE_L3_performance_metrics_mean_long_message.after_decapsulation.resident_set_size,
            BIKE_L3_performance_metrics_mean_long_message.after_decapsulation.virtual_memory,
            BIKE_L3_performance_metrics_mean_long_message.after_encryption.resident_set_size,
            BIKE_L3_performance_metrics_mean_long_message.after_encryption.virtual_memory,
            BIKE_L3_performance_metrics_mean_long_message.after_decryption.resident_set_size,
            BIKE_L3_performance_metrics_mean_long_message.after_decryption.virtual_memory,
            BIKE_L3_performance_metrics_mean_long_message.after_all_operations.resident_set_size,
            BIKE_L3_performance_metrics_mean_long_message.after_all_operations.virtual_memory);

    // BIKE-L5 short message
    fprintf(fp, "BIKE-L5,%s,%f,%f,%f,%f,%f,%zu,%zu,%zu,%zu,%zu,%zu,%zu,%zu,%zu,%zu,%zu,%zu,%zu,%zu\n",
            "Short",
            BIKE_L5_performance_metrics_mean_short_message.keypair_generation_time,
            BIKE_L5_performance_metrics_mean_short_message.encapsulation_time,
            BIKE_L5_performance_metrics_mean_short_message.decapsulation_time,
            BIKE_L5_performance_metrics_mean_short_message.encryption_time,
            BIKE_L5_performance_metrics_mean_short_message.decryption_time,
            BIKE_L5_performance_metrics_mean_short_message.before_any_operations.resident_set_size,
            BIKE_L5_performance_metrics_mean_short_message.before_any_operations.virtual_memory,
            BIKE_L5_performance_metrics_mean_short_message.after_key_generation.resident_set_size,
            BIKE_L5_performance_metrics_mean_short_message.after_key_generation.virtual_memory,
            BIKE_L5_performance_metrics_mean_short_message.after_encapsulation.resident_set_size,
            BIKE_L5_performance_metrics_mean_short_message.after_encapsulation.virtual_memory,
            BIKE_L5_performance_metrics_mean_short_message.after_decapsulation.resident_set_size,
            BIKE_L5_performance_metrics_mean_short_message.after_decapsulation.virtual_memory,
            BIKE_L5_performance_metrics_mean_short_message.after_encryption.resident_set_size,
            BIKE_L5_performance_metrics_mean_short_message.after_encryption.virtual_memory,
            BIKE_L5_performance_metrics_mean_short_message.after_decryption.resident_set_size,
            BIKE_L5_performance_metrics_mean_short_message.after_decryption.virtual_memory,
            BIKE_L5_performance_metrics_mean_short_message.after_all_operations.resident_set_size,
            BIKE_L5_performance_metrics_mean_short_message.after_all_operations.virtual_memory);

    // BIKE-L5 medium message
    fprintf(fp, "BIKE-L5,%s,%f,%f,%f,%f,%f,%zu,%zu,%zu,%zu,%zu,%zu,%zu,%zu,%zu,%zu,%zu,%zu,%zu,%zu\n",
            "Medium",
            BIKE_L5_performance_metrics_mean_medium_message.keypair_generation_time,
            BIKE_L5_performance_metrics_mean_medium_message.encapsulation_time,
            BIKE_L5_performance_metrics_mean_medium_message.decapsulation_time,
            BIKE_L5_performance_metrics_mean_medium_message.encryption_time,
            BIKE_L5_performance_metrics_mean_medium_message.decryption_time,
            BIKE_L5_performance_metrics_mean_medium_message.before_any_operations.resident_set_size,
            BIKE_L5_performance_metrics_mean_medium_message.before_any_operations.virtual_memory,
            BIKE_L5_performance_metrics_mean_medium_message.after_key_generation.resident_set_size,
            BIKE_L5_performance_metrics_mean_medium_message.after_key_generation.virtual_memory,
            BIKE_L5_performance_metrics_mean_medium_message.after_encapsulation.resident_set_size,
            BIKE_L5_performance_metrics_mean_medium_message.after_encapsulation.virtual_memory,
            BIKE_L5_performance_metrics_mean_medium_message.after_decapsulation.resident_set_size,
            BIKE_L5_performance_metrics_mean_medium_message.after_decapsulation.virtual_memory,
            BIKE_L5_performance_metrics_mean_medium_message.after_encryption.resident_set_size,
            BIKE_L5_performance_metrics_mean_medium_message.after_encryption.virtual_memory,
            BIKE_L5_performance_metrics_mean_medium_message.after_decryption.resident_set_size,
            BIKE_L5_performance_metrics_mean_medium_message.after_decryption.virtual_memory,
            BIKE_L5_performance_metrics_mean_medium_message.after_all_operations.resident_set_size,
            BIKE_L5_performance_metrics_mean_medium_message.after_all_operations.virtual_memory);

    // BIKE-L5 long message
    fprintf(fp, "BIKE-L5,%s,%f,%f,%f,%f,%f,%zu,%zu,%zu,%zu,%zu,%zu,%zu,%zu,%zu,%zu,%zu,%zu,%zu,%zu\n",
            "Long",
            BIKE_L5_performance_metrics_mean_long_message.keypair_generation_time,
            BIKE_L5_performance_metrics_mean_long_message.encapsulation_time,
            BIKE_L5_performance_metrics_mean_long_message.decapsulation_time,
            BIKE_L5_performance_metrics_mean_long_message.encryption_time,
            BIKE_L5_performance_metrics_mean_long_message.decryption_time,
            BIKE_L5_performance_metrics_mean_long_message.before_any_operations.resident_set_size,
            BIKE_L5_performance_metrics_mean_long_message.before_any_operations.virtual_memory,
            BIKE_L5_performance_metrics_mean_long_message.after_key_generation.resident_set_size,
            BIKE_L5_performance_metrics_mean_long_message.after_key_generation.virtual_memory,
            BIKE_L5_performance_metrics_mean_long_message.after_encapsulation.resident_set_size,
            BIKE_L5_performance_metrics_mean_long_message.after_encapsulation.virtual_memory,
            BIKE_L5_performance_metrics_mean_long_message.after_decapsulation.resident_set_size,
            BIKE_L5_performance_metrics_mean_long_message.after_decapsulation.virtual_memory,
            BIKE_L5_performance_metrics_mean_long_message.after_encryption.resident_set_size,
            BIKE_L5_performance_metrics_mean_long_message.after_encryption.virtual_memory,
            BIKE_L5_performance_metrics_mean_long_message.after_decryption.resident_set_size,
            BIKE_L5_performance_metrics_mean_long_message.after_decryption.virtual_memory,
            BIKE_L5_performance_metrics_mean_long_message.after_all_operations.resident_set_size,
            BIKE_L5_performance_metrics_mean_long_message.after_all_operations.virtual_memory);

    fclose(fp);

    return 0;
}