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

    int n=0, tmp, somma=0, totale=0;
    vector<int> a;

    cin>>n;
    for(int i=0; i<n; i++){
        cin >> tmp;
        totale += tmp;
        a.push_back(tmp);
    }
    sort(a.begin(), a.end());
    for(int j=n-1;j>=0;j--){
        somma += a[j];
        totale -= a[j];
        if(somma>totale){
            cout<< (n-j) << "\n";
            break;
        }
    }

    return 0;
}
