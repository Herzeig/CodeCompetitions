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

struct lunghezza{
    int i;
    int j;
    int b;

    bool operator<(const lunghezza& rhs) const
    {
        if (b < rhs.b) {
            return true;
        }
        return false;
    };
};

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t;
    cin>>t;
    for(int i=0; i<t; i++) {
        int n,m, iTmp;
        lunghezza tmp;
        cin >> n >> m;
        vector<lunghezza> b;
        int soluzione[n][m];
        bool usato[n][m];
        for(int j=0; j<n; j++){
            for(int k=0; k<m; k++){
                tmp.i=j;
                tmp.j=k;
                cin >> tmp.b;
                b.push_back(tmp);
                usato[j][k]=false;
                soluzione[j][k]=0;
            }
        }
        sort(b.begin(), b.end());
        for(int jj=0; jj<m; jj++){
            //minimo è b[jj] in posizione i,j
            soluzione[b[jj].i][jj]=b[jj].b;
            usato[b[jj].i][jj]=true;
        }
        for(int nn=0; nn<n; nn++){
            int cc = m;
            for(int mm=0; mm<m; mm++){
                if(usato[nn][mm]==false){
                    while(b[cc].i!=nn){
                        cc++;
                    }
                    soluzione[nn][mm] = b[cc].b;
                    cc++;
                }
            }
        }
        for(int nn=0; nn<n; nn++){
            for(int mm=0; mm<m; mm++){
                if(mm>0) cout << ' ';
                cout << soluzione[nn][mm];
            }
            cout << '\n';
        }
    }

    return 0;
}
