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
    int result = 0;
    cin>>t;
    for(int i=1; i<=t; i++){
        string x;
        cin >> x;
        for(auto c : x){
            if(c=='+'){
                result++;
                break;
            }
            if(c=='-'){
                result--;
                break;
            }
        }
    }
    cout<< result <<"\n";
    return 0;
}
