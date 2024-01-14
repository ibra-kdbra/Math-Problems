#ifndef DISSERTATION_PROJECT_RSA_H
#define DISSERTATION_PROJECT_RSA_H

#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/bn.h>

RSA* generate_RSA_key_pair(int bits, unsigned long e);
int save_RSA_key_pair(RSA *rsa, const char *private_path, const char *public_path);
RSA* load_RSA_key(const char *path, int is_private);
int encrypt_data(const char *data, RSA *rsa_public, unsigned char *encrypted);
int decrypt_data(int encrypted_length, unsigned char *encrypted, RSA *rsa_private, unsigned char *decrypted);
int run_RSA_performance_analysis(int key_size, const char *data_to_encrypt);

#endif //DISSERTATION_PROJECT_RSA_H
