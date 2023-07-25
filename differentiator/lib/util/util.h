#ifndef UTIL_H
#define UTIL_H

#define ARR_SIZE(array) ( sizeof(array) / sizeof(*array) )

static const double CMP_EPS = 1e-10;

/**
 * @brief Check if two doubles are equal to each other.
 * 
 * @param alpha 
 * @param beta 
 * @return true if values are within CMP_EPS from each other
 */
bool is_equal(double alpha, double beta);

/**
 * @brief Return cover if value is NULL, or value otherwise.
 * 
 * @param value 
 * @param cover 
 * @return value (cover if value is NULL)
 */
void* cover_null(void* value, void* cover);

/**
 * @brief GCD of two numbers.
 * 
 * @param alpha 
 * @param beta 
 * @return greatest common divisor
 */
unsigned long long gcd(unsigned long long alpha, unsigned long long beta);

#endif