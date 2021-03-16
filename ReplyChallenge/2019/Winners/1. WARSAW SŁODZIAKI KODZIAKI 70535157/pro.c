#include "bits/stdc++.h"
using namespace std;
#define PB push_back
#define MP make_pair
#define FOR(i,a,b) for(int i = (a); i <= (b); i++)
#define RE(i,n) FOR(i,1,n)
#define REP(i,n) FOR(i,0,(int)(n)-1)
#define R(i,n) REP(i,n)
#define VI vector<int>
#define PII pair<int,int>
#define LD long double
#define FI first
#define SE second
#define st FI
#define nd SE
#define ALL(x) (x).begin(), (x).end()
#define SZ(x) ((int)(x).size())

template<class C> void mini(C &a4, C b4) { a4 = min(a4, b4); }
template<class C> void maxi(C &a4, C b4) { a4 = max(a4, b4); }

using namespace std;
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

using ll = long long;

int H, W, count_clients, count_offices;
const int nax = 1005;
char grid[nax][nax];
int cost[nax][nax];
int id_client[nax][nax];
int odl[155][nax][nax];

const ll BIG_INF = 1e18L + 5;
const int INF = 1e9 + 5;
//~ void bfs(int row, int col,

struct Client {
  int row, col, reward;
  void read(int _id) {
    scanf("%d%d%d", &col, &row, &reward);
    id_client[row][col] = _id;
  }
};

struct Office {
  int row, col;
  
};





// Marek
constexpr int inf = INF;
int DajKoszt(char c) {
  switch (c) {
    case '#': return inf;
    case '~': return 800;
    case '*': return 200;
    case '+': return 150;
    case 'X': return 120;
    case '_': return 100;
    case 'H': return 70;
    case 'T': return 50;
    default:
      debug() << imie(c) imie(int(c));
      assert(false);
  }
}
const int dx[4] = {-1, 1, 0, 0};
const int dy[4] = {0, 0, -1, 1};
void LiczOdleglosci() {
  auto LiczOd = [&](int indeks, int r_, int c_) -> void {
    vector<vector<pair<int, int>>> dist(1000);
    auto Daj = [&](int x) -> vector<pair<int, int>>& {
      if ((int) dist.size() <= x) {
        //debug() << imie(x);
        dist.resize(x + 1);
      }
      return dist[x];
    };
    for (int r = 0; r < H; r++) {
      for (int c = 0; c < W; c++) {
        odl[indeks][r][c] = inf;
      }
    }
    vector<vector<bool>> juz(H, vector<bool>(W, false));
    Daj(0).emplace_back(r_, c_);
    for (int o = 0; o < (int) dist.size(); o++) {
      for (int i = 0; i < (int) dist[o].size(); i++) {
        const int r = dist[o][i].first;
        const int c = dist[o][i].second;
        const int noo = o + DajKoszt(grid[r][c]);
        if (juz[r][c]) continue;
        juz[r][c] = true;
        odl[indeks][r][c] = o;
        for (int j = 0; j < 4; j++) {
          const int rr = r + dx[j];
          const int cc = c + dy[j];
          if (0 <= rr and rr < H and 0 <= cc and cc < W) {
            if (!juz[rr][cc] and grid[rr][cc] != '#') {
              Daj(noo).emplace_back(rr, cc);
            }
          }
        }
      }
    }
  };
  for (int r = 0; r < H; r++) {
    for (int c = 0; c < W; c++) {
      if (id_client[r][c] >= 0) {
        debug() << imie(r) imie(c) imie(id_client[r][c]);
        LiczOd(id_client[r][c], r, c);
      }
    }
  }
}
string Odzyskaj(int r, int c, int r2, int c2) { // office, client
  static constexpr char kier[4] = {'U', 'D', 'L', 'R'};
  const int id = id_client[r2][c2];
  //debug() << imie(r) imie(c) imie(r2) imie(c2) imie(id);
  assert(id >= 0);
  assert(odl[id][r][c] < inf);
  string res;
  int koszt = 0;
  while (r != r2 or c != c2) {
    int rrr = -1;
    int ccc = -1;
    int ooo = inf;
    char znak = '\0';
    for (int i = 0; i < 4; i++) {
      const int rr = r + dx[i];
      const int cc = c + dy[i];
      if (0 <= rr and rr < H and 0 <= cc and cc < W) {
        //debug() << imie(rr) imie(cc) imie(odl[id][rr][cc]);
        const int oo = odl[id][rr][cc] + DajKoszt(grid[rr][cc]);
        if (ooo > oo) {
          ooo = oo;
          rrr = rr;
          ccc = cc;
          znak = kier[i];
        }
      }
    }
    //debug() << imie(rrr) imie(ccc) imie(ooo) imie(znak);
    assert(rrr != -1);
    r = rrr;
    c = ccc;
    koszt += DajKoszt(grid[r][c]);
    res.push_back(znak);
  }
  //debug() << imie(koszt);
  return res;
}





