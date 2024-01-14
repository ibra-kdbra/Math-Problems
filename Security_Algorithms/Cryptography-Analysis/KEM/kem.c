#include "kem.h"
#include <oqs/oqs.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <openssl/aes.h> // Used for AES encryption/decryption
#include <openssl/evp.h> // Used for AES encryption/decryption

#include "../measure_performance.h"
#include "../utils.h"


// Function to encrypt plaintext using AES and the shared secret
void aes_encrypt(const uint8_t *plaintext, int plaintext_len, uint8_t *ciphertext, uint8_t *key) {
    unsigned char iv[AES_BLOCK_SIZE] = {0}; // Initialization vector set to zero
    int ciphertext_len;

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    EVP_EncryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, iv);
    EVP_EncryptUpdate(ctx, ciphertext, &ciphertext_len, plaintext, plaintext_len);
    int temp_len;
    EVP_EncryptFinal_ex(ctx, ciphertext + ciphertext_len, &temp_len);
    ciphertext_len += temp_len;
    EVP_CIPHER_CTX_free(ctx);
}

// Function to decrypt ciphertext using AES and the shared secret
void aes_decrypt(uint8_t *ciphertext, int ciphertext_len, uint8_t *decryptedtext, uint8_t *key) {
    unsigned char iv[AES_BLOCK_SIZE] = {0}; // Initialization vector set to zero
    int decryptedtext_len;

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    EVP_DecryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, iv);
    EVP_DecryptUpdate(ctx, decryptedtext, &decryptedtext_len, ciphertext, ciphertext_len);
    int temp_len;
    EVP_DecryptFinal_ex(ctx, decryptedtext + decryptedtext_len, &temp_len);
    decryptedtext_len += temp_len;
    EVP_CIPHER_CTX_free(ctx);

    decryptedtext[decryptedtext_len] = '\0'; // Null-terminate the decrypted text
}

int run_KEM_performance_analysis(const char *kem_variant, const uint8_t *plaintext, KEM_performance_metrics *kem_performance_metrics) {
    int plaintext_len = (int) strlen((const char *) plaintext);
    int result = 0;

    /**
     * Declare all variables used in the KEM.
     * This is useful for the cleanup section where we free all the memory, checking for NULL pointers.
     */
    uint8_t *kem_ciphertext = NULL;
    uint8_t *shared_secret_encap = NULL;
    uint8_t *shared_secret_decap = NULL;
    uint8_t *public_key = NULL;
    uint8_t *secret_key = NULL;
    uint8_t *ciphertext = NULL;
    uint8_t *decryptedtext = NULL;

    // Measure memory usage before any operations
    kem_performance_metrics->before_any_operations = measure_memory("KEM %s -- Before any operations", kem_variant);

    // Initialize KEM
    OQS_KEM *kem = OQS_KEM_new(kem_variant);
    if (kem == NULL) {
        fprintf(stderr, "Failed to initialize KEM\n");
        return 1;
    }

    // Key generation
    public_key = malloc(kem->length_public_key);
    secret_key = malloc(kem->length_secret_key);
    if (public_key == NULL || secret_key == NULL) {
        fprintf(stderr, "Failed to allocate memory for public key or secret key\n");
        result = 1;
        goto cleanup;
    }
    start_timer();
    OQS_KEM_keypair(kem, public_key, secret_key);
    kem_performance_metrics->keypair_generation_time = stop_timer("KEM %s -- Key generation", kem_variant);
    kem_performance_metrics->after_key_generation = measure_memory("KEM %s -- After key generation", kem_variant);

    // Encapsulation
    kem_ciphertext = malloc(kem->length_ciphertext);
    shared_secret_encap = malloc(kem->length_shared_secret);
    if (kem_ciphertext == NULL || shared_secret_encap == NULL) {
        fprintf(stderr, "Failed to allocate memory for ciphertext or shared secret\n");
        result = 1;
        goto cleanup;
    }
    start_timer();
    OQS_KEM_encaps(kem, kem_ciphertext, shared_secret_encap, public_key);
    kem_performance_metrics->encapsulation_time = stop_timer("KEM %s -- Encapsulation", kem_variant);
    kem_performance_metrics->after_encapsulation = measure_memory("KEM %s -- After encapsulation", kem_variant);

    // Decapsulation
    shared_secret_decap = malloc(kem->length_shared_secret);
    if (shared_secret_decap == NULL) {
        fprintf(stderr, "Failed to allocate memory for shared secret\n");
        result = 1;
        goto cleanup;
    }
    start_timer();
    OQS_KEM_decaps(kem, shared_secret_decap, kem_ciphertext, secret_key);
    kem_performance_metrics->decapsulation_time = stop_timer("KEM %s -- Decapsulation", kem_variant);
    kem_performance_metrics->after_decapsulation = measure_memory("KEM %s -- After decapsulation", kem_variant);

    // Verify that the shared secrets match
    if (memcmp(shared_secret_encap, shared_secret_decap, kem->length_shared_secret) != 0) {
        fprintf(stderr, "Shared secrets don't match!\n");
    } else {
        printf("Shared secrets match.\n");
    }

    // Allocate memory for the ciphertext and decrypted text
    ciphertext = calloc(plaintext_len + AES_BLOCK_SIZE, sizeof(uint8_t));
    decryptedtext = calloc(plaintext_len + AES_BLOCK_SIZE, sizeof(uint8_t));
    if (ciphertext == NULL || decryptedtext == NULL) {
        fprintf(stderr, "Failed to allocate memory for ciphertext or decrypted text\n");
        result = 1;
        goto cleanup;
    }

    // Encrypt the plaintext with AES using the shared secret from KEM encapsulation
    start_timer();
    aes_encrypt(plaintext, plaintext_len, ciphertext, shared_secret_encap);
    kem_performance_metrics->encryption_time = stop_timer("KEM %s -- AES encryption", kem_variant);
    kem_performance_metrics->after_encryption = measure_memory("KEM %s -- After AES encryption", kem_variant);

    // Decrypt the ciphertext with AES using the shared secret from KEM decapsulation
    start_timer();
    aes_decrypt(ciphertext, plaintext_len, decryptedtext, shared_secret_decap);
    kem_performance_metrics->decryption_time = stop_timer("KEM %s -- AES decryption", kem_variant);
    kem_performance_metrics->after_decryption = measure_memory("KEM %s -- After AES decryption", kem_variant);

    // Verify that the decrypted ciphertext matches the original plaintext
    if (memcmp(plaintext, decryptedtext, 16) != 0) {
        fprintf(stderr, "Decrypted text doesn't match the original plaintext!\n");
    } else {
        printf("Decrypted text matches the original plaintext.\n");
    }

    kem_performance_metrics->after_all_operations = measure_memory("KEM %s -- After all operations", kem_variant);
    puts("-------------------------------------");

    // Cleanup section to avoid memory leaks
    cleanup:
    if (ciphertext != NULL)
        free(ciphertext);
    if (decryptedtext != NULL)
        free(decryptedtext);
    if (public_key != NULL)
        free(public_key);
    if (secret_key != NULL)
        free(secret_key);
    if (kem_ciphertext != NULL)
        free(kem_ciphertext);
    if (shared_secret_encap != NULL)
        free(shared_secret_encap);
    if (shared_secret_decap != NULL)
        free(shared_secret_decap);
    if (kem != NULL)
        OQS_KEM_free(kem);

    return result;
}
