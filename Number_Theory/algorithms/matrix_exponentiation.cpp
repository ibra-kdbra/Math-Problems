// Include the <bits/stdc++.h> header file. This header file includes a variety of useful libraries, such as the `vector` and `algorithm` libraries.
#include <bits/stdc++.h>

// Import all of the names from the `std` namespace into the current scope. This means that you can use functions and classes from the `std` namespace without having to prefix them with `std::`.
using namespace std;

// Define a macro `pb` to expand to `push_back()`. This macro is useful for adding elements to the end of a vector.
#define pb push_back

// Define a macro `ll` to expand to `long long int`. This macro is useful for defining variables that need to store large integer values.
#define ll long long int

// Function to multiply two 2x2 matrices.
void multiply(ll a[2][2], ll m[2][2]) {
  // Calculate the element at row 0, column 0 of the resulting matrix.
  ll firstvalue = a[0][0] * m[0][0] + a[0][1] * m[1][0];

  // Calculate the element at row 0, column 1 of the resulting matrix.
  ll secondvalue = a[0][0] * m[1][0] + a[0][1] * m[1][1];

  // Calculate the element at row 1, column 0 of the resulting matrix.
  ll thirdvalue = a[1][0] * m[0][0] + a[1][1] * m[1][0];

  // Calculate the element at row 1, column 1 of the resulting matrix.
  ll fourthvalue = a[1][0] * m[0][1] + a[1][1] * m[1][1];

  // Update the matrix a with the new values.
  a[0][0] = firstvalue;
  a[0][1] = secondvalue;
  a[1][0] = thirdvalue;
  a[1][1] = fourthvalue;
}

// Function to raise a 2x2 matrix to the power of n.
void power(ll a[2][2], ll n) {
  // If n is 0 or 1, then simply return.
  if (n == 0 || n == 1) {
    return;
  }

  // Recursively compute the power of n / 2.
  power(a, n / 2);

  // Multiply the matrix a by itself n / 2 times.
  multiply(a, a);

  // If n is odd, then multiply the matrix a by the matrix m.
  if (n % 2 != 0) {
    ll m[2][2] = {{1, 1}, {1, 0}};
    multiply(a, m);
  }
}

// Function to compute the Nth fibonacci number.
ll febo(ll n) {
  // If n is 0, then return 0.
  if (n == 0) {
    return 0;
  }

  // Create a 2x2 matrix a to represent the recurrence relation.
  ll a[2][2] = {{1, 1}, {1, 0}};

  // Compute the power of the matrix a to the power of n - 1.
  power(a, n - 1);

  // Return the element at row 0, column 0 of the resulting matrix.
  return a[0][0];
}

// Main function.
int main() {
  // Print the 0th Fibonacci number.
  cout << febo(0) << endl;

  // Return 0 from the main() function.
  return 0;
}
