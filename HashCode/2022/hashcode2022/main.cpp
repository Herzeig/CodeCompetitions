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
//string nomeFile = "a_an_example";
//string nomeFile = "b_basic";
//string nomeFile = "c_coarse";
//string nomeFile = "c_coarse_reworked";
string nomeFile = "d_difficult";
//string nomeFile = "e_elaborate";
string inputFile = nomeFile + ".in.txt";
string outputFile = nomeFile + ".out.txt";
vector<string> soluzione;
map<string, int> ingredientiLike;
map<string, double> ingredientiLikeScore;
map<string, double> ingredientiDislikeScore;
map<string, int> ingredientiDislike;
bool fine = false;
int scoreMax = 0;
int step = 0;
string maxIngrediente = "";
double maxLikeScore=0.0;
int maxDislike = 0;
int iterazioni = 1000;

void leggiFile(void)
{
    ifstream inFl;
    int numClienti, i, num, j;
    string ingrediente;
    string inputFl;

    if(step > 0){
        inputFl = nomeFile + "_" + to_string(step) + ".in.txt";
    }else{
        inputFl = nomeFile + ".in.txt";
    }

    //cout << "leggiFile: " << inputFl << endl;

    inFl.open(inputFl);
    if (inFl)
    {
        ingredientiLike.clear();
        ingredientiDislike.clear();
        ingredientiLikeScore.clear();
        ingredientiDislikeScore.clear();
        maxLikeScore = 0;
        maxIngrediente = "";
        inFl >> numClienti;
        FOR(i, 0, numClienti)
        {
            // Like
            inFl >> num;
            FOR(j, 0, num)
            {
                inFl >> ingrediente;
                if (ingredientiLike.count(ingrediente) == 0)
                {
                    ingredientiLike[ingrediente] = 1;
                    ingredientiLikeScore[ingrediente] = 1.0 / (double)num;
                    //ingredientiDislikeScore[ingrediente] = 1.0 / (double)num;
                }
                else
                {
                    ingredientiLike[ingrediente]++;
                    ingredientiLikeScore[ingrediente] += 1.0 / (double)num;
                    //ingredientiDislikeScore[ingrediente] = 1.0 / (double)num;
                }
                if(ingredientiLikeScore[ingrediente] > maxLikeScore){
                    maxLikeScore = ingredientiLikeScore[ingrediente];
                    maxIngrediente = ingrediente;
                }
            }
            // Dislike
            inFl >> num;
            FOR(j, 0, num)
            {
                inFl >> ingrediente;
                if (ingredientiDislike.count(ingrediente) == 0)
                {
                    ingredientiDislike[ingrediente] = 1;
                }
                else
                {
                    ingredientiDislike[ingrediente]++;
                }
                if(ingredientiDislike[ingrediente]>maxDislike){
                    maxDislike = ingredientiDislike[ingrediente];
                }
            }
        }
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
    outFl << soluzione.size();
    FOR(i, 0, soluzione.size())
    {
        outFl << ' ' << soluzione[i];
    }
    outFl << endl;
    outFl.close();
}

void trovaSoluzione(void)
{
    if(step == 0){
        // Prendo tutti gli ingredienti che non DISLIKE
        for (auto x:ingredientiLike){
            if(ingredientiDislike.count(x.first) == 0){
                soluzione.insert(soluzione.end(), x.first);
            }
        }
    } else {
        // Strategia 1: prendo il punteggio like piu' alto
//        if(maxIngrediente != ""){
//            soluzione.insert(soluzione.end(), maxIngrediente);
//        }else{
//            fine = true;
//        }

        // Strategia 2: prendo il punteggio dislike piu' basso
//        if(ingredientiDislike.size() > 0){
//            auto minimo = min_element(ingredientiDislike.begin(), ingredientiDislike.end(),
//                                      [](const auto& l, const auto& r) { return l.second < r.second; });
//            soluzione.insert(soluzione.end(), minimo->first);
//            if(minimo->second > maxDislike / 2){
//                fine = true;
//            }
//        }else{
//            fine = true;
//        }

        // Strategia 3: random pick
        if(iterazioni >= 1){
            iterazioni--;
            auto it = ingredientiLike.begin();
            std::advance(it, rand()%ingredientiLike.size());
            soluzione.insert(soluzione.end(), it->first);
        }else{
            fine = true;
        }

    }
}

void calcolaStatistica(void)
{
    cout << "LIKE" << endl;
    for (auto x:ingredientiLike)
    {
        cout << x.first << " " << x.second << " Score: " << ingredientiLikeScore[x.first] << endl;
        //soluzione.insert(soluzione.end(), x.first);
    }
    cout << "DISLIKE" << endl;
    for (auto x:ingredientiDislike)
    {
        cout << x.first << " " << x.second << endl;
        //soluzione.insert(soluzione.end(), x.first);
    }
}

void calcolaPunteggio(void)
{
    int score = 0;
    int punto;
    ifstream inFl;
    int numClienti, i, num, j;
    string ingrediente;

    inFl.open(inputFile);
    if (inFl)
    {
        inFl >> numClienti;
        FOR(i, 0, numClienti)
        {
            //cout << i+1 << endl;
            punto = 1;
            // Like
            inFl >> num;
            FOR(j, 0, num)
            {
                inFl >> ingrediente;
                if (find(soluzione.begin(), soluzione.end(), ingrediente) == soluzione.end())
                {
                    punto = 0;
                }
            }
            // Dislike
            inFl >> num;
            FOR(j, 0, num)
            {
                inFl >> ingrediente;
                if (find(soluzione.begin(), soluzione.end(), ingrediente) != soluzione.end())
                {
                    punto = 0;
                }
            }
            score += punto;
        }
        inFl.close();
    } else {
        cout << inputFile << endl;
    }
    cout << "Score: " << score << " su " << numClienti << endl;

    if(score <= scoreMax){
        // Commentato per strategia 3
        // fine = true;
        if (score < scoreMax){
            soluzione.pop_back();
        }
    } else {
        scoreMax = score;
    }
}

void scriviFile(void)
{
    ifstream inFl;
    ofstream outFl;
    int numClienti, i, num, j;
    string ingrediente;
    string inputFl;
    string outputFl;
    vector<string> tmp;

    if(step > 0){
        inputFl = nomeFile + "_" + to_string(step) + ".in.txt";
    }else{
        inputFl = nomeFile + ".in.txt";
    }
    outputFl = nomeFile + "_" + to_string(step + 1) + ".in.txt";

    // "Step: " << step << endl;
    //cout << "Input: " << inputFl << endl;
    //cout << "Output: " << outputFl << endl;

    inFl.open(inputFl);
    outFl.open(outputFl);
    if (inFl)
    {
        inFl >> numClienti;
        outFl << numClienti << endl;
        FOR(i, 0, numClienti)
        {
            // Like
            inFl >> num;
            tmp.clear();
            FOR(j, 0, num)
            {
                inFl >> ingrediente;
                if (find(soluzione.begin(), soluzione.end(), ingrediente) == soluzione.end())
                {
                    tmp.insert(tmp.end(), ingrediente);
                }
            }
            outFl << tmp.size();
            FOR(j, 0, tmp.size())
            {
                outFl << " " << tmp[j];
            }
            outFl << endl;
            // Dislike
            inFl >> num;
            outFl << num;
            FOR(j, 0, num)
            {
                inFl >> ingrediente;
                outFl << " " << ingrediente;
            }
            outFl << endl;
        }
        inFl.close();
    }
    else
    {
        cout << inputFile << endl;
    }

    outFl.close();
}

int main() {
    while (fine == false)
    {
        leggiFile();
        //calcolaStatistica();
        trovaSoluzione();
        calcolaPunteggio();
        if(fine == false) {
            scriviFile();
            step++;
        }
    }
    scriviSoluzione();
    return 0;
}



