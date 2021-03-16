#include <bits/stdc++.h>

using namespace std;

const int MAXN = 1010;
const int MAXC = 155;
char mat[MAXN][MAXN];
int value[MAXN][MAXN];
const int INF = 0x3f3f3f3f;
const long long LLINF = (long long)1e12;

int width, height, c, r;
int row[MAXN], col[MAXN], bounty[MAXN];

const int dx[] = {0, 1, 0, -1};
const int dy[] = {1, 0, -1, 0};

long long dist[MAXC][MAXN][MAXN];
int from[MAXC][MAXN][MAXN];

map<char, int> maps = {{'#', INF}, {'~', 800}, {'*', 200}, {'+', 150}, {'X', 120}, {'_', 100}, {'H', 70}, {'T', 50}};

struct solution
{
    int r_i[MAXC], r_j[MAXC];
    int pair[MAXC];  // customer i connected w/ (r_i[pair[i]], r_j[pair[i]])
} ;

void print_path(FILE *f, int cx, int cy, int ind) {
	static char dc[] = {'L', 'U', 'R', 'D'};
	vector<int> chr;
	long long cost = 0;
	bool flag = true;
    chr.clear();
//    int cx = sol.r_i[sol.pair[ind]], cy = sol.r_j[sol.pair[ind]];
    int tx = row[ind], ty = col[ind];
    
    if (dist[ind][cx][cy] >= LLINF) {
        fprintf(stderr, "%d not connected\n", ind);
        flag = false;
        return;
    }
    fprintf(f, "%d %d ", cy-1, cx-1);
    
    cost -= dist[ind][cx][cy];
    cost += bounty[ind];
    
    while (!(cx == tx && cy == ty)) {
        int fr = from[ind][cx][cy];
        chr.push_back(dc[fr]);
        cx -= dx[fr];
        cy -= dy[fr];
    }
    
    for (char c: chr)
        fprintf(f, "%c", c);
    fprintf(f, "\n");
}

void make_random_coord(int &i, int &j)
{
    do
    {
        i = rand() % height + 1;
        j = rand() % width + 1;
    } while(mat[i][j] == '#');
}

void assign_closest(solution &sol)
{
    for(int i = 1; i <= c; i++)
    {
        int best = 1;
        for(int j = 2; j <= r; j++)
            if(dist[i][sol.r_i[best]][sol.r_j[best]] > dist[i][sol.r_i[j]][sol.r_j[j]])
                best = j;
        sol.pair[i] = best;
    }
}

void move_centres(solution &sol)
{
    vector<int> best_sc(r + 1), sc(r + 1);
    for(int &x : best_sc) x = INF;

    // move clusters to optimal positions
    for(int i = 1; i <= width; i++)
        for(int j = 1; j <= height; j++)
        {
            for(int &x : sc) x = 0;
            for(int k = 1; k <= c; k++)
            {
                sc[sol.pair[k]] += dist[k][i][j];
            }

            for(int k = 1; k <= r; k++)
                if(sc[k] < best_sc[k])
                {
                    best_sc[k] = sc[k];
                    sol.r_i[k] = i; sol.r_j[k] = j;
                }
        }

    // move zero-size clusters to random coords
    for(int i = 1; i <= r; i++)
    {
        bool has_adj = false;
        for(int j = 1; j <= c; j++)
            has_adj = has_adj || (sol.pair[j] == i);
        if(!has_adj)
            make_random_coord(sol.r_i[i], sol.r_j[i]);
    }
}

solution solve_clustering()
{
    solution curr;
    for(int i = 1; i <= r; i++)
        make_random_coord(curr.r_i[i], curr.r_j[i]);

    for(int t = 0; t < 100; t++)
    {
        if(t % 10 == 0) fprintf(stderr, "%02d", t);
        else fprintf(stderr, ".");
        assign_closest(curr);
        move_centres(curr);
    }

    return curr;
}

bool mark[MAXN][MAXN];
typedef pair<long long, pair<int, int>> pqe;
void dijkstra(int id){
	priority_queue<pqe, vector<pqe>, greater<pqe>> pq;
	for(int i = 1; i <= height; i += 1){
		for(int j = 1; j <= width; j += 1){
			dist[id][i][j] = LLINF;
			mark[i][j] = false;
		}
	}
	pq.push({value[row[id]][col[id]], {row[id], col[id]}});
	dist[id][row[id]][col[id]] = value[row[id]][col[id]];
	while(!pq.empty()){
		auto tp = pq.top();
		pq.pop();
		int node_x = tp.second.first;
		int node_y = tp.second.second;
		long long dis = tp.first;
		if(dis != dist[id][node_x][node_y]) continue;
		for(int d = 0; d < 4; d += 1){
			int xt_x = node_x + dx[d];
			int xt_y = node_y + dy[d];
			if(value[xt_x][xt_y] == INF) continue;
			if(mark[xt_x][xt_y]) continue;
			if(dis + value[xt_x][xt_y] < dist[id][xt_x][xt_y]){
				dist[id][xt_x][xt_y] = dis + value[xt_x][xt_y];
				from[id][xt_x][xt_y] = d;
				pq.push({dist[id][xt_x][xt_y], {xt_x, xt_y}});
			}
		}
		mark[node_x][node_y] = true;
	}
	for(int i = 1; i <= height; i += 1){
		for(int j = 1; j <= width; j += 1){
			if(i == row[id] && j == col[id]) continue;
			if(dist[id][i][j] == LLINF) continue;
			dist[id][i][j] -= value[i][j];
		}
	}
}
void precompute(){
	for(int i = 1; i <= c; i += 1){
        if((i-1) % 10 == 0) fprintf(stderr, "%02d", i-1);
        else fprintf(stderr, ".");
		dijkstra(i);
	}
}

