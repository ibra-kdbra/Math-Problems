// Function to precompute the primes up to 1000000.
void sieve() {
  // Set all numbers to 1, indicating that they are prime by default.
  for (int i = 0; i <= 1000000; i++) {
    p[i] = 1;
  }

  // Iterate over all the numbers from 2 to 1000000, inclusive.
  for (int i = 2; i <= 1000000; i++) {
    // If the current number is prime, then mark all of its multiples as non-prime.
    if (p[i]) {
      for (int j = 2 * i; j <= 1000000; j += i) {
        p[j] = 0;
      }
    }
  }
}

// Function to count the number of primes in the range [a, b], inclusive.
int segmented_sieve(long long a, long long b) {
  // Precompute the primes up to 1000000.
  sieve();

  // Create an array to store whether or not each number in the range [a, b] is prime.
  bool pp[b - a + 1];
  // Set all numbers in the array to 1, indicating that they are prime by default.
  memset(pp, 1, sizeof(pp));

  // Iterate over all the primes up to the square root of b.
  for (long long i = 2; i * i <= b; i++) {
    // Iterate over all the numbers in the range [a, b], inclusive.
    for (long long j = a; j <= b; j++) {
      // If the current number is divisible by the current prime, then mark it as non-prime.
      if (p[i] && j % i == 0) {
        pp[j - a] = 0;
      }
    }
  }

  // Count the number of prime numbers in the range [a, b].
  int res = 1;
  for (long long i = a; i < b; i++) {
    // If the current number is marked as prime in the array, then increment the count.
    if (pp[i - a]) {
      res++;
    }
  }

  // Return the count of prime numbers in the range [a, b].
  return res;
}
