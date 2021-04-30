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
                if(d<3){
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
                        ++countLevel[0+k];
                        cout << k+1<< endl;
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
                            ++countLevel[1+k];
                            cout << 1+k+1 << endl;
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
                                ++countLevel[2+k];
                                cout << 2+k+1<< endl;
                            } else {
                                for(int pp=0; pp<b; pp++){
                                    if(countLevel[pp]<n){
                                        ++countLevel[pp];
                                        cout << pp+1 << endl;
                                    }
                                }
                            }
                        }
                    }
                } else if(d<6){
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
                        ++countLevel[1+k];
                        cout << 1+k+1<< endl;
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
                            ++countLevel[1+k];
                            cout << 1+k+1<< endl;
                        } else {
                            for(int pp=0; pp<b; pp++){
                                if(countLevel[pp]<n){
                                    ++countLevel[pp];
                                    cout << pp << endl;
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
                        ++countLevel[2+k];
                        cout <<  2+k+1 << endl;
                    } else {
                        for(int pp=0; pp<b; pp++){
                            if(countLevel[pp]<n){
                                ++countLevel[pp];
                                cout <<  pp+1 << endl;
                            }
                        }
                    }
                }
            }
        }
    }
    return 0;
}