vector<pair<PII,PII>> res;
bool podlaczony[nax];


int SUM = 0;

int main() {
  scanf("%d%d%d%d", &W, &H, &count_clients, &count_offices);
  vector<Client> clients(count_clients);
  for(int row = 0; row < H; ++row) {
    for(int col = 0; col < W; ++col) {
      id_client[row][col] = -1;
    }
  }
  for(int i = 0; i < count_clients; ++i) {
    clients[i].read(i);
      SUM += clients[i].reward;
  
  }
  for(int row = 0; row < H; ++row) {
    scanf("%s", grid[row]);
  }
  LiczOdleglosci();
  //~ vector<Office> offices;
  //~ for(int row = 0; row < H; ++row) {
    //~ for(int col = 0; col < W; ++col) {
      //~ if(grid[row][col] != '#' && id_client[row][col] != -1 && (int) offices.size() < count_offices) {
        //~ offices.push_back(Office{row, col});
      //~ }
    //~ }
  //~ }
   vector<vector<int>> his_score(H, vector<int>(W, 0));
  vector<pair<int,  pair<int,int> >> order;
  debug() << imie(H) imie(W);
  for(int row = 0; row < H; ++row) {
    for(int col = 0; col < W; ++col) {
      if(id_client[row][col] < 0 && grid[row][col] != '#') {
        his_score[row][col] = 0;
        for(int c = 0; c < count_clients; ++c) {
          if(odl[c][row][col] != 0 && odl[c][row][col] <= clients[c].reward) {
            his_score[row][col] += clients[c].reward - odl[c][row][col];
          }
        }
        order.push_back(make_pair(his_score[row][col], make_pair(row, col)));
      }
    }
  }
  // from biggest
  sort(order.begin(), order.end());
  reverse(ALL(order));
  
  int WYN = 0;
  debug() << imie(SZ(order));
  assert((int) order.size() >= count_offices);
  for(int who = 0; who < count_offices; ++who) {
    int row = order[who].second.first;
    int col = order[who].second.second;
    
    for(int i = 0; i < count_clients; i++){
      if(odl[i][row][col] != 0 && clients[i].reward >= odl[i][row][col]){
        WYN += clients[i].reward - odl[i][row][col];
        res.PB(MP(MP(row,col), MP(clients[i].row, clients[i].col)));
        
        debug() << imie(row) imie(col) imie(clients[i].row) imie(clients[i].col);
        Odzyskaj(row, col, clients[i].row, clients[i].col);
        podlaczony[i] = 1;
      }
    }
    //~ printf("%d %d\n", col, row); // odwrotny zapis
    //~ for(int c = 1; c <= count_clients; ++c) {
      //~ if(odl[c][row][col] > 0) {
        //~ printf("%5d %5d   %d\n", odl[c][row][col], clients[c-1].reward, clients[c-1].reward - odl[c][row][col]);
      //~ }
      //~ else puts("-");
    //~ }
    //~ puts("");
    //~ puts("");
    assert(id_client[row][col] == -1);
    id_client[row][col] = -2;
  }
  
  
  for(auto el:res){
    cout << el.FI.SE << " " << el.FI.FI << " " << Odzyskaj(el.FI.FI, el.FI.SE, el.SE.FI, el.SE.SE) << "\n";
  }
  //~ for(int row = 0; row < H; ++row) {
    //~ printf("%s\n", grid[row]);
  //~ }
}
