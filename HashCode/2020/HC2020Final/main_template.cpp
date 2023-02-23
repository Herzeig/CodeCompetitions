//#include <bits/stdc++.h>

#include <fstream>
#include <set>
#include <iterator>

#include<cstdio>
#include<set>
#include<vector>
#include<algorithm>
#include<queue>
#include<map>
#include<cstdlib>
#include<time.h>
#include<string>
#include<stack>
#include<cmath>
#include<iostream>
#include<cstring>
#include<complex>
using namespace std;
#define FOR(i,j,k) for(int i=j;i<k;i++)
#define FORD(i,j,k) for(int i=j;i>=k;i--)
#define ll long long
//Make sure no overflow problems
#define pii  pair<int, int>
#define vi vector<int >
#define pb push_back
#define mp make_pair
#define x first
#define y second
#define VAR(i,n) __typeof(n) i = (n)
#define FOREACH(i,c) for(VAR(i,(c).begin());i!=(c).end();i++)
#define FORDEACH(i,c) for( VAR(i,(c).rbegin()),i!=(c).rend();i++)
#define REP(i,n) FOR(i,0,n)
#define ld long double
const int INF = 1000000009;
const long long INFLL = (ll)INF * (ll)INF;
const ld EPS = 10e-9;
///////////////////////////////////////////////////////////////
string nomeFile = "a_example";
//string nomeFile = "b_";
//string nomeFile = "c_";
//string nomeFile = "d_";
//string nomeFile = "e_";
//string nomeFile = "f_";
string inputFile = nomeFile + ".txt";
string outputFile = nomeFile + "out.txt";

void leggiFile(void)
{
    ifstream inFl;
    // Variabili locali

    inFl.open(inputFile);
    if (inFl) {
        // Lettura variabili
        inFl.close();
    }
    else
    {
        cout << inputFile << endl;
    }
}

void scriviSoluzione(void)
{
    ofstream outFl;

    outFl.open(outputFile);
    // Scrivi soluzione in file

    outFl.close();
}

void trovaSoluzione(void)
{

}

void calcolaPunteggio(void)
{

}

void calcolaStatistica(void)
{

}

int main() {
    leggiFile();
    calcolaStatistica();
    trovaSoluzione();
    calcolaPunteggio();
    scriviSoluzione();
    return 0;
}
