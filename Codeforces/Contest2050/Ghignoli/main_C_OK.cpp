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

    int n, count, movex, movey;
    cin>>n;
    int chess[n][n];
    for(int i=0; i<n; i++) {
        cin >> chess[i][i];
        for(int k=0; k<i; k++){
            chess[i][k] = 0;
        }
    }
    for(int i=0; i<n; i++){
       count = chess[i][i]-1;
       movex=0;
       movey=0;
       while(count>0){
           if((i-movex-1>=0) && (chess[i+movey][i-movex-1]==0)){
               movex++;
               chess[i+movey][i-movex]=chess[i][i];
           } else {
               movey++;
               chess[i+movey][i-movex]=chess[i][i];
           }
           count--;
       }
    }
    for(int j=0; j<n; j++){
        for(int k=0; k<=j; k++){
            if(k>0) cout << ' ';
            cout << chess[j][k];
        }
        cout << '\n';
    }

    return 0;
}
