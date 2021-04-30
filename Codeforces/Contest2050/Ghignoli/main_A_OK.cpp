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

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t;
    cin>>t;
    for(int i=0; i<t; i++) {
        ll n, ans=0;
        cin >> n;
        if(n<2050){
            cout << "-1\n";
        } else {
            for(int j=14; j>=0; j--){
                while(n>=(long long)2050*((long long)pow(10,j))){
                    n-=(long long)2050*((long long)pow(10,j));
                    //cout << n << '\n';
                    ans++;
                    if(n<2050){
                        break;
                    }
                }
                if(n<2050){
                    break;
                }
            }
            if(n==0){
                cout << ans << '\n';
            } else {
                cout << "-1\n";
            }
        }
    }

    return 0;
}
