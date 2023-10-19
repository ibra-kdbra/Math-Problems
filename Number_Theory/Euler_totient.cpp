/*  The purpose of each section of code, including initializing the array,
calculating Euler's Totient Phi values using the Sieve of Eratosthenes algorithm,
and printing the values. */
#include<bits/stdc++.h>
using namespace std;
#define pb push_back
#define ll long long int

// Function to calculate Euler's Totient Phi values using the Sieve of Eratosthenes
void eulerSeive(ll n)
{
    ll a[n+2];
    
    // Initialize the array with numbers from 1 to n
    for(ll i=0;i<=n+1;i++)
    {
        a[i]=i;
    }
    
    // Calculate the Euler's Totient Phi values
    for(ll i=2;i<=n;i++)
    {
        if(a[i]==i)
        {
            a[i]=i-1;
            
            // Mark multiples of i as non-prime and update their Euler's Totient Phi values
            for(ll j=2*i;j<=n;j+=i)
            {
                a[j]=(a[j]*(i-1))/i;
            }
        }
        else
        {
            continue;
        }
    }
    
    // Print the Euler's Totient Phi values for each number from 1 to n
    for(ll i=1;i<=n;i++)
    {
        cout<<"EULER TOTIENT PHI VALUE FOR "<<i<<" IS "<<a[i]<<endl;
    }
    
    cout<<endl;
}

int main()
{
    ll t;
    cin>>t; // Read the number of test cases
    
    eulerSeive(t); // Call the eulerSeive function to calculate and print Euler's Totient Phi values
    
    return 0;
}
