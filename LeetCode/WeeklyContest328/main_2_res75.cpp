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
    int totDiff=0, maxLenDiff=1, ret=0, differenza;
    int idxStart=0;
    vector<int> diff(nums.size());
    sort(nums.begin(), nums.end());
    for(int i=1; i<nums.size(); i++){
        diff[i]=nums[i]-nums[i-1];
        differenza = nums[i]-nums[i-maxLenDiff];
        totDiff += differenza;
        if(totDiff<=k){
            maxLenDiff++;
        } else {
            while((totDiff>k)&&(maxLenDiff>1)){
                totDiff -= (nums[i]-nums[i-maxLenDiff]);
                maxLenDiff--;
            };
        }
        if(maxLenDiff>ret){
            ret = maxLenDiff;
        }

    }
    /*for(int j=0; j<nums.size();j++){
        cout << nums[j] << '\n';
            cout << diff[j] << '\n';
    }*/
    return ret;
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

    cout << maxFrequency(a, tt) << '\n';
    return 0;
}
