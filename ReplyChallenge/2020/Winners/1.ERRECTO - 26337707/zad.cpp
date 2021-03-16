//~ while (clock()<=69*CLOCKS_PER_SEC)
//~ #pragma comment(linker, "/stack:200000000")
#pragma GCC optimize("O3")
//~ #pragma GCC target ("avx2")
//~ #pragma GCC optimize("Ofast")
//~ #pragma GCC target("sse,sse2,sse3,ssse3,sse4,popcnt,abm,mmx,avx,tune=native")
//~ #pragma GCC optimize("unroll-loops")
#include <bits/stdc++.h>
#include <sys/time.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>

#define ALL(x) (x).begin(), (x).end()
#define SZ(x) ((int)(x).size())
#define st first
#define nd second

using namespace __gnu_pbds;
using namespace std;

template <typename T>
using ordered_set =
    tree<T, null_type, less<T>, rb_tree_tag, tree_order_statistics_node_update>;

#define sim template < class c
#define ris return * this
#define dor > debug & operator <<
#define eni(x) sim > typename \
  enable_if<sizeof dud<c>(0) x 1, debug&>::type operator<<(c i) {
sim > struct rge { c b, e; };
sim > rge<c> range(c i, c j) { return rge<c>{i, j}; }
sim > auto dud(c* x) -> decltype(cerr << *x, 0);
sim > char dud(...);
struct debug {
#ifdef LOCAL
~debug() { cerr << endl; }
eni(!=) cerr << boolalpha << i; ris; }
eni(==) ris << range(begin(i), end(i)); }
sim, class b dor(pair < b, c > d) {
  ris << "(" << d.first << ", " << d.second << ")";
}
sim dor(rge<c> d) {
  *this << "[";
  for (auto it = d.b; it != d.e; ++it)
    *this << ", " + 2 * (it == d.b) << *it;
  ris << "]";
}
#else
sim dor(const c&) { ris; }
#endif
};
#define imie(...) " [" << #__VA_ARGS__ ": " << (__VA_ARGS__) << "] "

#define shandom_ruffle random_shuffle

using ll=long long;
using pii=pair<int,int>;
using pll=pair<ll,ll>;
using vi=vector<int>;
using vll=vector<ll>;
const int nax=1000*1007;

#ifdef LOCAL
mt19937 rng(unsigned(69));
#else
mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
#endif
#define rand rng

//for time
double pam_time;

void init_time()
{
	timeval tv;
	gettimeofday(&tv, 0);
	pam_time=tv.tv_sec+tv.tv_usec*1e-6;
}

inline double get_time()
{
	timeval tv;
	gettimeofday(&tv, 0);
	return tv.tv_sec+tv.tv_usec*1e-6 - pam_time;
}
//end of time

vector<string> board;
int W, H;
int num_devs;

struct Developer {
	int company;
	int bonus;
	int id;
	vector<int> skills;
	bool is_manager;
};

vector<Developer> devs;

int ScoreWith(int a, int b) {
	if (a == b) { return -1e9; }

	int ans = 0;
	if (devs[a].company == devs[b].company) {
		ans += devs[a].bonus * devs[b].bonus;
	}

	vi inter;
	set_intersection(ALL(devs[a].skills), ALL(devs[b].skills),
		back_inserter(inter));

	int intersz = SZ(inter);
	int unionsz = SZ(devs[a].skills) + SZ(devs[b].skills) - intersz;

	ans += intersz * (unionsz - intersz);

	return ans;
}

void Input() {
	cin >> W >> H;
	board.resize(H);
	for (auto &line : board) { cin >> line; }

	int D, M;
	cin >> D;

	map<string, int> known_comp;
	map<string, int> known_skill;

	int cur_id = 0;

	auto Read = [&](bool is_mgr) -> Developer {
		string company;
		cin >> company;

		Developer dev;
		dev.is_manager = is_mgr;
		dev.id = cur_id++;
		
		if (known_comp.count(company)) {
			dev.company = known_comp[company];
		} else {
			dev.company = SZ(known_comp);
			known_comp[company] = dev.company;
		}

		cin >> dev.bonus;

		if (!is_mgr) {
			int skills;
			cin >> skills;
			for (int i = 0; i < skills; ++i) {
				string what;
				cin >> what;
				int idx;
				if (known_skill.count(what)) {
					idx = known_skill[what];
				} else {
					idx = SZ(known_skill);
					known_skill[what] = idx;
				}

				dev.skills.push_back(idx);
			}
			sort(ALL(dev.skills));
		}
		return dev;
	};

	for (int i = 0; i < D; ++i) {
		devs.push_back(Read(false));
	}
	
	cin >> M;
	num_devs = D + M;

	for (int i = 0; i < M; ++i) {
		devs.push_back(Read(true));
	}
}

void Output(const vector<vector<int>> &answer) {
	vector<pii> out(num_devs, pii(-1, -1));

	for (int i = 0; i < H; ++i) {
		for (int j = 0; j < W; ++j) {
			if (answer[i][j] >= 0) {
				out[answer[i][j]] = pii(j, i);
			}
		}
	}

	for (int i = 0; i < num_devs; ++i) {
		if (out[i].st == -1) {
			cout << "X\n";
		} else {
			cout << out[i].st << " " << out[i].nd << "\n";
		}
	}
}

int ScoreAnswer(const vector<vector<int>> &answer) {
	int score = 0;
	for (int i = 0; i < H; ++i) {
		for (int j = 0; j < W; ++j) {
			if (i && answer[i-1][j] != -1 && answer[i][j] != -1) {
				score += ScoreWith(answer[i-1][j], answer[i][j]);
			}
			if (j && answer[i][j-1] != -1 && answer[i][j] != -1) {
				score += ScoreWith(answer[i][j-1], answer[i][j]);
			}
		}
	}
	return score;
}

