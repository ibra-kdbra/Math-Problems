/* The purpose of each section of code, including reading the number of test cases,
reading the value of n and the large number as a string,
calculating the remainder step by step using the property of GCD,
and printing the greatest common divisor (GCD) of n and p. */

#include<bits/stdc++.h>
using namespace std;
#define ll long long int 

int main(){
ll t,n,m,x,k,i,j,p;
    cin>>t; // Read the number of test cases
    
    while(t--)
    {
        string s;
        cin>>n>>s; // Read the value of n and the large number as a string
        
        p=0; // Initialize the remainder
        
        if(n==0)
        {
            cout<<s<<endl; // If n is 0, the remainder is the same as the large number
            continue;
        }
        
        // Calculate the remainder step by step using the property of GCD: gcd(A,B)=gcd(B, A%B)
        for(i=0;i<s.length();i++)
        {
            x=s[i]-48; // Convert the character to an integer
            p=((p*10) + x%n)%n; // Calculate the remainder step by step using the previous value
        }
        
        cout<<__gcd(n,p)<<endl; // Print the greatest common divisor (GCD) of n and p
    }
    
    return 0;
}
