//#include <bits/stdc++.h>

#include <unordered_set>
#include <algorithm>
#include <random>
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
#define TO_ASSIGNED 0
#define WAITING_START 1
#define IN_PROGRESS 2
#define COMPLETED 3
#define CONTR_AVAILABLE -1
//string nomeFile = "00-example";
//string nomeFile = "01-the-cloud-abyss";
//string nomeFile = "02-iot-island-of-terror";
string nomeFile = "03-etheryum";
//string nomeFile = "04-the-desert-of-autonomous-machines";
//string nomeFile = "05-androids-armageddon";
string inputFile = nomeFile + ".txt";
string outputFile = nomeFile + "_out.txt";

#define TO_DEFEAT 0
#define FIGTHED 1
#define SKIPPED 2

struct Daemon {
    int idx;
    int staminaToBeat;
    int turnToRecover;
    int staminaRecovered;
    int turnsGainedFragment;
    int totalFragmentsValue;
    double medianFragmentsValue;
    double recoverScore;
    double distributionScore;
    double quality;
    int status;
};

struct Player {
    int actualStamina;
    int maxStamina;
};

struct Stats {
    long maxScore;
    long maxStaminaToBeat;
    long minStaminaToBeat;
    long totalStamina;
};

int numTurns;
vector<Daemon> daemons;
vector<int> demonsList;
struct Player player;
struct Stats stats;

//bool acompare(project lhs, project rhs) { return lhs.best < rhs.best; };
//bool acompare(project lhs, project rhs) { return lhs.days < rhs.days; };
//bool acompare(project lhs, project rhs) { return lhs.score > rhs.score; };
//bool bcompare(contributor lhs, contributor rhs) { return lhs.skillLevels.size() < rhs.skillLevels.size(); };

//bool acompare(Daemon lhs, Daemon rhs) { return rhs.staminaToBeat > lhs.staminaToBeat; };
bool DaemonQuality(Daemon lhs, Daemon rhs) { return lhs.quality > rhs.quality; };

bool DaemonComparer(const Daemon& p1, Daemon& p2)
{
    if (p1.staminaToBeat < p2.staminaToBeat){
        return true;
    } else if (p1.staminaToBeat == p2.staminaToBeat) {
        if (p1.recoverScore > p2.recoverScore){
            return true;
        } else if (p1.recoverScore == p2.recoverScore) {
            if (p1.medianFragmentsValue > p2.medianFragmentsValue) {
                return true;
            } else if (p1.medianFragmentsValue == p2.medianFragmentsValue){
                if (p1.distributionScore > p2.distributionScore) {
                    return true;
                } else {
                    return false;
                }
            } else {
                return false;
            }
        } else {
            return false;
        }
    } else {
        return false;
    }
}

#define PERC_PARZIALE 20

