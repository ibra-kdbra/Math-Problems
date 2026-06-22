// Include the <bits/stdc++.h> header file. This header file includes a variety of useful libraries, such as the `vector` and `algorithm` libraries.
#include <bits/stdc++.h>

// Import all of the names from the `std` namespace into the current scope. This means that you can use functions and classes from the `std` namespace without having to prefix them with `std::`.
using namespace std;

// extended euclid algorithm which is application of DIOPHANTINE equation

class triplet {
public:
    int x;
    int y;
    int gcd;
};

triplet extendedEuclid(int a, int b) {
  // If b is 0, then the GCD is a and the coefficients x and y are 1 and 0, respectively.
  if (b == 0) {
    triplet ans;
    ans.gcd = a;
    ans.x = 1;
    ans.y = 0;
    return ans;
  }

  // Recursively compute the extended Euclidean algorithm for b and a % b.
  triplet smallans = extendedEuclid(b, a % b);

  // Update the coefficients x and y using the following formula:
  // x = smallans.y,
  // y = smallans.x - smallans.y * (a / b)

  triplet ans;
  ans.gcd = smallans.gcd;
  ans.x = smallans.y;
  ans.y = smallans.x - smallans.y * (a / b);
  return ans;
}

int main() {
  // Read the two integers a and b.
  int a, b;
  cin >> a >> b;

  // Compute the GCD of a and b, as well as the coefficients x and y.
  triplet ans = extendedEuclid(a, b);

  // Print the GCD.
  cout << ans.gcd << endl;

  // Print the coefficients x and y.
  cout << ans.x << endl;
  cout << ans.y << endl;

  return 0;
}
