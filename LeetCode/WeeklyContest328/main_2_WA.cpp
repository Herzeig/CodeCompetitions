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
    int differenza, totDiff=0, ret=0;
    int idxStart=0;
    sort(nums.begin(), nums.end());
    for(int i=1; i<nums.size(); i++){
        differenza = nums[i]-nums[idxStart];
        totDiff += differenza;
        if(totDiff>k){
            while((totDiff>k)&&(idxStart<=i)){
                totDiff -= (nums[i]-nums[idxStart]);
                idxStart++;
            };
        }
        if(i-idxStart>ret){
            ret = i-idxStart;
        }

    }
    for(int j=0; j<nums.size();j++) {
        cout << nums[j] << '\n';
        if(j>0){
        cout << nums[j]-nums[j-1] << '\n';
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

    cout << maxFrequency(a, tt) << '\n';
    return 0;
}