void leggiFile(void)
{
    ifstream inFl;
    int numDemons, tmp, somma, parziale;
    struct Daemon dem;
    stats.maxScore = 0;
    stats.totalStamina = 0;
    stats.maxStaminaToBeat = 0;
    stats.minStaminaToBeat = INF;

    inFl.open(inputFile);
    if (inFl) {
        // Lettura variabili
        inFl >> player.actualStamina >> player.maxStamina >> numTurns >> numDemons;
        for(int i=0; i<numDemons;i++){
            dem.idx = i;
            inFl >> dem.staminaToBeat >> dem.turnToRecover >> dem.staminaRecovered >> dem.turnsGainedFragment;
            dem.status = TO_DEFEAT;
            stats.totalStamina += dem.staminaToBeat;
            if(dem.staminaToBeat > stats.maxStaminaToBeat){
                stats.maxStaminaToBeat = dem.staminaToBeat;
            }
            if(dem.staminaToBeat < stats.minStaminaToBeat){
                stats.minStaminaToBeat = dem.staminaToBeat ;
            }
            if(dem.turnsGainedFragment > 0){
                somma = 0;
                parziale = 0;
                for(int j = 0; j<dem.turnsGainedFragment; j++){
                    inFl >> tmp;
                    //dem.fragments.push_back(tmp);
                    somma += tmp;
                    if(j < dem.turnsGainedFragment * PERC_PARZIALE / 100){
                        parziale += tmp;
                    }
                }
                dem.distributionScore = (double)parziale / somma;
                dem.medianFragmentsValue = (double)somma / dem.turnsGainedFragment;
            } else {
                dem.distributionScore = 0.0;
                dem.medianFragmentsValue = 0.0;
            }
            stats.maxScore += somma;
            dem.totalFragmentsValue = somma;
            dem.recoverScore = (double)dem.staminaRecovered / dem.turnToRecover;
            if(dem.staminaToBeat > 0){
                //dem.quality = dem.distributionScore * dem.totalFragmentsValue * dem.recoverScore / dem.staminaToBeat;
                //dem.quality = dem.distributionScore * dem.medianFragmentsValue * dem.recoverScore;
                // Best 04
                // dem.quality = dem.distributionScore * dem.medianFragmentsValue * dem.recoverScore / dem.staminaToBeat;
                // Best 03
                //dem.quality = dem.totalFragmentsValue;
                // Best 05 - 01 - 02
                dem.quality = dem.totalFragmentsValue / dem.staminaToBeat;
                // Ammazzali tutti da migliorare
                // dem.quality = dem.recoverScore / dem.staminaToBeat;
                //
                //dem.quality = dem.medianFragmentsValue / dem.staminaToBeat;
            } else {
                dem.quality = INF;
            }
            if(dem.totalFragmentsValue > 4800){
                daemons.push_back(dem);
            }
        }
        //std::sort(daemons.begin(), daemons.end(), DaemonComparer);
        //std::sort(daemons.begin(), daemons.end(), DaemonQuality);
        inFl.close();
        cout << "MaxScore: " << stats.maxScore << endl;
        cout << "Max Stamina: " << stats.maxStaminaToBeat << endl;
        cout << "Min Stamina: " << stats.minStaminaToBeat << endl;
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
    for(int i=0; i<demonsList.size(); i++){
        outFl << demonsList[i] << endl;
    }
    outFl.close();
}

void trovaSoluzione(void)
{
    vector<pair<int, int>> toRecover;
    //vector<pair<int, int>>::iterator it;

    std::random_device rd;
    std::mt19937 g(rd());
    shuffle(daemons.begin(), daemons.end(),g);

    for(int j=0; j<numTurns; j++){
        // Recover stamina
        for (auto it = toRecover.begin(); it != toRecover.end(); ++it) {
            if (it->first == j){
                player.actualStamina += it->second;
                if(player.actualStamina > player.maxStamina){
                    player.actualStamina = player.maxStamina;
                }
            }
        }
        // Selezione demone to fight
        for(int i=0; i<daemons.size(); i++){
            if(daemons[i].status != TO_DEFEAT){
                continue;
            }
            //if((daemons[i].staminaToBeat <= player.actualStamina) && ((daemons[i].turnsGainedFragment * PERC_PARZIALE / 100 <= numTurns - j) && (daemons[i].distributionScore < 0.5))){
            if(daemons[i].staminaToBeat <= player.actualStamina){
                demonsList.push_back(daemons[i].idx);
                daemons[i].status = FIGTHED;
                // Togliere stamina al player
                player.actualStamina -= daemons[i].staminaToBeat;
                toRecover.push_back(make_pair(daemons[i].turnToRecover+j, daemons[i].staminaRecovered));
                break;
            }
        }
        if (demonsList.size() == daemons.size()){
            break;
        }
    }
}

void calcolaPunteggio(void)
{

}

void calcolaStatistica(void)
{

}

int main() {
    int sizeSoluzione = 0;
    leggiFile();
    calcolaStatistica();
    while (sizeSoluzione < 990){
        demonsList.clear();
        trovaSoluzione();
        cout << demonsList.size() << endl;
        if(demonsList.size() > sizeSoluzione) sizeSoluzione = demonsList.size();
    }
    calcolaPunteggio();
    scriviSoluzione();
    return 0;
}
