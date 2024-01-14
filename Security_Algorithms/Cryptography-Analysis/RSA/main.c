#include "../utils.h"
#include "rsa.h"

int main() {
    // different key sizes
    int key_sizes[] = {2048, 4096, 8192};

    // different messages to encrypt of different sizes
    const char *short_msg = short_message;
    const char *medium_msg = medium_message;
    const char *long_msg = long_message;

    // Call the function to run the performance analysis for RSA with different key sizes and different messages of different sizes
    for(int i = 0; i < 3; i++) {
        puts("Short message");
        run_RSA_performance_analysis(key_sizes[i], short_msg);
        //puts("Medium message");
        //run_RSA_performance_analysis(key_sizes[i], medium_msg);
        //puts("Long message");
        //run_RSA_performance_analysis(key_sizes[i], long_msg);
    }

    return 0;
}