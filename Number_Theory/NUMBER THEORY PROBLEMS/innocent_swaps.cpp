#include<bits/stdc++.h>
using namespace std;

#define ll long long int 
#define m 1000000007
#define mod 1000000005

ll f[1000001];

// Function to calculate factorial values
void fact()
{
    f[0]=1;
    f[1]=1;
    f[2]=2;
    for(int i=3;i<=1000000;i++)
    {
        f[i]=(i%m)*(f[i-1]%m)%m;
        //cout<<f[i]<<endl;
    }
}

// Function to calculate modular exponentiation
ll modExpo(ll a, ll b,ll c)
{
    if(a==0)
        return 0;
    if(b==0)
        return 1;
    
    if(b%2==0)
    {
        ll smallans=modExpo(a,b/2,c);
        ll ans;
        ans=(smallans * smallans)%c;
        return (ans+c)%c;
    }
    else
    {
        ll smallans=modExpo(a,b-1,c);
        ll ans;
        ans=(smallans * a%c)%c;
        return (ans+c)%c;
    }
}

int main() 
{
ios_base::sync_with_stdio(false);
cin.tie(NULL);
cout.tie(NULL);
    
    fact(); // Calculate factorial values
    
ll t,n,k,x,y,z,p;
    cin>>t; // Read the number of test cases
    while(t--)
    {
        cin>>n>>k; // Read the values of n and k
        p=modExpo(2,k,m); // Calculate 2^k modulo m
        x=f[n]; // Get the factorial of n
        y=modExpo(f[n-k],mod,m); // Calculate (n-k)! modulo m
        z=modExpo(f[k],mod,m); // Calculate k! modulo m
        ll ans;
        //cout<<p<<" "<<x<<" "<<y<<" "<<z<<endl;
        ans=(x%m*y%m*z%m*p%m)%m; // Calculate the final answer
        cout<<ans<<endl; // Print the answer
    }
}
