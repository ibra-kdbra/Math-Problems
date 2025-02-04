#include "debug.h"

#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

void log_end_program() {
    log_printf(TERMINATE_REPORTS, "exit", "Program closed with errno = %d.\n", errno);
    log_close();
    if (errno == 0) return;
    printf("Error code: %d\n", errno);
    perror("Error");
}

bool check_ptr(const void* ptr) {
    if (ptr == NULL) return false;
    int mem_errno = errno;
    int fd = open("/dev/null", O_WRONLY);
    ssize_t result = write(fd, ptr, 1);
    close(fd);
    errno = mem_errno;
    return result != -1;
}

hash_t get_simple_hash(const void* start, const void* end) {
    hash_t hash = 0xDEADBABEDEAD;
    for (const char* ptr = (const char*)start; ptr < (const char*)end; ++ptr) {
        hash *= 0xC0FEBABEDEAD;
        hash += (unsigned char)*ptr;
    }
    return hash;
}