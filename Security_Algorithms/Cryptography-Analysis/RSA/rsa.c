#include "rsa.h"
#include "../measure_performance.h"
#include "../utils.h"

#include <string.h>
#include <stdio.h>

// Function to generate RSA key pair
RSA* generate_RSA_key_pair(int bits, unsigned long e) {
    BIGNUM *bne = BN_new();
    if(BN_set_word(bne, e) != 1){
        BN_free(bne);
        return NULL;
    }

    RSA *rsa = RSA_new();
    if(RSA_generate_key_ex(rsa, bits, bne, NULL) != 1){
        RSA_free(rsa);
        BN_free(bne);
        return NULL;
    }

    BN_free(bne);
    return rsa;
}

// Function to save RSA key pair to files
int save_RSA_key_pair(RSA *rsa, const char *private_path, const char *public_path) {
    FILE *fp = fopen(private_path, "w");
    if(fp == NULL) {
        return 0;
    }
    PEM_write_RSAPrivateKey(fp, rsa, NULL, NULL, 0, NULL, NULL);
    fclose(fp);

    fp = fopen(public_path, "w");
    if(fp == NULL) {
        return 0;
    }
    PEM_write_RSAPublicKey(fp, rsa);
    fclose(fp);

    return 1;
}

// Function to load RSA key pair from files
RSA* load_RSA_key(const char *path, int is_private) {
    FILE *fp = fopen(path, "r");
    if(fp == NULL) {
        return NULL;
    }

    RSA *rsa;
    if(is_private) {
        rsa = PEM_read_RSAPrivateKey(fp, NULL, NULL, NULL);
    } else {
        rsa = PEM_read_RSAPublicKey(fp, NULL, NULL, NULL);
    }
    fclose(fp);

    return rsa;
}

// Function to encrypt data
int encrypt_data(const char *data, RSA *rsa_public, unsigned char *encrypted) {
    int data_len = strlen(data);
    int encrypted_length = RSA_public_encrypt(data_len, (unsigned char*)data, encrypted, rsa_public, RSA_PKCS1_OAEP_PADDING);
    return encrypted_length;
}

// Function to decrypt data
int decrypt_data(int encrypted_length, unsigned char *encrypted, RSA *rsa_private, unsigned char *decrypted) {
    int decrypted_length = RSA_private_decrypt(encrypted_length, encrypted, decrypted, rsa_private, RSA_PKCS1_OAEP_PADDING);
    return decrypted_length;
}

/**
 * @brief This function runs the RSA performance analysis for a given key size (in bits).
 *
 * First, the function generates a RSA key pair and saves it to files.
 * Then, the function loads the RSA key pair from files.
 * Finally, the function encrypts and decrypts a given message.
 *
 * The function measures the time and memory usage for each step.
 * To measure performance, the function uses the functions defined in measure_performance.h.
 * Performance analysis is based on the following parameters: key size, time and memory usage.
 *
 * @param key_size
 * @return
 */
int run_RSA_performance_analysis(int key_size, const char *data_to_encrypt) {
    unsigned long e = RSA_F4; // 65537 or RSA_3 (public exposant)

    char logging_text[100] = {0};

    // Measure memory usage before key generation
    sprintf(logging_text, "RSA key size: %d bits -- Before key generation", key_size);
    measure_memory(logging_text);

    // Start timer for key generation
    start_timer();

    // Generate the RSA key pair
    RSA *rsa = generate_RSA_key_pair(key_size, e);
    if(rsa == NULL){
        fprintf(stderr, "Failed to generate RSA key pair\n");
        print_openssl_errors();
        return 1;
    }

    // Stop timer and print time for key generation
    stop_timer("RSA key size: %d bits -- Key generation", key_size);

    // Measure memory usage after key generation
    snprintf(logging_text, 100, "RSA key size: %d bits -- After key generation", key_size);
    measure_memory(logging_text);

    // Save the RSA Key Pair
    if(!save_RSA_key_pair(rsa, "private.pem", "public.pem")) {
        fprintf(stderr, "Failed to save RSA key pair\n");
        RSA_free(rsa);
        return 1;
    }

    // Load the RSA Key Pair for encryption and decryption
    RSA *rsa_private = load_RSA_key("private.pem", 1);
    RSA *rsa_public = load_RSA_key("public.pem", 0);
    if(rsa_private == NULL || rsa_public == NULL) {
        fprintf(stderr, "Failed to load RSA key pair\n");
        RSA_free(rsa);
        RSA_free(rsa_private);
        RSA_free(rsa_public);
        return 1;
    }

    // Encrypt Data
    unsigned char encrypted[4098] = {};

    // Start timer for encryption
    start_timer();

    int encrypted_length = encrypt_data(data_to_encrypt, rsa_public, encrypted);
    if (encrypted_length == -1) {
        fprintf(stderr, "Failed to encrypt data_to_encrypt\n");
        RSA_free(rsa);
        RSA_free(rsa_private);
        RSA_free(rsa_public);
        return 1;
    }

    // Stop timer and print time for encryption
    stop_timer("RSA key size: %d bits -- Encryption", key_size);

    // Measure memory usage after encryption
    snprintf(logging_text, 100, "RSA key size: %d bits -- After encryption", key_size);
    measure_memory(logging_text);

    // Decrypt Data
    unsigned char decrypted[4098] = {};

    // Start timer for decryption
    start_timer();

    int decrypted_length = decrypt_data(encrypted_length, encrypted, rsa_private, decrypted);
    if (decrypted_length == -1) {
        fprintf(stderr, "Failed to decrypt data_to_encrypt\n");
        RSA_free(rsa);
        RSA_free(rsa_private);
        RSA_free(rsa_public);
        return 1;
    }

    // Stop timer and print time for decryption
    stop_timer("RSA key size: %d bits -- Decryption", key_size);

    // Measure memory usage after decryption
    snprintf(logging_text, 100, "RSA key size: %d bits -- After decryption", key_size);
    measure_memory(logging_text);

    // Print the encrypted and decrypted data_to_encrypt
    printf("[RSA key size: %d bits -- Decrypted data_to_encrypt : %s]\n\n--------------------------------\n", key_size, decrypted);

    // Clear sensitive data_to_encrypt from memory
    memset(encrypted, 0, sizeof(encrypted));
    memset(decrypted, 0, sizeof(decrypted));

    RSA_free(rsa);
    RSA_free(rsa_private);
    RSA_free(rsa_public);

    return 0;
}