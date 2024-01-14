cmake_minimum_required(VERSION 3.25)
project(dissertation_project C)

set(CMAKE_C_STANDARD 11)

find_package(OpenSSL REQUIRED)

# include directories for liboqs
include_directories(/usr/local/include)

link_directories(/usr/local/lib)

add_executable(RSA_analysis RSA/main.c RSA/rsa.c RSA/rsa.h
        measure_performance.c
        measure_performance.h
        utils.c
        utils.h
)
target_link_libraries(RSA_analysis liboqs.a OpenSSL::SSL OpenSSL::Crypto)

add_executable(ECC_analysis ECC/main.c
        measure_performance.c
        measure_performance.h
        utils.c
        utils.h)
target_link_libraries(ECC_analysis liboqs.a OpenSSL::SSL OpenSSL::Crypto)

add_executable(Kyber_analysis KEM/kyber.c
        measure_performance.c
        measure_performance.h
        utils.c
        utils.h
        KEM/kem.c
        KEM/kem.h)
target_link_libraries(Kyber_analysis liboqs.a OpenSSL::SSL OpenSSL::Crypto)

add_executable(NTRU_analysis KEM/NTRU.c
        measure_performance.c
        measure_performance.h
        utils.c
        utils.h
        KEM/kem.c
        KEM/kem.h)
target_link_libraries(NTRU_analysis liboqs.a OpenSSL::SSL OpenSSL::Crypto)

add_executable(FrodoKEM_analysis KEM/FrodoKEM.c
        measure_performance.c
        measure_performance.h
        utils.c
        utils.h
        KEM/kem.c
        KEM/kem.h)
target_link_libraries(FrodoKEM_analysis liboqs.a OpenSSL::SSL OpenSSL::Crypto)

add_executable(BIKE_analysis KEM/BIKE.c
        measure_performance.c
        measure_performance.h
        utils.c
        utils.h
        KEM/kem.c
        KEM/kem.h)
target_link_libraries(BIKE_analysis liboqs.a OpenSSL::SSL OpenSSL::Crypto)

add_executable(Falcon_analysis Signature_Schemes/Falcon.c
        measure_performance.c
        measure_performance.h
        utils.c
        utils.h
    Signature_Schemes/signature.c
    Signature_Schemes/signature.h)
target_link_libraries(Falcon_analysis liboqs.a OpenSSL::SSL OpenSSL::Crypto)

add_executable(CRYSTALS_Dilithium_analysis Signature_Schemes/CRYSTALS-Dilithium.c
        measure_performance.c
        measure_performance.h
        utils.c
        utils.h
    Signature_Schemes/signature.c
    Signature_Schemes/signature.h)
target_link_libraries(CRYSTALS_Dilithium_analysis liboqs.a OpenSSL::SSL OpenSSL::Crypto)
