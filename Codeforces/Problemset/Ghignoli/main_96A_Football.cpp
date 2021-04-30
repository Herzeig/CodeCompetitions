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

    string situation;
    int consecutivi=0;
    cin>>situation;

    for(auto c : situation){
        if(c=='1'){
           if(consecutivi>=0){
               consecutivi++;
           }  else {
               consecutivi = 1;
           }
        } else {
            if(consecutivi<=0){
                consecutivi--;
            }  else {
                consecutivi = -1;
            }
        }
        if((consecutivi>=7)||(consecutivi<=-7)){
            cout<<"YES\n";
            return 0;
        }
    }
    cout<<"NO\n";
    return 0;
}
