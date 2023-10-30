// C++ program to find modular inverse of a under modulo m
// This program works only if m is prime.

// Include the <bits/stdc++.h> header file. This header file includes a variety of useful libraries, such as the `vector` and `algorithm` libraries.
#include <bits/stdc++.h>

// Import all of the names from the `std` namespace into the current scope. This means that you can use functions and classes from the `std` namespace without having to prefix them with `std::`.
using namespace std;

// To find GCD of a and b
int gcd(int a, int b) {
  // Returns the greatest common divisor of two integers.
  if (a == 0) {
    return b;
  }
  return gcd(b % a, a);
}

// To compute x raised to power y under modulo m
int power(int x, unsigned int y, unsigned int m) {
  // Computes x raised to the power of y under modulo m.
  if (y == 0) {
    return 1;
  }
  int p = power(x, y / 2, m) % m;
  p = (p * p) % m;

  return (y % 2 == 0) ? p : (x * p) % m;
}

// Function to find modular inverse of a under modulo m
// Assumption: m is prime
void modInverse(int a, int m) {
  // Finds the modular inverse of a under modulo m, assuming that m is prime.

  // Compute the greatest common divisor of a and m.
  int g = gcd(a, m);

  // If the greatest common divisor is not 1, then the modular inverse does not exist.
  if (g != 1) {
    cout << "Inverse doesn't exist" << endl;
    return;
  }

  // If a and m are relatively prime, then the modular inverse is a^(m-2) mode m.
  cout << "Modular multiplicative inverse is " << power(a, m - 2, m)
       << endl;
}

// Driver Program
int main() {
  // Test the modInverse() function with the values a = 3 and m = 11.
  int a = 3, m = 11;
  modInverse(a, m);
  return 0;
}
