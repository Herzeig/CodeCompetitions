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
//~ #include <SFML/Graphics.hpp>
//~ #include <SFML/Audio.hpp>
//~ #include <SFML/Window.hpp>
//~ #include <SFML/System.hpp>

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

//~ Event event;
//~ int koniec;

//~ RenderWindow window(VideoMode(100, 100), "Zarzanie");

//~ void ogarnijeventy()
//~ {
	//~ while (window.pollEvent(event))
	//~ {
		//~ if (event.type == Event::Closed)
		//~ {
			//~ koniec=1;
		//~ }
		//~ if (event.type == Event::KeyPressed)
		//~ {
			//~ if (event.key.code == Keyboard::Escape)
			//~ {
				//~ koniec=1;
			//~ }
			//~ if (event.key.code == Keyboard::X)
			//~ {
				//~ koniec=1;
			//~ }
		//~ }
	//~ }
//~ }

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

ll ost[1007];
ll czas;

const int radek_stala=1e4;
int prepro[radek_stala][radek_stala];

int ScoreWith(int a, int b) {
	if (a == b) { return -1e9; }
	
	if (a>b)
		swap(a, b);
	
	if (num_devs>radek_stala)
	{
		int ans = 0;
		if (devs[a].company == devs[b].company) {
			ans += devs[a].bonus * devs[b].bonus;
		}
		
		czas++;
		int prz = 0;
		for (const int i : devs[a].skills)
			ost[i] = czas;
		for (const int i : devs[b].skills)
			prz += (ost[i] == czas);
		
		ans += prz * (SZ(devs[a].skills) + SZ(devs[b].skills) - (prz<<1));

		return ans;
	}
	if (prepro[a][b]==-1)
	{
		int ans = 0;
		if (devs[a].company == devs[b].company) {
			ans += devs[a].bonus * devs[b].bonus;
		}
		
		czas++;
		int prz = 0;
		for (const int i : devs[a].skills)
			ost[i] = czas;
		for (const int i : devs[b].skills)
			prz += (ost[i] == czas);
		
		ans += prz * (SZ(devs[a].skills) + SZ(devs[b].skills) - (prz<<1));
		
		prepro[a][b]=ans;
	}
	return prepro[a][b];
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

vector<vector<int>> ReadOutput() {
	vector<vector<int>> answer(H, vector<int>(W, -1));

	for (int i = 0; i < num_devs; ++i) {
		string what;
		cin >> what;

		if (what == "X") { continue; }
		int c = stoi(what);
		int r;
		cin >> r;

		answer[r][c] = i;
	}

	return answer;
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

vector<vector<int>> Kamil2() {
	vector<vector<int>> answer(H, vector<int>(W, -1));
	vector<bool> taken(num_devs);
	int total_answer = 0;
	int used = 0;
	map<int, vector<int>> working_there;
	for(int i = 0; i < (int) devs.size(); ++i) {
		working_there[devs[i].company].push_back(i);
	}
	for(int row = 0; row < H; ++row) {
		for(int col = 0; col < W; ++col) {
			if(board[row][col] != '#') {
				pair<int,int> best{-1, -1};
				
				auto consider = [&](int who) {
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
				};
				
				set<int> companies;
				if(col && answer[row][col-1] != -1) {
					companies.insert(devs[answer[row][col-1]].company);
				}
				if(row && answer[row-1][col] != -1) {
					companies.insert(devs[answer[row-1][col]].company);
				}
				for(int c : companies) {
					for(int who : working_there[c]) {
						consider(who);
					}
				}
				if(best.first == -1) {
					for(int who = 0; who < num_devs; ++who) {
						consider(who);
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

vector<vector<int>> Kamil3() {
	const int dx[4] = {-1, 1, 0, 0};
	const int dy[4] = {0, 0, -1, 1};
	auto inside = [&](int row, int col) {
		return 0 <= row && row < H && 0 <= col && col < W;
	};
	vector<vector<int>> answer(H, vector<int>(W, -1));
	vector<bool> taken(num_devs);
	int total_answer = 0;
	int used = 0;
	
	vector<pair<int,int>> temps;
	auto mark = [&](int row, int col, int id, bool real) {
		answer[row][col] = id;
		taken[id] = true;
		if(!real) {
			temps.emplace_back(row, col);
		}
		else {
			used++;
		}
	};
	auto rollback = [&]() {
		for(pair<int,int> p : temps) {
			taken[answer[p.first][p.second]] = false;
			answer[p.first][p.second] = -1;
		}
		temps.clear();
	};
	
	map<int, vector<int>> working_there;
	for(int i = 0; i < (int) devs.size(); ++i) {
		working_there[devs[i].company].push_back(i);
	}
	vector<pair<int,int>> order;
	
	for(int sr = 0; sr < H; ++sr) {
		for(int sc = 0; sc < W; ++sc) {
			if(board[sr][sc] != '#') {
				order.emplace_back(sr, sc);
			}
		}
	}
	// random_shuffle(order.begin(), order.end());
	
	vector<int> everybody;
	for(int i = 0; i < num_devs; ++i) {
		everybody.push_back(i);
	}
	
	vector<pair<int,int>> remaining = order;
	int BAD = 0 ;
	while(!remaining.empty()) {
		bool anything = false;
	// for(pair<int,int> start : order) {
		// int sr = start.first;
		// int sc = start.second;
	// for(int sr = 0; sr < H; ++sr) {
		// for(int sc = 0; sc < W; ++sc) {
			// if(answer[sr][sc] == -1 && board[sr][sc] != '#') {
				pair<int,pair<int,int>> BEST{-1, {-1, -1}};
				const int TRIES = 100;
				// cerr << remaining.size() << " ";
				for(int rep = 0; rep <= TRIES && !remaining.empty(); ++rep) {
							int score_here = 0;
							int starting_who = -1;
							int id_where = rand() % remaining.size();
							if(rep == TRIES) {
								id_where = BEST.second.second;
								if(id_where == -1) {
									break;
								}
							}
							int sr = remaining[id_where].first;
							int sc = remaining[id_where].second;
							if(answer[sr][sc] != -1) {
								swap(remaining[id_where], remaining.back());
								remaining.pop_back();
								--rep;
								continue;
							}
							vector<pair<int,int>> kol;
							kol.emplace_back(sr, sc);
							for(int i = 0; i < min(900, (int) kol.size()); ++i) {
								int row = kol[i].first;
								int col = kol[i].second;
								if(answer[row][col] != -1) {
									continue;
								}
								
								
								pair<int,int> best{-1, -1};
								
								auto consider = [&](int who) {
									if(!taken[who] && (devs[who].is_manager == (board[row][col] == 'M'))) {
										int score = 0;
										for(int dir = 0; dir < 4; ++dir) {
											int r2 = row + dx[dir];
											int c2 = col + dy[dir];
											if(inside(r2, c2) && answer[r2][c2] != -1) {
												score += ScoreWith(who, answer[r2][c2]);
											}
										}
										// if(row && answer[row-1][col] != -1) {
											// score += ScoreWith(who, answer[row-1][col]);
										// }
										// if(col && answer[row][col-1] != -1) {
											// score += ScoreWith(who, answer[row][col-1]);
										// }
										if(score > best.first) {
											best = {score, who};
										}
										// best = max(best, {score, who});
									}
								};
								
								set<int> companies;
								for(int dir = 0; dir < 4; ++dir) {
									int r2 = row + dx[dir];
									int c2 = col + dy[dir];
									if(inside(r2, c2) && answer[r2][c2] != -1) {
										companies.insert(devs[answer[r2][c2]].company);
									}
								}
								if(rep == TRIES && i == 0) {
									companies.clear(); // ?
								}
								for(int c : companies) {
									for(int who : working_there[c]) {
										consider(who);
									}
								}
								if(best.first == -1) {
									if(rep == TRIES && i == 0) {
										if(BEST.second.first != -1) {
											consider(BEST.second.first);
										}
									}
									else {
										random_shuffle(everybody.begin(), everybody.end());
										for(int who : everybody) {
										// for(int who = 0; who < num_devs; ++who) {
											consider(who);
										}
									}
								}
								
								if(best.first != -1) {
									int who = best.second;
									if(rep == TRIES) {
										total_answer += best.first;
									}
									score_here += best.first;
									mark(row, col, who, rep == TRIES);
									if(rep == TRIES) {
										anything = true;
									}
									if(i == 0) {
										starting_who = who;
									}
									// taken[who] = true;
									// answer[row][col] = who;
									// used++;
									for(int dir = 0; dir < 4; ++dir) {
										int r2 = row + dx[dir];
										int c2 = col + dy[dir];
										if(inside(r2, c2) && answer[r2][c2] == -1 && board[r2][c2] != '#') {
											kol.emplace_back(r2, c2);
										}
									}
								}
									
								
								
								
							
								
							}
					// cerr << starting_who << " " << BAD << endl;
					if(starting_who != -1) {
						BEST = max(BEST, make_pair(score_here, make_pair(starting_who, id_where)));
						if(rep != TRIES) {
							rollback();
						}
					}
					else {
						// if(++BAD == 1000) {
							// break;
						// }
					}
				}
				if(!anything) {
					if(++BAD == 100) {
						break;
					}
				}
			}
		// }
	// }
	
	cerr << "kamil ma wynik " << total_answer << endl;
	cerr << "used : " << used << " / " << num_devs << endl;
	// debug() << imie(answr)
	return answer;
}

void OutputStream(const vector<vector<int>> &answer, ostream &os) {
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
			os << "X\n";
		} else {
			os << out[i].st << " " << out[i].nd << "\n";
		}
	}
}

void Output(const vector<vector<int>> &answer) {
	OutputStream(answer, cout);
}

void OutputNext(const vector<vector<int>> &answer) {
	static int idx = 0;
	++idx;
	ofstream fout("out-" + to_string(idx));
	OutputStream(answer, fout);
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
	swap(ktotu[ga.first][ga.second], ktotu[gb.first][gb.second]);
	swap(gdz[a], gdz[b]);
	swap(ga, gb);
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
	return ret;
}

int przesun(int v, pii g)
{
	int ret=0;
	for (int i=0; i<4; i++)
	{
		pii w=rusz(gdz[v], i);
		if (ist(w))
		{
			int x=ktotu[w.first][w.second];
			if (x>=0)
				ret-=ScoreWith(x, v);
		}
	}
	ktotu[gdz[v].first][gdz[v].second]=-1;
	gdz[v]=g;
	ktotu[gdz[v].first][gdz[v].second]=v;
	for (int i=0; i<4; i++)
	{
		pii w=rusz(gdz[v], i);
		if (ist(w))
		{
			int x=ktotu[w.first][w.second];
			if (x>=0)
				ret+=ScoreWith(x, v);
		}
	}
	return ret;
}

ll los(ll a, ll b)
{
	return a+(rng()%(b-a+1));
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
	gdz.resize(num_devs, nope);
	ktotu=ReadOutput();
	int sta=ScoreAnswer(ktotu);
	for (int i=0; i<H; i++)
		for (int j=0; j<W; j++)
			if (ktotu[i][j]>=0)
				gdz[ktotu[i][j]]={i, j};
	for (int iter=0; 1; iter++)
	{
		cerr << "iteracja " << iter << endl;
		init_time();
		while(get_time()<10)
		{
			int typ=rng()%3;
			if (typ==0)
			{
				int x=rng()%num_devs;
				int y=rng()%num_devs;
				if (x==y || devs[x].is_manager!=devs[y].is_manager)
					continue;
				int w=zamien(x, y);
				if (w>=0)
				{
					sta+=w;
					if (w)
						debug() << "1 z " << sta-w << " na " << sta;
				}
				else
				{
					zamien(x, y);
				}
			}
			if (typ==1)
			{
				int kt=rng()&1;
				int x=rng()%poz[kt].size();
				int y=rng()%poz[kt].size();
				pii ga=poz[kt][x];
				pii gb=poz[kt][y];
				int ktoa=ktotu[ga.first][ga.second];
				int ktob=ktotu[gb.first][gb.second];
				if (ktoa==-1 && ktob==-1)
					continue;
				if (ktoa>=0 && ktob>=0)
				{
					int w=zamien(ktoa, ktob);
					if (w>=0)
					{
						sta+=w;
						if (w)
							debug() << "2 z " << sta-w << " na " << sta;
					}
					else
					{
						zamien(ktoa, ktob);
					}
				}
				else
				{
					if (ktoa==-1)
					{
						swap(ga, gb);
						swap(ktoa, ktob);
					}
					int w=przesun(ktoa, gb);
					if (w>=0)
					{
						sta+=w;
						if (w)
							debug() << "3 z " << sta-w << " na " << sta;
					}
					else
					{
						przesun(ktoa, ga);
					}
				}
			}
			if (typ==2)
			{
				int kt=rng()&1;
				int x=rng()%poz[kt].size();
				pii ga=poz[kt][x];
				pii gb={-1, -1};
				const int odch=10;
				for (int pro=0; pro<100 && gb.first==-1; pro++)
				{
					ll x=los(max(ga.first-odch, 0), min(ga.first+odch, H-1));
					ll y=los(max(ga.second-odch, 0), min(ga.second+odch, W-1));
					if (board[x][y]!=board[ga.first][ga.second])
						continue;
					gb={x, y};
				}
				if (gb.first==-1)
					continue;
				
				int ktoa=ktotu[ga.first][ga.second];
				int ktob=ktotu[gb.first][gb.second];
				if (ktoa==-1 && ktob==-1)
					continue;
				if (ktoa>=0 && ktob>=0)
				{
					int w=zamien(ktob, ktoa);
					if (w>=0)
					{
						sta+=w;
						if (w)
							debug() << "4 z " << sta-w << " na " << sta;
					}
					else
					{
						zamien(ktoa, ktob);
					}
				}
				else
				{
					if (ktoa==-1)
					{
						swap(ga, gb);
						swap(ktoa, ktob);
					}
					int w=przesun(ktoa, gb);
					if (w>=0)
					{
						sta+=w;
						if (w)
							debug() << "5 z " << sta-w << " na " << sta;
					}
					else
					{
						przesun(ktoa, ga);
					}
				}
			}
		}
		cerr << "zapisuje " << endl;
		OutputNext(ktotu);
	}
	//~ debug() << imie(sta);
	//~ debug() << imie(ScoreAnswer(ktotu));
	//~ Output(ktotu);
}

}//end of radek

int main()
{
	memset(prepro, -1, sizeof(prepro));
	ios_base::sync_with_stdio(0);
	cin.tie(0);
	cout << fixed << setprecision(11);
	cerr << fixed << setprecision(6);
	Input();
	
	radek::zarzaj();
	return 0;
}
