// The purpose of each section of code,
// including marking multiples of cubes as non-cube-free numbers,
// assigning unique numbers to cube-free numbers,
// and processing each test case.

#include<bits/stdc++.h>
using namespace std;
#define ll long long int 

int main()
{
    ll a[1000000]={0};
    
    // Mark all multiples of cubes as non-cube-free numbers
    for(ll i=2;i<=100;i++)
    {
        ll p=pow(i,3);
        for(ll j=p;j<=1000000;j+=p)
        {
            a[j]=1;
        }
    }
    
    ll b[1000000]={0};
    ll k=1;
    
    // Assign unique numbers to cube-free numbers
    for(ll i=1;i<=1000000;i++)
    {
        if(a[i]==0)
        {
            b[i]=k++;
        }
        else
        {
            b[i]=-1;
        }
    }
    
    ll t,m;
    cin>>t;
    
    // Process each test case
    for(ll i=1;i<=t;i++)
    {
        cin>>m;
        
        if(b[m]==-1)
        {
            cout<<"Case "<<i<<": Not Cube Free"<<endl;
        }
        else
        {
            cout<<"Case "<<i<<": "<<b[m]<<endl;
        }
    }
    
    return 0;
}
