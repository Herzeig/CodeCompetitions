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

    long long n, tmp, idx_up, idx_down, min, max, median, tmpMin, tmpMax;
    long long minimo=0;
    bool up = false;
    vector<int> s;
    cin>>n;
    for(int i=0; i<n; i++) {
        cin >> tmp;
        s.push_back(tmp);
    }
    sort(s.begin(), s.end());
    median = idx_up = idx_down = s.size() / 2;
    min = s[median];
    max = s[median];

    while((idx_down>0) || (idx_up<s.size()-1)){
        if(idx_up+1<s.size()){
            tmpMax = s[idx_up+1] - min;
        } else {
            tmpMax = INT64_MAX;
        }
        if(idx_down - 1 >= 0){
            tmpMin = max - s[idx_down-1];
        } else {
            tmpMin=INT64_MAX;
        }
        if(tmpMin<tmpMax){
            up = false;
        } else {
            up = true;
        }
        if(up){
            if(idx_up+1<s.size()){
                idx_up++;
               max = s[idx_up];
               minimo = minimo + (max-min);
                cout << "Minimo: " << minimo << '\n';
            }
        } else {
            if (idx_down - 1 >= 0) {
                idx_down--;
                min = s[idx_down];
                minimo = minimo + (max - min);
                cout << "Minimo: " << minimo << '\n';
            }
        }
        up = !up;
        cout << idx_down << ' ' << idx_up << '\n';
        cout << min << ' ' << max << '\n';
        cout << '\n';
    }

    cout << minimo <<'\n';

    return 0;
}
