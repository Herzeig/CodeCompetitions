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
    bool found;
    cin >> t;
    for(int i=0; i<t; i++){
        int n,j;
        string str[3];
        int count1[3];
        int halfCount[3];
        cin >> n;
        cin >> str[0];
        cin >> str[1];
        cin >> str[2];
        found = false;
        for(int i=0; i<3; i++){
            j=0;
            for(auto c:str[i]){
                if(c=='1'){
                    count1[i]++;
                    if(j<n){
                        halfCount[i]++;
                    }
                }
                j++;
            }
            if(count1[i]==n){
                cout << str[i];
                if(i+1<3){
                    for(int k=0; k<n; k++){
                        cout << str[i+1][n+k];
                    }
                } else {
                    for(int k=0; k<n; k++){
                        cout << str[i-1][n+k];
                    }
                }
                cout << '\n';
                found = true;
                break;
            }
        }
        if(found==false) {
            for (int kk = 0; kk < 3; kk++) {
                for(int jj=0; jj < 3; jj++){
                    if(kk!=jj) {
                        if (found == false) {
                            if ((halfCount[jj] < count1[kk]) && (n - halfCount[jj] < 2 * n - count1[kk])) {
                                cout << str[kk];
                                for (int k = 0; k < n; k++) {
                                    cout << str[jj][n + k];
                                }
                                cout << '\n';
                                found = true;
                                break;
                            }
                        }
                    }
                }
            }
        }
    }

    return 0;
}
