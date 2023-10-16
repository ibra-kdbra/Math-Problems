/* The purpose of each section of code, including finding primes using the Sieve of Eratosthenes algorithm,
calculating the number of times a prime number occurs in a given number,
and calculating the product of factors of all primes in the range [1, n]. */

#include<bits/stdc++.h>
using namespace std;
#define ll long long int
#define m 1000000007

// FIND PRIMES IN [1,N] USING EXTRA SPACE WITH TIME COMPLEXITY O(N LOG (LOG (N)))
vector<int> v;

void seive(ll n)
{
    ll c=0;
    bool a[n+1];
    
    // Initialize all elements as true
    for(ll i=0;i<=n+1;i++){
        a[i]=true;
    }
    
    a[0]=a[1]=false; // 0 and 1 are not prime
    
    // Sieve of Eratosthenes algorithm to find primes
    for(ll i=2;i*i<=n;i++)
    {
        if(a[i]==true)
        {
            // Mark all multiples of i as false
            for(ll j=i*i;j<=n;j+=i)
            {
                a[j]=false;
            }
        }
    }
    
    // Store the primes in the vector v
    for(ll i=2;i<=n;i++)
    {
        if(a[i]==true)
        {
            v.push_back(i);
        }
    }
}

// Calculate the number of times a prime number occurs in a given number
ll factors(ll p,ll n)
{
    ll s=0,z,x=1;
    z=p;
    
    // Calculate the power of p in n
    while(z<=n)
    {
        s+=n/z;
        x++;
        z=pow(p,x);
    }
    
    return s+1;
}

int main()
{
    ll t,c,n,i,j,k,x;
    c=0;
    cin>>t; // Read the number of test cases
    
    while(t--)
    {
        cin>>n; // Read the value of n
        
        seive(n); // Find primes up to n
        
        k=1; // Initialize the result
        
        // Calculate the product of factors of all primes in the range [1, n]
        for(i=0;i<v.size();i++)
        {
            x=factors(v[i],n);
            k=(k%m*x%m)%m;
        }
        
        cout<<k<<endl; // Print the result
        v.clear(); // Clear the vector for the next test case
    }
    
    return 0;
}
