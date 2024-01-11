#include "measure_performance.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <string.h>

clock_t start_time;

void start_timer() {
    start_time = clock();
}

/**
 *
 * @param logging_text_format
 * @param ... variable number of arguments that will replace the placeholders in the format string
 * @return
 */
double stop_timer(const char *logging_text_format, ...) {
    // Measure execution time
    clock_t end_time = clock();
    double cpu_time_used = ((double) (end_time - start_time)) / CLOCKS_PER_SEC;

    // Print execution time
    char logging_text[100] = {0};
    va_list args; // Variable argument list for the variable number of arguments
    va_start(args, logging_text_format);
    vsprintf(logging_text, logging_text_format, args); // Replace the placeholders in the format string with the arguments
    va_end(args);

    // Print execution time
    printf("[%s] Execution time: %f seconds\n", logging_text, cpu_time_used);

    // Return execution time
    return cpu_time_used;
}

/**
 * This function measures the memory usage of a specific process or algorithm.
 * The process id is the current process id.
 * This will provide more accurate memory usage data for each algorithm.
 * @param logging_text_format
 * @param ...
 * @return the memory usage in kB
 */
struct s_memory_usage measure_memory(const char *logging_text_format, ...) {
    struct s_memory_usage memory_usage = {0};
    // Get the current process ID
    pid_t pid = getpid();

    // Construct the path to the /proc/[pid]/status file
    char path[256];
    snprintf(path, sizeof(path), "/proc/%d/status", pid);

    FILE *file = fopen(path, "r");
    if (!file) {
        perror("Failed to open /proc/[pid]/status");
        return memory_usage;
    }

    int vmSize = 0;
    int rss = 0;
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "VmSize:", 7) == 0) {  // VmSize represents the total virtual memory size
            sscanf(line, "VmSize: %d", &vmSize);
        } else if (strncmp(line, "VmRSS:", 6) == 0) {  // VmRSS represents the Resident Set Size
            sscanf(line, "VmRSS: %d", &rss);
        }

        // If both values are found, break out of the loop
        if (vmSize && rss) {
            break;
        }
    }

    va_list args;
    va_start(args, logging_text_format);
    vprintf(logging_text_format, args);
    va_end(args);

    printf(" Virtual Memory usage (VmSize): %d kB\n", vmSize);
    printf(" Resident Set Size (RSS): %d kB\n", rss);

    memory_usage.virtual_memory = vmSize;
    memory_usage.resident_set_size = rss;

    fclose(file);

    return memory_usage;
}