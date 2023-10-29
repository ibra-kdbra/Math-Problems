#include <bits/stdc++.h>
using namespace std;
#define pb push_back
#define ll long long int

// Calculates the modular exponentiation of a to the power of b modulo c.
ll modExpo(ll a, ll b, ll c) {
  // If a is 0, then the modular exponentiation is 0.
  if (a == 0) {
    return 0;
  }

  // If b is 0, then the modular exponentiation is 1.
  if (b == 0) {
    return 1;
  }

  // Calculate the modular exponentiation recursively.
  ll ans;
  if (b % 2 == 0) {
    // If b is even, then the modular exponentiation is (a ^ (b / 2)) ^ 2 modulo c.
    ll smallans = modExpo(a, b / 2, c);
    ans = (smallans * smallans) % c;
  } else {
    // If b is odd, then the modular exponentiation is a * (a ^ (b - 1)) modulo c.
    ll smallans = modExpo(a, b - 1, c);
    ans = (smallans * a) % c;
  }

  // Return the modular exponentiation.
  return ans;
}

int main() {
  // Get the input.
  ll a, b, c;
  cin >> a >> b >> c;

  // Calculate the modular exponentiation.
  ll ans = modExpo(a, b, c);

  // Print the output.
  cout << ans << endl;

  // Return 0 to indicate successful execution of the program.
  return 0;
}
