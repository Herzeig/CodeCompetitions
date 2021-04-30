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

bool isPerfectSquare(long double x)
{
    // Find floating point value of
    // square root of x.
    if (x >= 0) {

        long long sr = sqrt(x);

        // if product of square root
        //is equal, then
        // return T/F
        return (sr * sr == x);
    }
    // else return false if n<0
    return false;
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t, tmp;
    float prod;
    bool found;
    cin>>t;

    for(int i=0; i<t; i++){
        int n;
        cin >> n;
        vector<int> a;
        for(int j=0; j<n; j++){
            cin >> tmp;
            a.push_back(tmp);
        }
        /*if(n==1){
            if(isPerfectSquare(a[0])){
                cout << "NO\n";
            } else {
                cout << "YES\n";
            }
        } else if(n==2){
            if(isPerfectSquare(a[0]*a[1])){
                cout << "NO\n";
            } else {
                cout << "YES\n";
            }
        } else */{
            found = false;
            for(int k=0; k<n; k++){
                if(!isPerfectSquare(a[k])){
                    cout << "YES\n";
                    found = true;
                    break;
                }
            }
            if(found == false){
                cout << "NO\n";
            }
        }
    }

    return 0;
}

