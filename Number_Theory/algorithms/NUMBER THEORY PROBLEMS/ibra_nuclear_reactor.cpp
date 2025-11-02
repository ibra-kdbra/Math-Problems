/* The purpose of each section of code, including calculating factorial values,
handling different cases based on the values of n and m, and printing the final result. */

#include<bits/stdc++.h>
using namespace std;
#define ll long long int

ll m;
ll f[100002];

// Function to calculate factorial values
void fact()
{
    f[0]=1;
    f[1]=1;
    f[2]=2;
    
    // Calculate factorial values for i from 3 to 100000
    for(int i=3;i<=100000;i++)
    {
        f[i]=(i%m)*(f[i-1]%m)%m;
        //cout<<f[i]<<endl;
    }
}

int main() 
{
    ll t,n,x;
    
    cin>>t; // Read the number of test cases
    
    while(t--)
    {
        cin>>n>>m; // Read the values of n and m
        
        fact(); // Calculate factorial values
        
        if(n<m)
        {
            cout<<f[n]<<endl; // If n is less than m, print f[n]
        }
        else 
        {
            ll a=n/m;
            
            if(a%2==0)
            {
                x=n%m;
                cout<<f[x]<<endl; // If a is even, print f[x]
            }
            else
            {
                x=(m-1)*(f[n%m]);
                cout<<x%m<<endl; // If a is odd, print (m-1)*f[n%m] % m
            }            
        }
    }
    
    return 0;
}
