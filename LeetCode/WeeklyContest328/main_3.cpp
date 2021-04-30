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

char next(char actual){
    switch(actual){
        case 'a':
            return 'e';
        case 'e':
            return 'i';
        case 'i':
            return 'o';
        case 'o':
            return 'u';
        case 'u':
            return 'u';
        default:
            return 'a';
    }
}
int longestBeautifulSubstring(string word) {
    int ret=0, lenSeq=0;
    bool reach = false;
    char actual='a';
    for(int i=0; i<word.size();i++){
        if((word[i]!=actual)&&(lenSeq>0)){
            if(word[i]==next(actual)){
                actual = next(actual);
            } else {
                actual = 'a';
                lenSeq = 0;
            }
        }
        if(word[i]==actual){
            lenSeq++;
            if(actual=='u'){
                reach = true;
            }
        }
        if((lenSeq>ret)&&(reach==true)){
            ret = lenSeq;
        }
    }
    if(reach==true){
        return ret;
    } else {
        return 0;
    }
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    string a;
    cin >> a;
    cout << longestBeautifulSubstring(a) << '\n';
    return 0;
}
