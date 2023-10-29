// Include the standard C++ library.
#include <bits/stdc++.h>

// Use the standard C++ namespace.
using namespace std;

// Extended Euclidean algorithm which is application of DIOPHANTINE equation

// Class to represent a triplet of integers
class triplet {
public:
  int x;
  int y;
  int gcd;
};

// Extended Euclidean algorithm to find the greatest common divisor of two numbers and the coefficients of Bézout's identity
triplet extendedEuclid(int a, int b) {
  // If b is 0, then the greatest common divisor of a and b is a, and the coefficients of Bézout's identity are 1 and 0
  if (b == 0) {
    triplet ans;
    ans.gcd = a;
    ans.x = 1;
    ans.y = 0;
    return ans;
  }

  // Recursive call to find the greatest common divisor of b and a % b
  triplet smallans = extendedEuclid(b, a % b);

  // Use the following formula to calculate the coefficients of Bézout's identity for a and b, given the coefficients for b and a % b
  triplet ans;
  ans.gcd = smallans.gcd;
  ans.x = smallans.y;
  ans.y = smallans.x - smallans.y * (a / b);

  return ans;
}

// Function to find the multiplicative modular inverse of a modulo m
int mminverse(int a, int m) {
  // Call the extended Euclidean algorithm to find the coefficients of Bézout's identity for a and m
  triplet ans = extendedEuclid(a, m);

  // The multiplicative modular inverse of a modulo m is the coefficient of x in Bézout's identity
  return ans.x;
}

int main() {
  // Get the input
  int a, m;
  cin >> a >> m;

  // Find the multiplicative modular inverse of a modulo m
  int x = mminverse(a, m);

  // Print the output
  cout << "MULTIPLICATIVE MODULAO INVERSE " << x << endl;

  return 0;
}
