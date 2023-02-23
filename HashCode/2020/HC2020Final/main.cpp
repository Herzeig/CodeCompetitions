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
#include <random>
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
//string nomeFile = "a_example";
//string nomeFile = "b_single_arm";
//string nomeFile = "c_few_arms";
string nomeFile = "d_tight_schedule";
//string nomeFile = "e_dense_workspace";
//string nomeFile = "f_decentralized";
string inputFile = nomeFile + ".txt";
string outputFile = nomeFile + "_out.txt";
int cols, rows, cnt_mounts, cnt_arms, cnt_tasks, cnt_turns, total_score=0;
vector<pair<int, int>> mountPoints;
//vector<vector<pair<int, int>>> tasks;
vector<int> taskScores;
vector<vector<int>> taskCostPerMountPoint;
FILE *fp;
FILE * fpout;

vector<vector<vector<pair<int, int>>>> neis;
vector<vector<int>> when_can;
vector<vector<bool>> is_mount;

int man_dist(pair<int, int> a, pair<int, int> b){return abs(a.first - b.first) + abs(a.second - b.second);}
bool inside(int row, int col){return 0 <= row && row < rows && 0 <= col && col < cols;}
vector<pair<int, int>> delta{{0,-1}, {0,1}, {1,0}, {-1,0}};

struct Task {
    int id;
    int score;
    vector<pair<int, int>> goals;
    void read(int _id){
        id = _id;
        fscanf(fp,"%d", &score);
        int cnt_goals;
        fscanf(fp,"%d", &cnt_goals);
        for(int i=0; i<cnt_goals;i++){
            int x,y;
            fscanf(fp,"%d%d", &x, &y);
            goals.emplace_back(x, y);
        }
    }
};
vector<Task> tasks;

vector<pair<int, int>> bfs(pair<int, int> start, pair<int, int> goal, int turn){
    static vector<vector<int>> vis;
    static vector<vector<pair<int, int>>> prv;
    if(vis.empty()){
        vis.resize(rows, vector<int>(cols));
        prv.resize(rows, vector<pair<int, int>>(cols));
    }
    vector<pair<int, int>> kol;
    kol.push_back(start);
    vis[start.first][start.second] = 1;
    for(int i=0; i <(int) kol.size(); ++i){
        pair<int,int> a = kol[i];
        for(pair<int,int> nei : neis[a.first][a.second]){
            if(!vis[nei.first][nei.second] && when_can[nei.first][nei.second] <= turn + vis[a.first][a.second]){
                vis[nei.first][nei.second] = vis[a.first][a.second] + 1;
                prv[nei.first][nei.second] = a;
                kol.push_back(nei);
                if(nei == goal){
                    break;
                }
            }
        }
        if(kol.back() == goal){
            break;
        }
    }
    if(kol.back() != goal){
        for(pair<int,int> p : kol){
            vis[p.first][p.second] = false;
        }
        return {};
    }
    vector<pair<int,int>> rev_path;
    while(goal != start){
        rev_path.push_back(goal);
        goal = prv[goal.first][goal.second];
    }
    reverse(rev_path.begin(), rev_path.end());
    for(pair<int,int> p : kol){
        vis[p.first][p.second] = false;
    }
    return rev_path;
}



