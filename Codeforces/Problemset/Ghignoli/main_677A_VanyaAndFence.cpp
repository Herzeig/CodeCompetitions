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

    int n, h;
    int w=0;

    cin>>n>>h;
    for(int i=0; i<n; i++) {
        int tmp;
        cin >> tmp;
        if(tmp<=h){
            w++;
        } else {
            w += 2;
        }
    }
    cout << w << '\n';
    return 0;
}

