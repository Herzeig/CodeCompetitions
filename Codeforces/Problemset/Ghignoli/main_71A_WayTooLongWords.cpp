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
    for(int i=0; i<t; i++){
        string tmp;
        cin >> tmp;
        if(tmp.length()>10){
            cout << tmp[0] << (tmp.length()-2) << tmp[tmp.length()-1] << '\n';
        } else {
            cout << tmp << '\n';
        }
    }

    return 0;
}

