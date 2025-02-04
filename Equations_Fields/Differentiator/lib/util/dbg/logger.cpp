#include "logger.h"

#include <string.h>
#include <time.h>

#include "debug.h"

static FILE* logfile = NULL;
static unsigned int log_threshold = 0;

/**
 * @brief Prints out log line prefix (time and tag).
 * 
 * @param tag (optional) prefix tag
 * @param importance (optional) message importance
 */
static void log_prefix(const char* tag = "status", const unsigned int importance = ABSOLUTE_IMPORTANCE);

/**
 * @brief Returns currently opened log file by given importance.
 * 
 * @param importance (optional) importance of the message file will be used for.
 * 
 * @return FILE* log file
 */
static FILE* log_file(const unsigned int importance = ABSOLUTE_IMPORTANCE);

void log_init(const char* filename, const unsigned int threshold, int* const error_code) {
    log_threshold = threshold;

    if ((logfile = fopen(filename, "a"))) {
        setvbuf(logfile, NULL, _IONBF, 0);
        fprintf(logfile, "<pre>");
        log_printf(ABSOLUTE_IMPORTANCE, "open", "Log file %s was opened.\n", filename);
        return;
    }

    if (error_code) *error_code = FILE_ERROR;
}

static void log_prefix(const char* tag, const unsigned int importance) {
    if (!log_file()) return;
    time_t raw_time;
    struct tm * time_info;

    time(&raw_time);
    time_info = localtime (&raw_time);
    char* pc_timestamp = asctime(time_info);
    pc_timestamp[strlen(pc_timestamp) - 1] = '\0';

    fprintf(log_file(importance), "%-20s [%s]:  ", pc_timestamp, tag);
}

void _log_printf(const unsigned int importance, const char* tag, const char* format, ...) {
    va_list args;
    va_start(args, format);

    if (importance >= log_threshold && logfile) {
        log_prefix(tag, importance);
        vfprintf(log_file(importance), format, args);
        fflush(log_file(importance));
    }

    va_end(args);
}

static FILE* log_file(const unsigned int importance) {
    return importance >= log_threshold ? logfile : NULL;
}

void log_close(int* error_code) {
    if (!log_file()) return;
    log_printf(ABSOLUTE_IMPORTANCE, "close", "Closing log file.\n\n");
    fprintf(log_file(ABSOLUTE_IMPORTANCE), "</pre>");
    if (!fclose(logfile) && error_code) *error_code = FILE_ERROR;
}