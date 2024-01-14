#ifndef DISSERTATION_PROJECT_UTILS_H
#define DISSERTATION_PROJECT_UTILS_H

#include <openssl/err.h>

void print_openssl_errors();

// export the different messages to sign
extern uint8_t *long_message;
extern uint8_t *short_message;
extern uint8_t *medium_message;

#endif //DISSERTATION_PROJECT_UTILS_H
