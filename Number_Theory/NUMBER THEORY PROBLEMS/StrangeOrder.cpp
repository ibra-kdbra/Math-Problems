/* The purpose of each section of code, including the Sieve of Eratosthenes algorithm,
finding prime factors, inserting multiples in the vector, sorting the vector,
and clearing the vector for the next iteration. */

#include<bits/stdc++.h>
using namespace std;

int main() 
{
    int n,x,z,y;
    cin>>n; // Read the value of n
    
    int prime[n+1]={0}; // Array to store the least prime factor for each number
    
    // Sieve of Eratosthenes algorithm to find the least prime factor
    for(int i=2;i<=n;i++)
    {
        if(prime[i]==0)
        {
            // Mark all multiples of i with i as their least prime factor
            for(int j=i;j<=n;j+=i)
            {
                if(prime[j]==0)
                    prime[j]=i;
            }
        }
    }
    
    // Uncomment the following code to print the least prime factor for each number
    /*for(int i=1;i<=20;i++)
        cout<<prime[i]<<" ";
    cout<<endl;*/
    
    vector<int > v; // Vector to store the multiples of a number
    
    int a[n+2]={0}; // Array to mark the numbers that have been processed
    
    for(int i=n;i>=1;i--)
    {
        if(a[i]!=0)
            continue;
        if(i==1)
        {
            cout<<1<<" ";
            continue;
        }
        
        // Find the prime factors of the current number and insert its multiples in the vector
        x=prime[i];  // Least prime factor
        v.clear(); // Clear the vector for each iteration
        
        y=i;
        while(y!=1)
        {
            for(int j=i;j>=1;j=j-x)
            {
                if(a[j]!=1)
                {
                    a[j]=1; // Mark the number as processed
                    v.push_back(j); // Insert the multiple in the vector
                }
                else
                    continue;
            }
            
            while(y%x==0)
            {
                y=y/x;
            }
            
            x=prime[y]; // Update the least prime factor
        }
        
        sort(v.rbegin(),v.rend()); // Sort the vector in descending order
        
        for(auto it:v)
        {
            cout<< it <<" "; // Print each element of the vector
        }
        
        v.clear(); // Clear the vector for the next iteration
        
    }
    
    return 0;
}


/*
//----SEIVE--------
    for(int i=2;i*i<=2000001;i++)
    { 
        for(int j=i*i;j<=2000001;j+=i)
        { 
            if(sieve[j]>i)
            { 
                sieve[j]=i; 
                          
            } 
        } 
    }
*/