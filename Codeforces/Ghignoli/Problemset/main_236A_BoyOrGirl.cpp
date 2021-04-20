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

    string username;
    cin>>username;

    set<char> distinct;
    for(auto c : username){
        distinct.insert(c);
    }
    if(distinct.size()%2==0)
    {
        cout<<"CHAT WITH HER!\n";
    } else {
        cout<<"IGNORE HIM!\n";
    }
    return 0;
}
