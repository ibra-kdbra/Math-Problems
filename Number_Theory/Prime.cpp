#include <bits/stdc++.h>
using namespace std;

// Check if a number is prime
bool isprime(int n) {
  // Initialize a counter to keep track of the number of factors.
  int c = 0;

  // Iterate over all the numbers from 1 to the square root of n, inclusive.
  for (int i = 1; i * i <= n; i++) {
    // If the current number is a factor of n, then increment the counter.
    if (n % i == 0) {
      c++;

      // If the current number is the square root of n, then we only need to count it once.
      if (i * i == n) {
        continue;
      }

      // Otherwise, we need to count it twice, since it has two factors (itself and its divisor).
      else {
        c++;
      }
    }
  }

  // If the counter is equal to 2, then the number has only two factors (1 and itself), and is therefore prime.
  // Otherwise, the number has more than two factors, and is therefore not prime.
  return c == 2;
}

int main() {
  // Declare variables.
  int n, count;

  // Get the input.
  cin >> n;

  // Initialize the count of prime numbers.
  count = 0;

  // Iterate over all the numbers from 1 to n, inclusive.
  for (int i = 1; i <= n; i++) {
    // Check if the current number is prime.
    if (isprime(i)) {
      // If it is, increment the count of prime numbers.
      count++;
    }
  }

  // Print the count of prime numbers.
  cout << count << endl;

  // Return 0 to indicate successful execution of the program.
  return 0;
}
