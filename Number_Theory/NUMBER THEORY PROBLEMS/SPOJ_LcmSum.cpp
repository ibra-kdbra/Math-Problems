/* Given n, calculate the sum LCM(1,n) + LCM(2,n) + .. + LCM(n,n), where LCM(i,n)
denotes the Least Common Multiple of the integers i and n.

Input:

The first line contains T the number of test cases. Each of the next T lines contain an integer n.

Output:

Output T lines, one for each test case, containing the required sum.
Example: */

#include<bits/stdc++.h>
using namespace std;
#define pb push_back
#define ll long long 
#define n 1000002
ll sumphi[n]={0};
void eulerSeive()
{
    ll a[n+2];
    for(ll i=0;i<=n+1;i++)
    {
        a[i]=i;
    }
    for(ll i=2;i<=n;i++)
    {
        if(a[i]==i)
        {
            a[i]=i-1;
            for(ll j=2*i;j<=n;j+=i)
            {
                a[j]=(a[j]*(i-1))/i;
            }
        }
    }
    /*for(ll i=1;i<=n;i++)
    cout<<"EULER TOTIENT PHI VALUE FOR "<<i<<" IS "<<a[i]<<endl;
    cout<<endl;*/
    for(ll i=1;i<=n;i++)
    {
        for(ll j=i;j<=n;j+=i)
        {
            sumphi[j]+=(i*a[i]); 
        }
    }
}

void func(long long m)
{
	ll x=((sumphi[m]+1)*m)/2;
    cout<<x<<endl;
}
int main()
{
    ll x,t;
    eulerSeive();
    cin>>t;
    while(t--){
    cin>>x;
    func(x);
    }
}
