// Include the standard C++ library.
#include <bits/stdc++.h>

// Use the standard C++ namespace.
using namespace std;

// Macro to define a long long integer type
#define ll long long

// Macro to define the modulus
#define m 1000000000

// Global variables
vector<ll> b, c;
ll k;

// Function to multiply two matrices A and B of size (k+1) x (k+1) modulo m
vector<vector<ll>> multiply(vector<vector<ll>> A, vector<vector<ll>> B) {
  // Create a new matrix C of size (k+1) x (k+1) to store the result.
  vector<vector<ll>> C(k + 1, vector<ll>(k + 1));

  // Iterate over all the rows and columns of the matrices A, B, and C.
  for (int i = 1; i <= k; i++) {
    for (int j = 1; j <= k; j++) {
      // Calculate the element at row i, column j of the resulting matrix C.
      for (int x = 1; x <= k; x++) {
        C[i][j] = (C[i][j] + (A[i][x] * B[x][j]) % m) % m;
      }
    }
  }

  // Return the resulting matrix C.
  return C;
}

// Function to compute the matrix exponentiation of the matrix T to the power of n modulo m.
vector<vector<ll>> pow(vector<vector<ll>> T, ll n) {
  // If the power is 1, then simply return the matrix T.
  if (n == 1) {
    return T;
  }

  // Check if the power is even or odd.
  if (n & 1) {
    // If the power is odd, then return the product of T and pow(T, n - 1).
    return multiply(T, pow(T, n - 1));
  } else {
    // If the power is even, then return the square of pow(T, n / 2).
    vector<vector<ll>> A = pow(T, n / 2);
    return multiply(A, A);
  }
}

// Function to compute the Nth term of the recurrence relation.
ll compute(ll n) {
  // If n is 0, then return 0.
  if (n == 0) {
    return 0;
  }

  // If n is less than or equal to k, then return the corresponding element of the vector b.
  if (n <= k) {
    return b[n - 1];
  }

  // Create a new vector F1 of size k + 1 to store the first k terms of the recurrence relation.
  vector<ll> F1(k + 1);
  for (int i = 1; i <= k; i++) {
    F1[i] = b[i - 1];
  }

  // Create a new matrix T of size k + 1 x k + 1 to represent the recurrence relation.
  vector<vector<ll>> T(k + 1, vector<ll>(k + 1));

  // Fill the matrix T.
  for (int i = 1; i <= k; i++) {
    for (int j = 1; j <= k; j++) {
      if (i < k) {
        if (j == i + 1) {
          // The element next to the diagonal is 1.
          T[i][j] = 1;
        } else {
          T[i][j] = 0;
        }
      } else {
        // The last row is to be filled in the opposite direction.
        T[i][j] = c[k - j];
      }
    }
  }

  // Compute the matrix exponentiation of T to the power of n - 1 modulo m.
  vector<vector<ll>> M = pow(T, n - 1);

  // Calculate the Nth term of the recurrence relation.
  ll res = 0;
  for (int i = 1; i <= k; i++) {
    res = (res + (M[1][i] * F1[i]) % m) % m;
  }
    //response returned
    return res;
}
int main() {
  // Declare variables
  ll t, n, x;

  // Read the number of test cases from the input
  cin >> t;

  // Iterate over all the test cases
  while (t--) {
    // Read the value of k from the input
    cin >> k;

    // Clear the vectors b and c
    b.clear();
    c.clear();

    // Read the elements of the vector b from the input
    for (int i = 0; i < k; i++) {
      cin >> x;
      b.push_back(x);
    }

    // Read the elements of the vector c from the input
    for (int i = 0; i < k; i++) {
      cin >> x;
      c.push_back(x);
    }

    // Read the value of n from the input
    cin >> n;

    // Compute the Nth term of the recurrence relation
    ll res = compute(n);

    // Print the result to the output
    cout << res << endl;
  }

  // Return 0 from the main() function
  return 0;
}


