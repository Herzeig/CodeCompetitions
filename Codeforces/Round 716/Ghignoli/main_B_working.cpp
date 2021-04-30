//#include <bits/stdc++.h>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <ctime>
#include <cmath>
#include <iterator>
#include <cstdlib>
using namespace std;

#define ll long long
int binomialCoeff(int n, int k)
{
    // Base Cases
    if (k > n)
        return 0;
    if (k == 0 || k == n)
        return 1;

    // Recur
    return (binomialCoeff(n - 1, k - 1)
            + binomialCoeff(n - 1, k)) % (1000000000+7);
}

unsigned int factorial(unsigned int n)
{
    if (n == 0)
        return 1;
    return n * factorial(n - 1);
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t;
    cin>>t;

    for(int i=0; i<t; i++){
        int n, k, ans=0, tmp, comb, per;
        cin >> n >> k;
        if(k>n){
            tmp = n;
        } else {
            tmp = k;
        }
        for(int j=0; (j<=k) && (j<=n); j++){
            //calcolo combinazioni K su j
            comb = binomialCoeff(tmp, j);
            if(comb<n){

                ans += ((comb*(factorial(n-j))) % (1000000000+7));
            } else {
                break;
            }
        }
        cout << ans << '\n';
    }

    return 0;
}