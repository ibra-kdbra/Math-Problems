/*  The purpose of each section of code, including initializing the array,
calculating Euler's Totient Phi values using the Sieve of Eratosthenes algorithm,
and printing the values. */

// Include the <bits/stdc++.h> header file. This header file includes a variety of useful libraries, such as the `vector` and `algorithm` libraries.
#include <bits/stdc++.h>

// Import all of the names from the `std` namespace into the current scope. This means that you can use functions and classes from the `std` namespace without having to prefix them with `std::`.
using namespace std;

// Define a macro `pb` to expand to `push_back()`. This macro is useful for adding elements to the end of a vector.
#define pb push_back

// Define a macro `ll` to expand to `long long int`. This macro is useful for defining variables that need to store large integer values.
#define ll long long int

// Function to calculate Euler's Totient Phi values using the Sieve of Eratosthenes algorithm
void eulerSeive(ll n)
{
    /* Initialize the array with numbers from 1 to n */
    ll a[n + 2];
    for (ll i = 0; i <= n + 1; i++) {
        a[i] = i;
    }

    /* Calculate the Euler's Totient Phi values */
    for (ll i = 2; i <= n; i++) {
        if (a[i] == i) {
            /* If a[i] is prime, then mark it as such and update its Phi value */
            a[i] = i - 1;

            /* Mark multiples of i as non-prime and update their Phi values */
            for (ll j = 2 * i; j <= n; j += i) {
                a[j] = (a[j] * (i - 1)) / i;
            }
        } else {
            /* If a[i] is not prime, then its Phi value has already been updated */
            continue;
        }
    }

    /* Print the Euler's Totient Phi values for each number from 1 to n */
    for (ll i = 1; i <= n; i++) {
        cout << "EULER TOTIENT PHI VALUE FOR " << i << " IS " << a[i] << endl;
    }

    cout << endl;
}

int main() {
    /* Read the number of test cases */
    ll t;
    cin >> t;

    /* Call the eulerSeive function to calculate and print Euler's Totient Phi values */
    eulerSeive(t);

    return 0;
}
