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

bool checkOrario(ll h, ll m, ll s){
    if(12*((h)%3600000000000LL) == (m)){
        if(12*720*((m)%3600000000000LL)==(s)){
            return true;
        }
    }
    return false;
}

void printOrario(ll h, ll m, ll s){
    cout << h/3600000000000LL << ' ';
    cout << m*60/3600000000000LL/12LL << ' ';
    cout << s*60/3600000000000LL/720LL << ' ';
    cout << s%(3600000000000LL*720LL) << '\n';
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int tt;
    cin>>tt;
    for(int i=0; i<tt; i++){
        ll a, b, c;
        cin >> a >> b >> c;
        for(ll rot=0; rot<43200000000000LL; rot+=10000000LL){
            if(checkOrario(a+rot,b+rot,c+rot)==true){
                printOrario(a+rot,b+rot,c+rot);
                break;
            } else if(checkOrario(a+rot,c+rot,b+rot)==true){
                printOrario(a+rot,c+rot,b+rot);
                break;
            } else if(checkOrario(b+rot,a+rot,c+rot)==true){
                printOrario(b+rot,a+rot,c+rot);
                break;
            } else if(checkOrario(b+rot,c+rot, a+rot)==true){
                printOrario(b+rot,c+rot, a+rot);
                break;
            } else if(checkOrario(c+rot,a+rot,b+rot)==true){
                printOrario(c+rot,a+rot,b+rot);
                break;
            } else if(checkOrario(c+rot,b+rot,a+rot)==true){
                printOrario(c+rot,b+rot,a+rot);
                break;
            }
        }
    }

    return 0;
}
