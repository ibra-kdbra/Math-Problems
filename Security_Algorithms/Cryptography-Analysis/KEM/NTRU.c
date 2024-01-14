#include <oqs/oqs.h>
#include "kem.h"
#include "../measure_performance.h"
#include "../utils.h"
#include "../RSA/rsa.h"
#include <string.h>

#define MEASURE_ITERATIONS 1000

int main(void) {

    KEM_performance_metrics ntru_761_performance_metrics_short_message[MEASURE_ITERATIONS] = {0};
    KEM_performance_metrics ntru_761_performance_metrics_medium_message[MEASURE_ITERATIONS] = {0};
    KEM_performance_metrics ntru_761_performance_metrics_long_message[MEASURE_ITERATIONS] = {0};

    // Run performance analysis for NTRU KEM variant
    for (size_t i = 0; i < MEASURE_ITERATIONS; i++) {
        puts("Short message");
        ntru_761_performance_metrics_short_message[i].message_lengthyness = SHORT_MESSAGE;
        run_KEM_performance_analysis("sntrup761", short_message, &ntru_761_performance_metrics_short_message[i]);

        puts("Medium message");
        ntru_761_performance_metrics_medium_message[i].message_lengthyness = MEDIUM_MESSAGE;
        run_KEM_performance_analysis("sntrup761", medium_message, &ntru_761_performance_metrics_medium_message[i]);

        puts("Long message");
        ntru_761_performance_metrics_long_message[i].message_lengthyness = LONG_MESSAGE;
        run_KEM_performance_analysis("sntrup761", long_message, &ntru_761_performance_metrics_long_message[i]);
    }

    // Calculate the means of the performance metrics
    KEM_performance_metrics ntru_761_performance_metrics_short_message_mean = {0};
    KEM_performance_metrics ntru_761_performance_metrics_medium_message_mean = {0};
    KEM_performance_metrics ntru_761_performance_metrics_long_message_mean = {0};

    for (size_t i = 0; i < MEASURE_ITERATIONS; i++) {
        // Short message
        ntru_761_performance_metrics_short_message_mean.keypair_generation_time += ntru_761_performance_metrics_short_message[i].keypair_generation_time;
        ntru_761_performance_metrics_short_message_mean.encapsulation_time += ntru_761_performance_metrics_short_message[i].encapsulation_time;
        ntru_761_performance_metrics_short_message_mean.decapsulation_time += ntru_761_performance_metrics_short_message[i].decapsulation_time;
        ntru_761_performance_metrics_short_message_mean.encryption_time += ntru_761_performance_metrics_short_message[i].encryption_time;
        ntru_761_performance_metrics_short_message_mean.decryption_time += ntru_761_performance_metrics_short_message[i].decryption_time;
        ntru_761_performance_metrics_short_message_mean.before_any_operations.resident_set_size += ntru_761_performance_metrics_short_message[i].before_any_operations.resident_set_size;
        ntru_761_performance_metrics_short_message_mean.before_any_operations.virtual_memory += ntru_761_performance_metrics_short_message[i].before_any_operations.virtual_memory;
        ntru_761_performance_metrics_short_message_mean.after_key_generation.resident_set_size += ntru_761_performance_metrics_short_message[i].after_key_generation.resident_set_size;
        ntru_761_performance_metrics_short_message_mean.after_key_generation.virtual_memory += ntru_761_performance_metrics_short_message[i].after_key_generation.virtual_memory;
        ntru_761_performance_metrics_short_message_mean.after_encapsulation.resident_set_size += ntru_761_performance_metrics_short_message[i].after_encapsulation.resident_set_size;
        ntru_761_performance_metrics_short_message_mean.after_encapsulation.virtual_memory += ntru_761_performance_metrics_short_message[i].after_encapsulation.virtual_memory;
        ntru_761_performance_metrics_short_message_mean.after_decapsulation.resident_set_size += ntru_761_performance_metrics_short_message[i].after_decapsulation.resident_set_size;
        ntru_761_performance_metrics_short_message_mean.after_decapsulation.virtual_memory += ntru_761_performance_metrics_short_message[i].after_decapsulation.virtual_memory;
        ntru_761_performance_metrics_short_message_mean.after_all_operations.resident_set_size += ntru_761_performance_metrics_short_message[i].after_all_operations.resident_set_size;
        ntru_761_performance_metrics_short_message_mean.after_all_operations.virtual_memory += ntru_761_performance_metrics_short_message[i].after_all_operations.virtual_memory;

        // Medium message
        ntru_761_performance_metrics_medium_message_mean.keypair_generation_time += ntru_761_performance_metrics_medium_message[i].keypair_generation_time;
        ntru_761_performance_metrics_medium_message_mean.encapsulation_time += ntru_761_performance_metrics_medium_message[i].encapsulation_time;
        ntru_761_performance_metrics_medium_message_mean.decapsulation_time += ntru_761_performance_metrics_medium_message[i].decapsulation_time;
        ntru_761_performance_metrics_medium_message_mean.encryption_time += ntru_761_performance_metrics_medium_message[i].encryption_time;
        ntru_761_performance_metrics_medium_message_mean.decryption_time += ntru_761_performance_metrics_medium_message[i].decryption_time;
        ntru_761_performance_metrics_medium_message_mean.before_any_operations.resident_set_size += ntru_761_performance_metrics_medium_message[i].before_any_operations.resident_set_size;
        ntru_761_performance_metrics_medium_message_mean.before_any_operations.virtual_memory += ntru_761_performance_metrics_medium_message[i].before_any_operations.virtual_memory;
        ntru_761_performance_metrics_medium_message_mean.after_key_generation.resident_set_size += ntru_761_performance_metrics_medium_message[i].after_key_generation.resident_set_size;
        ntru_761_performance_metrics_medium_message_mean.after_key_generation.virtual_memory += ntru_761_performance_metrics_medium_message[i].after_key_generation.virtual_memory;
        ntru_761_performance_metrics_medium_message_mean.after_encapsulation.resident_set_size += ntru_761_performance_metrics_medium_message[i].after_encapsulation.resident_set_size;
        ntru_761_performance_metrics_medium_message_mean.after_encapsulation.virtual_memory += ntru_761_performance_metrics_medium_message[i].after_encapsulation.virtual_memory;
        ntru_761_performance_metrics_medium_message_mean.after_decapsulation.resident_set_size += ntru_761_performance_metrics_medium_message[i].after_decapsulation.resident_set_size;
        ntru_761_performance_metrics_medium_message_mean.after_decapsulation.virtual_memory += ntru_761_performance_metrics_medium_message[i].after_decapsulation.virtual_memory;
        ntru_761_performance_metrics_medium_message_mean.after_all_operations.resident_set_size += ntru_761_performance_metrics_medium_message[i].after_all_operations.resident_set_size;
        ntru_761_performance_metrics_medium_message_mean.after_all_operations.virtual_memory += ntru_761_performance_metrics_medium_message[i].after_all_operations.virtual_memory;

        // Long message
        ntru_761_performance_metrics_long_message_mean.keypair_generation_time += ntru_761_performance_metrics_long_message[i].keypair_generation_time;
        ntru_761_performance_metrics_long_message_mean.encapsulation_time += ntru_761_performance_metrics_long_message[i].encapsulation_time;
        ntru_761_performance_metrics_long_message_mean.decapsulation_time += ntru_761_performance_metrics_long_message[i].decapsulation_time;
        ntru_761_performance_metrics_long_message_mean.encryption_time += ntru_761_performance_metrics_long_message[i].encryption_time;
        ntru_761_performance_metrics_long_message_mean.decryption_time += ntru_761_performance_metrics_long_message[i].decryption_time;
        ntru_761_performance_metrics_long_message_mean.before_any_operations.resident_set_size += ntru_761_performance_metrics_long_message[i].before_any_operations.resident_set_size;
        ntru_761_performance_metrics_long_message_mean.before_any_operations.virtual_memory += ntru_761_performance_metrics_long_message[i].before_any_operations.virtual_memory;
        ntru_761_performance_metrics_long_message_mean.after_key_generation.resident_set_size += ntru_761_performance_metrics_long_message[i].after_key_generation.resident_set_size;
        ntru_761_performance_metrics_long_message_mean.after_key_generation.virtual_memory += ntru_761_performance_metrics_long_message[i].after_key_generation.virtual_memory;
        ntru_761_performance_metrics_long_message_mean.after_encapsulation.resident_set_size += ntru_761_performance_metrics_long_message[i].after_encapsulation.resident_set_size;
        ntru_761_performance_metrics_long_message_mean.after_encapsulation.virtual_memory += ntru_761_performance_metrics_long_message[i].after_encapsulation.virtual_memory;
        ntru_761_performance_metrics_long_message_mean.after_decapsulation.resident_set_size += ntru_761_performance_metrics_long_message[i].after_decapsulation.resident_set_size;
        ntru_761_performance_metrics_long_message_mean.after_decapsulation.virtual_memory += ntru_761_performance_metrics_long_message[i].after_decapsulation.virtual_memory;
        ntru_761_performance_metrics_long_message_mean.after_all_operations.resident_set_size += ntru_761_performance_metrics_long_message[i].after_all_operations.resident_set_size;
        ntru_761_performance_metrics_long_message_mean.after_all_operations.virtual_memory += ntru_761_performance_metrics_long_message[i].after_all_operations.virtual_memory;

    }

    // Calculate the means
    ntru_761_performance_metrics_short_message_mean.keypair_generation_time /= MEASURE_ITERATIONS;
    ntru_761_performance_metrics_short_message_mean.encapsulation_time /= MEASURE_ITERATIONS;
    ntru_761_performance_metrics_short_message_mean.decapsulation_time /= MEASURE_ITERATIONS;
    ntru_761_performance_metrics_short_message_mean.encryption_time /= MEASURE_ITERATIONS;
    ntru_761_performance_metrics_short_message_mean.decryption_time /= MEASURE_ITERATIONS;
    ntru_761_performance_metrics_short_message_mean.before_any_operations.resident_set_size /= MEASURE_ITERATIONS;
    ntru_761_performance_metrics_short_message_mean.before_any_operations.virtual_memory /= MEASURE_ITERATIONS;
    ntru_761_performance_metrics_short_message_mean.after_key_generation.resident_set_size /= MEASURE_ITERATIONS;
    ntru_761_performance_metrics_short_message_mean.after_key_generation.virtual_memory /= MEASURE_ITERATIONS;
    ntru_761_performance_metrics_short_message_mean.after_encapsulation.resident_set_size /= MEASURE_ITERATIONS;
    ntru_761_performance_metrics_short_message_mean.after_encapsulation.virtual_memory /= MEASURE_ITERATIONS;
    ntru_761_performance_metrics_short_message_mean.after_decapsulation.resident_set_size /= MEASURE_ITERATIONS;
    ntru_761_performance_metrics_short_message_mean.after_decapsulation.virtual_memory /= MEASURE_ITERATIONS;
    ntru_761_performance_metrics_short_message_mean.after_all_operations.resident_set_size /= MEASURE_ITERATIONS;
    ntru_761_performance_metrics_short_message_mean.after_all_operations.virtual_memory /= MEASURE_ITERATIONS;

    ntru_761_performance_metrics_medium_message_mean.keypair_generation_time /= MEASURE_ITERATIONS;
    ntru_761_performance_metrics_medium_message_mean.encapsulation_time /= MEASURE_ITERATIONS;
    ntru_761_performance_metrics_medium_message_mean.decapsulation_time /= MEASURE_ITERATIONS;
    ntru_761_performance_metrics_medium_message_mean.encryption_time /= MEASURE_ITERATIONS;
    ntru_761_performance_metrics_medium_message_mean.decryption_time /= MEASURE_ITERATIONS;
    ntru_761_performance_metrics_medium_message_mean.before_any_operations.resident_set_size /= MEASURE_ITERATIONS;
    ntru_761_performance_metrics_medium_message_mean.before_any_operations.virtual_memory /= MEASURE_ITERATIONS;
    ntru_761_performance_metrics_medium_message_mean.after_key_generation.resident_set_size /= MEASURE_ITERATIONS;
    ntru_761_performance_metrics_medium_message_mean.after_key_generation.virtual_memory /= MEASURE_ITERATIONS;
    ntru_761_performance_metrics_medium_message_mean.after_encapsulation.resident_set_size /= MEASURE_ITERATIONS;
    ntru_761_performance_metrics_medium_message_mean.after_encapsulation.virtual_memory /= MEASURE_ITERATIONS;
    ntru_761_performance_metrics_medium_message_mean.after_decapsulation.resident_set_size /= MEASURE_ITERATIONS;
    ntru_761_performance_metrics_medium_message_mean.after_decapsulation.virtual_memory /= MEASURE_ITERATIONS;
    ntru_761_performance_metrics_medium_message_mean.after_all_operations.resident_set_size /= MEASURE_ITERATIONS;
    ntru_761_performance_metrics_medium_message_mean.after_all_operations.virtual_memory /= MEASURE_ITERATIONS;


    // Write performance metrics to CSV file
    FILE *file = fopen("../KEM/ntru_performance_metrics.csv", "w");
    if (file == NULL) {
        fprintf(stderr, "Failed to open file for writing\n");
        return 1;
    }

    // Print header
    fprintf(file, "KEM variant,Key generation time,Encapsulation time,Decapsulation time,Encryption time,Decryption time,Resident set size before any operations (kB),Virtual memory before any operations (kB),Resident set size after key generation (kB),Virtual memory after key generation (kB),Resident set size after encapsulation (kB),Virtual memory after encapsulation (kB),Resident set size after decapsulation (kB),Virtual memory after decapsulation (kB),Resident set size after all operations (kB),Virtual memory after all operations (kB),Message Length\n");

    // Print metrics for NTRU 761 short message
    fprintf(file, "%s,%f,%f,%f,%f,%f,%zu,%zu,%zu,%zu,%zu,%zu,%zu,%zu,%zu,%zu,%s\n",
            "sntrup761",
            ntru_761_performance_metrics_short_message_mean.keypair_generation_time,
            ntru_761_performance_metrics_short_message_mean.encapsulation_time,
            ntru_761_performance_metrics_short_message_mean.decapsulation_time,
            ntru_761_performance_metrics_short_message_mean.encryption_time,
            ntru_761_performance_metrics_short_message_mean.decryption_time,
            ntru_761_performance_metrics_short_message_mean.before_any_operations.resident_set_size,
            ntru_761_performance_metrics_short_message_mean.before_any_operations.virtual_memory,
            ntru_761_performance_metrics_short_message_mean.after_key_generation.resident_set_size,
            ntru_761_performance_metrics_short_message_mean.after_key_generation.virtual_memory,
            ntru_761_performance_metrics_short_message_mean.after_encapsulation.resident_set_size,
            ntru_761_performance_metrics_short_message_mean.after_encapsulation.virtual_memory,
            ntru_761_performance_metrics_short_message_mean.after_decapsulation.resident_set_size,
            ntru_761_performance_metrics_short_message_mean.after_decapsulation.virtual_memory,
            ntru_761_performance_metrics_short_message_mean.after_all_operations.resident_set_size,
            ntru_761_performance_metrics_short_message_mean.after_all_operations.virtual_memory,
            "Short"
    );

    // Print metrics for NTRU 761 medium message
    fprintf(file, "%s,%f,%f,%f,%f,%f,%zu,%zu,%zu,%zu,%zu,%zu,%zu,%zu,%zu,%zu,%s\n",
            "sntrup761",
            ntru_761_performance_metrics_medium_message_mean.keypair_generation_time,
            ntru_761_performance_metrics_medium_message_mean.encapsulation_time,
            ntru_761_performance_metrics_medium_message_mean.decapsulation_time,
            ntru_761_performance_metrics_medium_message_mean.encryption_time,
            ntru_761_performance_metrics_medium_message_mean.decryption_time,
            ntru_761_performance_metrics_medium_message_mean.before_any_operations.resident_set_size,
            ntru_761_performance_metrics_medium_message_mean.before_any_operations.virtual_memory,
            ntru_761_performance_metrics_medium_message_mean.after_key_generation.resident_set_size,
            ntru_761_performance_metrics_medium_message_mean.after_key_generation.virtual_memory,
            ntru_761_performance_metrics_medium_message_mean.after_encapsulation.resident_set_size,
            ntru_761_performance_metrics_medium_message_mean.after_encapsulation.virtual_memory,
            ntru_761_performance_metrics_medium_message_mean.after_decapsulation.resident_set_size,
            ntru_761_performance_metrics_medium_message_mean.after_decapsulation.virtual_memory,
            ntru_761_performance_metrics_medium_message_mean.after_all_operations.resident_set_size,
            ntru_761_performance_metrics_medium_message_mean.after_all_operations.virtual_memory,
            "Medium"
    );

    // Print metrics for NTRU 761 long message
    fprintf(file, "%s,%f,%f,%f,%f,%f,%zu,%zu,%zu,%zu,%zu,%zu,%zu,%zu,%zu,%zu,%s\n",
            "sntrup761",
            ntru_761_performance_metrics_long_message_mean.keypair_generation_time,
            ntru_761_performance_metrics_long_message_mean.encapsulation_time,
            ntru_761_performance_metrics_long_message_mean.decapsulation_time,
            ntru_761_performance_metrics_long_message_mean.encryption_time,
            ntru_761_performance_metrics_long_message_mean.decryption_time,
            ntru_761_performance_metrics_long_message_mean.before_any_operations.resident_set_size,
            ntru_761_performance_metrics_long_message_mean.before_any_operations.virtual_memory,
            ntru_761_performance_metrics_long_message_mean.after_key_generation.resident_set_size,
            ntru_761_performance_metrics_long_message_mean.after_key_generation.virtual_memory,
            ntru_761_performance_metrics_long_message_mean.after_encapsulation.resident_set_size,
            ntru_761_performance_metrics_long_message_mean.after_encapsulation.virtual_memory,
            ntru_761_performance_metrics_long_message_mean.after_decapsulation.resident_set_size,
            ntru_761_performance_metrics_long_message_mean.after_decapsulation.virtual_memory,
            ntru_761_performance_metrics_long_message_mean.after_all_operations.resident_set_size,
            ntru_761_performance_metrics_long_message_mean.after_all_operations.virtual_memory,
            "Long"
    );

    fclose(file);

    return EXIT_SUCCESS;
}