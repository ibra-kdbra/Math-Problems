#ifndef UNTITLED_MEASURE_PERFORMANCE_H
#define UNTITLED_MEASURE_PERFORMANCE_H

#include <time.h>

extern clock_t start_time;

void start_timer();

double stop_timer(const char *logging_text_format, ...);

/**
 * Values are in kB
 */
struct s_memory_usage {
    size_t virtual_memory; // Virtual memory
    size_t resident_set_size; // Physical memory
};

struct s_memory_usage measure_memory(const char *logging_text_format, ...);

#endif //UNTITLED_MEASURE_PERFORMANCE_H
