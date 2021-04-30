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

    int w=0;
    cin>>w;
    if((w>3) && (w%2==0)){
        cout << "YES\n";
    } else {
        cout << "NO\n";
    }

    return 0;
}