struct Arm {
    pair<int, int> mount;
    vector<pair<int, int>> stack;
    vector<pair<int, int>> moves;
    vector<int> tasks_done;
    int turn;
    void print () {
        auto [curx , cury] = mount;
        fprintf(fpout,"%d %d %d %d\n", cury, curx, (int)tasks_done.size(), (int)moves.size());
        for (int x:tasks_done){
            fprintf(fpout,"%d ", x);
        }
        //puts("");
        fprintf(fpout,"\n ");
        for(int i=0; i<(int)moves.size() ; i++){
            auto [newx , newy] = moves[i];
            int dx = newx - curx, dy = newy - cury;
            if (dx == 0 and dy == 0) fprintf (fpout,"W ");
            else if (dx == 1 and dy == 0) fprintf (fpout,"U ");
            else if (dx == -1 and dy == 0) fprintf (fpout,"D ");
            else if (dx == 0 and dy == 1) fprintf (fpout,"R ");
            else if (dx == 0 and dy == -1) fprintf (fpout,"L ");
            curx = newx; cury = newy;
        }
        fprintf(fpout,"\n");
    }
    void backuj(){
        stack.pop_back();
        moves.push_back(stack.back());
    }
    void forward (int row, int col){
        stack.emplace_back(row, col);
        moves.emplace_back(row, col);
    }
    void forward (pair<int, int> p){
        forward(p.first, p.second);
    }
    Arm(pair<int,int> _mount){
        mount = _mount;
        stack.push_back(mount);
        turn = 0;
    }
    pair<int,int> me() {
        return stack.back();
    }
    bool solve (bool do_print){
        set<int> forbidden;
        while(true){
            auto memo_moves = moves;
            pair<double,int> best_task{-1e-9, -1};
            for(int tid = 0; tid < (int)tasks.size(); ++tid){
                if(forbidden.count(tasks[tid].id)){
                    continue;
                }
                // if(!task_taken[tid]){
                    int total_len = 0;
                    int max_dist = 0;
                    for(pair<int,int> p: tasks[tid].goals){
                        total_len += man_dist(mount, p);
                        max_dist = max(max_dist, man_dist(mount, p));
                    }
                //}
                best_task = max(best_task, make_pair((double) tasks[tid].score / total_len, tid));
            }
            int tid = best_task.second;
            if(tid == -1){
                break;
            }
            Task task = tasks[tid];
            bool cant = false;
            for(pair<int,int> goal:task.goals){
                int row = stack.back().first;
                int col = stack.back().second;
                vector<pair<int,int>> path = bfs(make_pair(row, col), goal, (int) moves.size());
                if(path.empty()){
                    cant = true;
                    break;
                }
                for(pair<int,int>p:path){
                    forward(p.first, p.second);
                }
                while((int) stack.size()>=2){
                    backuj();
                }
            }
            if(cant){
                moves=memo_moves;
                forbidden.insert(task.id);
                continue;
            }
            if((int)moves.size()>cnt_turns){
                //stack.resize();
                moves = memo_moves;
                forbidden.insert(task.id);
                continue;
            }
            else {
                tasks_done.push_back(task.id);
                total_score += task.score;
                tasks.erase(tasks.begin()+tid);
                for(int i=0;i<(int)moves.size();i++){
                    int& tmp = when_can[moves[i].first][moves[i].second];
                    tmp = max(tmp, i + 2);
                }
                return true;
            }
        }
        return false;
    }
};

void recompute_neis() {
    neis.clear();
    neis.resize(rows, vector<vector<pair<int,int>>>(cols));
    for(int row = 0; row < rows; row ++){
        for(int col = 0; col < cols; col++){
            for(int dir = 0; dir < 4; dir ++){
                int r2 = row + delta[dir].first;
                int c2 = col + delta[dir].second;
                if(inside(r2, c2) && !is_mount[r2][c2]){
                    neis[row][col].emplace_back(r2, c2);
                }
            }
        }
    }
}

int main(int argc, char* argv[]){

    fp = fopen(inputFile.c_str(),"r");
    fpout = fopen(outputFile.c_str() ,"w");


    fscanf(fp,"%d%d", &cols, &rows);
    when_can.resize(rows, vector<int>(cols));
    is_mount.resize(rows, vector<bool>(cols));
    fscanf(fp,"%d%d%d%d", &cnt_arms, &cnt_mounts, &cnt_tasks, &cnt_turns);
    vector<pair<int,int>> mounts(cnt_mounts);
    for(pair<int,int>& p:mounts){
        int x,y;
        fscanf(fp,"%d%d", &x, &y);
        p = {y, x};
        int row = y, col = x;
        is_mount[row][col] = true;
    }



    tasks.resize(cnt_tasks);
    for(int i=0; i<cnt_tasks; i++){
        tasks[i].read(i);
    }

    fclose(fp);

   std::random_device rd;
    std::mt19937 g(rd());

    vector<Arm> arms;
    srand(atoi(argv[2]));
    shuffle(mounts.begin(), mounts.end(), g);

    //int ile = min(atoi(argv[1]), cnt_arms);
    int ile = min(1000, cnt_arms);

    for(int arm_id = 0; arm_id < ile; arm_id++){
        int mount_id = arm_id;
        Arm arm(mounts[mount_id]);
        arms.push_back(arm);
    }
    recompute_neis();
    set<int> remaining;
    for(int i = 0; i<(int)arms.size();i++){
        remaining.insert(i);
    }
    while(true){
//        if((int) remaining.size() == 1){
//            when_can = vector<vector<int>>(row, vector<int>(cols));
//        }
        pair<int,int> best{INT_MAX, -1};
        for(int i:remaining){
            best = min(best, make_pair((int)arms[i].moves.size(), i));
        }
        int who = best.second;
        if(who == -1){
            break;
        }
        if(!arms[who].solve(false)){
            remaining.erase(who);
        }
    }
    vector<Arm> useful_arms;
    for (Arm arm:arms){
        if(!arm.tasks_done.empty()){
            while(arm.moves.back() == arm.moves.end()[-2]){
                arm.moves.pop_back();
            }
            useful_arms.push_back(arm);
        }
    }
    arms = useful_arms;
    fprintf(fpout,"%d\n", (int) arms.size());
    for(Arm arm:arms){
        arm.print();
    }
    fclose(fpout);
    cerr << total_score << arms.size() << endl;

    return 0;
}
