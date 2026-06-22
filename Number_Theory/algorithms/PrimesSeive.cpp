// Include the standard C++ library.
#include <bits/stdc++.h>

// Use the standard C++ namespace.
using namespace std;

// Find primes in [1, N] using extra space with time complexity O(N log (log (N)))
int seive(int n) {
  // Initialize a boolean array to store whether or not each number in the range [1, N] is prime.
  bool a[n + 1];
  // Set all numbers to true, indicating that they are prime by default.
  for (int i = 0; i <= n + 1; i++) {
    a[i] = true;
  }

  // Mark 0 and 1 as non-prime.
  a[0] = a[1] = false;

  // Iterate over all the numbers from 2 to the square root of N, inclusive.
  for (int i = 2; i * i <= n; i++) {
    // If the current number is prime, then mark all of its multiples as non-prime.
    if (a[i] == true) {
      for (int j = i * i; j <= n; j += i) {
        a[j] = false;
      }
    }
  }

  // Count the number of prime numbers in the range [1, N].
  int count = 0;
  for (int i = 2; i <= n; i++) {
    if (a[i] == true) {
      count++;
    }
  }

  // Return the count of prime numbers in the range [1, N].
  return count;
}

int main() {
  // Declare variables.
  int n, count;

  // Get the input.
  cin >> n;

  // Count the number of prime numbers in the range [1, N].
  count = seive(n);

  // Print the output.
  cout << count << endl;

  // Return 0 to indicate successful execution of the program.
  return 0;
}
