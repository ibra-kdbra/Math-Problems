// Include the <bits/stdc++.h> header file. This header file includes a variety of useful libraries, such as the `vector` and `algorithm` libraries.
#include <bits/stdc++.h>

// Import all of the names from the `std` namespace into the current scope. This means that you can use functions and classes from the `std` namespace without having to prefix them with `std::`.
using namespace std;

// Function to calculate the greatest common divisor of two integers a and b
int gcd(int a, int b) {
  // Special case: If a is less than b, swap them so that a is always greater than or equal to b.
  if (a < b) {
    return gcd(b, a);
  }

  // If b is equal to 0, then the greatest common divisor is a.
  if (b == 0) {
    return a;
  }

  // Otherwise, recursively compute the greatest common divisor of b and a % b.
  return gcd(b, a % b);

  /*
  Alternatively, you can use the built-in function __gcd() to calculate the greatest common divisor of two integers.
  However, this function is not available on all compilers, so it is generally better to use your own implementation of the Euclidean algorithm.
  */
}

