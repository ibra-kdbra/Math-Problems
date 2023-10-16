// The code calculates the sum of all distinct divisors for each test 
// case using dynamic programming and prints the final sum

#include<iostream>
using namespace std;
#define m 1000000007

int main(){
long int t,n,k,j,l,x,i;
    cin>>t; // Read the number of test cases
    
    while(t--)
    {
        cin>>n; // Read the size of the input array
        long int a[n+2];
        long int b[750005]={0}; // Auxiliary array to mark the elements
        
        long int s=0; // Variable to store the final sum
        
        // Read the input array and mark the elements in the auxiliary array
        for(i=0;i<n;i++)
        {
            cin>>a[i];
            b[a[i]]=1;
        }
        
        // Calculate the sum of all distinct divisors using dynamic programming
        for(i=1;i<=750003;i++)
        {
            if(b[i]>0)
            {
                s=(s%m+b[i]%m)%m; // Add the current element to the sum
                
                // Update the sum for all multiples of i
                for(j=2*i;j<=750003;j+=i)
                {
                    if(b[j]>0)
                    {
                        b[j]=(b[j]%m+b[i]%m)%m; // Update the sum for multiples of i
                    }
                }
            }
        }
        
        cout<<s<<endl; // Print the final sum
    }
    
    return 0;
}