vector<vector<int>> Kamil() {
	vector<vector<int>> answer(H, vector<int>(W, -1));
	vector<bool> taken(num_devs);
	int total_answer = 0;
	int used = 0;
	for(int row = 0; row < H; ++row) {
		debug() << "kamil " << row << " na " << H;
		for(int col = 0; col < W; ++col) {
			if(board[row][col] != '#') {
				pair<int,int> best{-1, -1};
				for(int who = 0; who < num_devs; ++who) {
					if(!taken[who] && (devs[who].is_manager == (board[row][col] == 'M'))) {
						int score = 0;
						if(row && answer[row-1][col] != -1) {
							score += ScoreWith(who, answer[row-1][col]);
						}
						if(col && answer[row][col-1] != -1) {
							score += ScoreWith(who, answer[row][col-1]);
						}
						best = max(best, {score, who});
					}
				}
				if(best.first != -1) {
					int who = best.second;
					total_answer += best.first;
					taken[who] = true;
					answer[row][col] = who;
					used++;
				}	
			}
		}
	}
	cerr << "kamil ma wynik " << total_answer << endl;
	cerr << "used : " << used << " / " << num_devs << endl;
	// debug() << imie(answr)
	return answer;
}

namespace radek
{

vector <pii> poz[2];

vector <pii> gdz;
vector <vi> ktotu;
pii nope{-1, -1};

int rx[]={-1, 1, 0, 0};
int ry[]={0, 0, -1, 1};

int ist(pii v)
{
	return (v.first>=0 && v.second>=0 && v.first<H && v.second<W);
}

pii rusz(pii v, int kt)
{
	return {v.first+rx[kt], v.second+ry[kt]};
}

int zamien(int a, int b)
{
	//~ debug() << a << " " << b << "  " << gdz[a] << " " << gdz[b];
	if (gdz[a]==nope && gdz[b]==nope)
		return 0;
	if (gdz[b]==nope)
		swap(a, b);
	int ret=0;
	if (gdz[a]==nope)
	{
		for (int i=0; i<4; i++)
		{
			pii w=rusz(gdz[b], i);
			if (ist(w))
			{
				int x=ktotu[w.first][w.second];
				if (x>=0)
					ret+=ScoreWith(x, a)-ScoreWith(x, b);
			}
		}
		ktotu[gdz[b].first][gdz[b].second]=a;
		gdz[a]=gdz[b];
		gdz[b]=nope;
		return ret;
	}
	pii ga=gdz[a];
	pii gb=gdz[b];
	for (int i=0; i<4; i++)
	{
		pii w=rusz(ga, i);
		if (ist(w))
		{
			int x=ktotu[w.first][w.second];
			if (x>=0)
				ret-=ScoreWith(x, a);
		}
	}
	for (int i=0; i<4; i++)
	{
		pii w=rusz(gb, i);
		if (ist(w))
		{
			int x=ktotu[w.first][w.second];
			if (x>=0)
				ret-=ScoreWith(x, b);
		}
	}
	//~ debug() << imie(ret) << imie(a) << imie(gdz[a]) << imie(ga) << imie(ktotu[ga.first][ga.second]);
	//~ debug() << imie(ret) << imie(b) << imie(gdz[b]) << imie(gb) << imie(ktotu[gb.first][gb.second]);
	swap(ktotu[ga.first][ga.second], ktotu[gb.first][gb.second]);
	swap(gdz[a], gdz[b]);
	swap(ga, gb);
	//~ debug() << imie(ret) << imie(a) << imie(gdz[a]) << imie(ga) << imie(ktotu[ga.first][ga.second]);
	//~ debug() << imie(ret) << imie(b) << imie(gdz[b]) << imie(gb) << imie(ktotu[gb.first][gb.second]);
	for (int i=0; i<4; i++)
	{
		pii w=rusz(ga, i);
		if (ist(w))
		{
			int x=ktotu[w.first][w.second];
			if (x>=0)
				ret+=ScoreWith(x, a);
		}
	}
	for (int i=0; i<4; i++)
	{
		pii w=rusz(gb, i);
		if (ist(w))
		{
			int x=ktotu[w.first][w.second];
			if (x>=0)
				ret+=ScoreWith(x, b);
		}
	}
	//~ debug() << imie(ret);
	return ret;
}

void zarzaj()
{
	for (int i=0; i<H; i++)
	{
		for (int j=0; j<W; j++)
		{
			if (board[i][j]=='_')
				poz[0].push_back({i, j});
			if (board[i][j]=='M')
				poz[1].push_back({i, j});
		}
	}
	init_time();
	gdz.resize(num_devs, nope);
	ktotu=Kamil();
	int sta=ScoreAnswer(ktotu);
	for (int i=0; i<H; i++)
		for (int j=0; j<W; j++)
			if (ktotu[i][j]>=0)
				gdz[ktotu[i][j]]={i, j};
	while(get_time()<10)
	{
		int x=rng()%num_devs;
		int y=rng()%num_devs;
		if (x==y || devs[x].is_manager!=devs[y].is_manager)
			continue;
		int w=zamien(x, y);
		//~ debug() << imie(w);
		if (w>=0)
		{
			sta+=w;
			if (w)
				debug() << "z " << sta-w << " na " << sta;
		}
		else
		{
			zamien(x, y);
		}
	}
	debug() << imie(sta);
	debug() << imie(ScoreAnswer(ktotu));
	Output(ktotu);
}

}//end of radek

int main()
{
	ios_base::sync_with_stdio(0);
	cin.tie(0);
	cout << fixed << setprecision(11);
	cerr << fixed << setprecision(6);
	Input();
	
	radek::zarzaj();
	return 0;
}
