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

    int tt, n, b, p, d;
    int k;
    cin>>tt>>n>>b>>p;
    for(int i=0; i<tt; i++){
        vector<int> countLevel(b);
        for(int j=0; j<n*b; j++){
            cin >> d;
            if(d==-1){
                return 0;
            } else {
                if(d<4){
                    k=0;
                    while((countLevel[0+k]>=n)&&(k<b-3)){
                        k+=3;
                        if(countLevel[0+k]==countLevel[k-2]){
                            k-=2;
                            break;
                        }
                        if(countLevel[0+k]==countLevel[k-1]){
                            k-=1;
                            break;
                        }
                    }
                    if(countLevel[0+k]<n){
                        cout << ++countLevel[0+k];
                    } else {
                        k=0;
                        while((countLevel[1+k]>=n)&&(k<b-3)){
                            k+=3;
                            if(countLevel[0+k]==countLevel[k-2]){
                                k-=2;
                                break;
                            }
                            if(countLevel[0+k]==countLevel[k-1]){
                                k-=1;
                                break;
                            }
                        }
                        if(countLevel[1+k]<n){
                            cout << ++countLevel[1+k];
                        } else {
                            k=0;
                            while((countLevel[2+k]>=n)&&(k<b-3)){
                                k+=3;
                                if(countLevel[0+k]==countLevel[k-2]){
                                    k-=2;
                                    break;
                                }
                                if(countLevel[0+k]==countLevel[k-1]){
                                    k-=1;
                                    break;
                                }
                            }
                            if(countLevel[2+k]<n){
                                cout << ++countLevel[2+k];
                            } else {
                                for(int pp=0; pp<b; pp+){
                                    if(countLevel[pp]<n){
                                        cout << ++countLevel[pp];
                                    }
                                }
                            }
                        }
                    }
                } else if(d<7){
                    k=0;
                    while((countLevel[1+k]>=n)&&(k<b-3)){
                        k+=3;
                        if(countLevel[1+k]==countLevel[k-2]){
                            k-=2;
                            break;
                        }
                        if(countLevel[1+k]==countLevel[k-1]){
                            k-=1;
                            break;
                        }
                    }
                    if(countLevel[1+k]<n){
                        cout << ++countLevel[1+k];
                    } else {
                        k=0;
                        while((countLevel[1+k]>=n)&&(k<b-3)){
                            k+=3;
                            if(countLevel[1+k]==countLevel[k-2]){
                                k-=2;
                                break;
                            }
                            if(countLevel[1+k]==countLevel[k-1]){
                                k-=1;
                                break;
                            }
                        }
                        if(countLevel[1+k]<n){
                            cout << ++countLevel[1+k];
                        } else {
                            for(int pp=0; pp<b; pp+){
                                if(countLevel[pp]<n){
                                    cout << ++countLevel[pp];
                                }
                            }
                        }
                        }
                } else {
                        k=0;
                        while((countLevel[2+k]>=n)&&(k<b-3)){
                            k+=3;
                            if(countLevel[2+k]==countLevel[k-2]){
                                k-=2;
                                break;
                            }
                            if(countLevel[2+k]==countLevel[k-1]){
                                k-=1;
                                break;
                            }
                        }
                        if(countLevel[2+k]<n){
                            cout << ++countLevel[2+k];
                        } else {
                            for(int pp=0; pp<b; pp+){
                                if(countLevel[pp]<n){
                                    cout << ++countLevel[pp];
                                }
                            }
                        }
                }
            }
        }
    }

    return 0;
}
