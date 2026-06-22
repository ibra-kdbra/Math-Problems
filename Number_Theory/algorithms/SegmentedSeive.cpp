// Include the standard C++ library.
#include <bits/stdc++.h>

// Use the standard C++ namespace.
using namespace std;

// Create a macro for the long long int type.
#define ll long long int

// Create a macro for the maximum value to check for primality.
#define MAX 100001

// Function to return a vector containing all the prime numbers less than or equal to MAX.
vector<int>* Seive() {

    // Create an array to store whether or not each number is prime.
    bool isprime[MAX];

    // Initialize the isprime array so that all numbers are initially marked as prime.
    for (int i = 0; i < MAX; i++) {
        isprime[i] = true;
    }

    // Iterate over all the prime numbers less than or equal to the square root of MAX.
    for (int i = 2; i * i < MAX; i++) {
        // If the current number is prime, then mark all of its multiples as non-prime.
        if (isprime[i]) {
            for (int j = i * i; j < MAX; j += i) {
                isprime[j] = false;
            }
        }
    }

    // Create a new vector to store the prime numbers.
    vector<int>* primes = new vector<int>();

    // Add the first prime number, 2, to the vector.
    primes->push_back(2);

    // Iterate over all the odd numbers less than or equal to MAX and add any prime numbers to the vector.
    for (int i = 3; i < MAX; i += 2) {
        if (isprime[i]) {
            primes->push_back(i);
        }
    }

    // Return the vector containing all the prime numbers less than or equal to MAX.
    return primes;
}

// Function to print all the prime numbers between l and r, inclusive.
void printPrimes(ll l, ll r, vector<int>*& primes) {

    // Create an array to store whether or not each number in the range [l, r] is prime.
    bool isprime[r - l + 1];

    // Initialize the isprime array so that all numbers are initially marked as prime.
    for (int i = 0; i <= r - l; i++) {
        isprime[i] = true;
    }

    // Iterate over all the prime numbers in the vector, up to and including the largest prime number that is less than or equal to the square root of r.
    for (ll i = 0; primes->at(i) * (ll)primes->at(i) <= r; i++) {
        // Get the current prime number from the vector.
        int currprime = primes->at(i);

        // Mark all multiples of the current prime number in the range [l, r] as non-prime.
        for (ll j = l; j <= r; j += currprime) {
            isprime[j - l] = false;
        }

        // There may be a case where the current prime number is itself a multiple of l.
        // In this case, we need to make sure that it is still marked as prime in the isprime array.
        if (l % currprime == 0) {
            isprime[l - l] = true;
        }
    }

    // Print all the prime numbers in the range [l, r] that are still marked as prime in the isprime array.
    for (int i = 0; i <= r - l; i++) {
        if (isprime[i] == true) {
            cout << i + l << endl;
        }
    }
}
// Main function.
int main() {
    // Disable synchronization with stdio to improve performance.
    ios_base::sync_with_stdio(false);

    // Disable tieing cin and cout to streams to improve performance.
    cin.tie(NULL);
    cout.tie(NULL);

    // Create a vector to store the prime numbers.
    vector<int>* primes = Seive();

    // Get the number of test cases.
    int t;
    cin >> t;

    // Process each test case.
    while (t--) {
        // Get the lower and upper bounds for the range of prime numbers to print.
        ll l, r;
        cin >> l >> r;

        // Print all the prime numbers in the range [l, r].
        printPrimes(l, r, primes);
    }

    // Delete the vector of prime numbers.
    delete primes;

    // Return 0 to indicate successful execution of the program.
    return 0;
}
