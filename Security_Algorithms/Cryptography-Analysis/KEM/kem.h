#ifndef DISSERTATION_PROJECT_KEM_H
#define DISSERTATION_PROJECT_KEM_H

#include <stdint-gcc.h>
#include "../measure_performance.h"

enum message_type {
    SHORT_MESSAGE,
    MEDIUM_MESSAGE,
    LONG_MESSAGE
};

typedef struct s_KEM_performance_analysis_metrics {
    double keypair_generation_time;
    double encapsulation_time;
    double decapsulation_time;
    double encryption_time;
    double decryption_time;
    struct s_memory_usage before_any_operations;
    struct s_memory_usage after_key_generation;
    struct s_memory_usage after_encapsulation;
    struct s_memory_usage after_decapsulation;
    struct s_memory_usage after_encryption;
    struct s_memory_usage after_decryption;
    struct s_memory_usage after_all_operations;
    enum message_type message_lengthyness;
} KEM_performance_metrics;

int run_KEM_performance_analysis(const char *kem_variant, const uint8_t *plaintext, KEM_performance_metrics *kem_performance_metrics);

#endif //DISSERTATION_PROJECT_KEM_H
