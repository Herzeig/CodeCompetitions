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
   ll tmpM, tmpS;

   tmpM = m % 3600000000000;
   tmpS = s
   if(tmpM == )
}

void printOrario(ll h, ll m, ll s){

}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int tt;
    cin>>tt;
    for(int i=0; i<tt; i++){
        ll a, b, c;
        cin >> a >> b >> c;
        //proviamo permutazioni
        // H M S
        if(checkOrario(a,b,c)==true){
            printOrario(a,b,c);
        } else if(checkOrario(a,c,b)==true){
            printOrario(a,c,b);
        } else if(checkOrario(b,a,c)==true){
            printOrario(b,a,c);
        } else if(checkOrario(b,c, a)==true){
            printOrario(b,c, a);
        } else if(checkOrario(c,a,b)==true){
            printOrario(c,a,b);
        } else if(checkOrario(c,b,a)==true){
            printOrario(c,b,a);
        }
    }

    return 0;
}
