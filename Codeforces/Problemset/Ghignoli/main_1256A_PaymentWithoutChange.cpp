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
        int a, b, n, S;
        int tmp=0;
        cin >> a >> b >> n >> S;
        tmp = S/n;
        if(tmp>a){
            tmp = a;
        }
        S -= (n*tmp);
        if((S==0) || (S<=b)){
            cout << "YES\n";
        } else {
            cout << "NO\n";
        }
    }

    return 0;
}