long long najbolji_do[MAXC];

void recalc_najbolji(solution &s) {
    for(int i = 1; i <= c; i++) {
        najbolji_do[i] = -INF;
        for(int j = 1; j <= r; j++) {
            if (s.r_i[j] == 0) continue;
            najbolji_do[i] = max(najbolji_do[i], bounty[i] - dist[i][s.r_i[j]][s.r_j[j]]);
        }
    }
}

bool uzet[MAXN][MAXN];
//long long best_to_here[MAXC];
void pick_one(int k, solution &s, int ind) {
    recalc_najbolji(s);
    long long best_score = -1;
    int bi = 0, bj = 0;

    for (int i = 1; i <= height; i++) {
        for (int j = 1; j <= width; j++) {
            if (uzet[i][j]) continue;

            long long score = 0;
            for (int w = 1; w <= c; w++) {
                long long tren = bounty[w] - dist[w][i][j] ;
                if (tren > 0)
                    score += tren + max(0ll, -najbolji_do[w]);
                else
                   if (tren > najbolji_do[w])
                       score += tren - najbolji_do[w];
            }
            if (score > best_score) {
                best_score = score;
                bi = i;
                bj = j;
            }
        }
    }
    uzet[bi][bj] = true;
    s.r_i[ind] = bi;
    s.r_j[ind] = bj;
}

solution gridi(int iters) {
    solution s;
    for (int w = 1; w <= r; w++) s.r_i[w] = s.r_j[w] = 0;

    for(int t = 0; t < iters; t++)
    {
        if(t % 10 == 0) fprintf(stderr, "%02d", t);
        else fprintf(stderr, ".");

        for (int w = 1; w <= c; w++) 
            uzet[row[w]][col[w]] = true;

        vector<int> order;
        for(int i = 1; i <= r; i++) order.push_back(i);
        random_shuffle(order.begin(), order.end());
        
        for (int _i = 0; _i < r; _i++) {
            int i = order[_i];
            uzet[s.r_i[i]][s.r_j[i]] = false;
            s.r_i[i] = s.r_j[i] = 0;
            pick_one(c,s,i); 
        }
    }

    recalc_najbolji(s);
//    for(int i = 1; i <= c; i++) fprintf(stderr, "%10lld%c", najbolji_do[i], i == c ? '\n' : ' ');
    return s;
}

void print_solution(solution &s, FILE *f)
{
    // for(int i = 1; i <= r; i++)
    //     cerr << s.r_i[i] << " " << s.r_j[i] << endl;
    for(int to = 1; to <= c; to++)
    {
        bool used = false;
        int best = 1;
        for(int from = 1; from <= r; from++)
        {
            int ii = s.r_i[from], jj = s.r_j[from];
            if(dist[to][ii][jj] <=  bounty[to])
            {
                used = true;
                print_path(f, ii, jj, to);
            }

            if(dist[to][ii][jj] < dist[to][s.r_i[best]][s.r_j[best]])
                best = from;
        }

        if(!used)
            print_path(f, s.r_i[best], s.r_j[best], to);
    }
}

int main(int argc, char *argv[]) {

    if(argc != 2)
    {
        fprintf(stderr, "Usage: solver <test>\n");
        return 1;
    }

    if(strlen(argv[1]) != 1)
    {
        fprintf(stderr, "Unknown testcase!\n");
        return 1;
    }

    int iters = 100;
    
    FILE *f = NULL;
    switch(argv[1][0])
    {
    case '0':
    	f = fopen("tests/0_example.txt", "r");
    	break;
    case '1':
        f = fopen("tests/1_victoria_lake.txt", "r");
        break;
    case '2':
        f = fopen("tests/2_himalayas.txt", "r");
        break;
    case '3':
        f = fopen("tests/3_budapest.txt", "r");
        break;
    case '4':
        f = fopen("tests/4_manhattan.txt", "r");
        break;
    case '5':
        f = fopen("tests/5_oceania.txt", "r");
        iters = 10;
        break;
    }

    if(f == NULL)
    {
        fprintf(stderr, "Unknown testcase!\n");
        return 1;
    }
    
	fscanf(f, "%d %d", &width, &height);
	fscanf(f, "%d %d", &c, &r);
	for(int i = 0; i <= height + 1; i += 1){
		for(int j = 0; j <= width + 1; j += 1){
			value[i][j] = INF;
		}
	}

	for(int i = 1; i <= c; i += 1){
		fscanf(f, "%d %d %d", &col[i], &row[i], &bounty[i]);
		col[i] += 1;
		row[i] += 1;
	}

	for(int i = 1; i <= height; i += 1){
		fscanf(f, "%s", mat[i] + 1);
		for(int j = 1; j <= width; j += 1){
			value[i][j] =  maps[mat[i][j]];
		}
	}

    fprintf(stderr, "Precomputing...\n");
	precompute();
    fprintf(stderr, "Done.\n");

    fprintf(stderr, "Finding clusters... ");
    solution sol = gridi(iters);
    fprintf(stderr, "Done.\n");

    char out_name[] = "outputs/test_#.out";
    out_name[13] = argv[1][0];
    fprintf(stderr, "Saving output \"%s\"... ", out_name);
    
    FILE *out = fopen(out_name, "w");
    print_solution(sol, out);
    fclose(out);

    fprintf(stderr, "Done.\n");
	return 0;
}
