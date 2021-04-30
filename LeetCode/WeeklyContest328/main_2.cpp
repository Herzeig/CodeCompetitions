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


int maxFrequency(vector<int>& nums, int k) {
    int totDiff=0, maxLenDiff=0, ret=0;
    vector<int> diff(nums.size());
    sort(nums.begin(), nums.end());
    for(int i=1; i<nums.size(); i++){
        diff[i]=nums[i]-nums[i-1];
        if(diff[i]<=k){
            totDiff += diff[i];
            if(totDiff<=k){
                totDiff += diff[i];
                maxLenDiff++;
            } else {
                while((totDiff>k)&&(maxLenDiff>=1)){
                    totDiff -= diff[i-maxLenDiff];
                    maxLenDiff--;
                };
            }
            if(maxLenDiff>ret){
                ret = maxLenDiff;
            }
        } else {
            break;
        }
    }
    return ret+1;
};


int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int tt, size, tmp;
    vector<int> a;
    cin>>tt;
    cin >>size;
    for(int i=0; i<size; i++){
        cin >> tmp;
        a.push_back(tmp);
    }

    cout << maxFrequency(a, tt);
    return 0;
}
