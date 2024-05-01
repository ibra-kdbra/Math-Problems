#include "file_helper.h"

#include <sys/stat.h>
#include <stdarg.h>
#include <stdlib.h>

#include "util/dbg/debug.h"

int skip_to_char(FILE* file, const char target, char* const buffer, size_t limit) {
    if (file == NULL) return -1;

    int count = 0;

    while (true) {
        char in_c = (char)fgetc(file);

        if (in_c == target) return count;
        if (in_c == EOF) return -1;

        if (buffer && (count < (int)limit || limit == 0)) buffer[count] = in_c;
        count++;
    }
}

void fclose_void(FILE** ptr) {
    if (ptr == NULL) return;
    if (*ptr) fclose(*ptr);
}

size_t get_file_size(int fd) {
    struct stat buffer;
    fstat(fd, &buffer);
    return (size_t)buffer.st_size;
}

int caret_printf(caret_t* caret, const char* format, ...) {
    va_list args;
    va_start(args, format);

    int delta = vsprintf(*caret, format, args);
    *caret += delta;

    va_end(args);

    return delta;
}

char* read_whole(const char* fname) {
    int fd = open(fname, O_RDONLY);

    if (!fd) return NULL;

    size_t buf_size = get_file_size(fd) + 1;
    char* buffer = (char*) calloc(buf_size, sizeof(buffer));

    if (!buffer) return 0;

    read(fd, buffer, buf_size);

    buffer[buf_size - 1] = '\0';

    return buffer;
}

char* dynamic_sprintf(const char* format, ...) {
    va_list args;
    va_list args_copy;
    va_start(args, format);
    va_copy(args_copy, args);

    size_t length = (size_t)vsnprintf(NULL, 0, format, args_copy);

    va_end(args_copy);

    char* result = (char*) calloc(length + 1, sizeof(*result));
    _LOG_FAIL_CHECK_(result, "error", ERROR_REPORTS, return NULL, &errno, ENOMEM);
    vsnprintf(result, length + 1, format, args);

    va_end(args);

    return result;
}