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
        int tmp, n;
        vector<int> odd, even;
        cin >> n;
        for(int j=0; j<n; j++){
           cin >> tmp;
           if(tmp%2==0){
               even.push_back(tmp);
           } else {
               odd.push_back(tmp);
           }
        }
        for(int k=0; k<odd.size(); k++){
            cout<< odd[k] << ' ';
        }
        for(int k=0; k<even.size(); k++){
            cout<< even[k];
            if(k!=even.size()-1){
                cout<< ' ';
            }
        }
        cout << '\n';
    }

    return 0;
}
