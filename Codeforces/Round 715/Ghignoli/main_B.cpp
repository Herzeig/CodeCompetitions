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
    cin >> t;
    for(int i=0; i<t; i++){
        int n, expectedM, counter=0;
        string str;
        cin >> n;
        expectedM = n / 3;
        cin >> str;

        for(auto c:str){
            if(c=='T'){
                counter++;
            } else {
                counter--;
            }
            if((counter<0)||(counter>expectedM)){
                break;
            }
        }
        if(counter==expectedM){
            cout << "YES\n";
        } else {
            cout << "NO\n";
        }
    }

    return 0;
}
