#include <iostream>
#include <vector>
#include <utility>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <bitset>
#include <random>
#include <algorithm>
#include <cassert>
#include <stack>
#include <map>
#include <fstream>

#define rep(i, s, t) for (auto i = s; i < t; ++i)
#define per(i, s, t) for (auto i = s; i >= t; --i)
#define all(x) (x).begin(), (x).end()
#define sz(x) ((int)x.size())

using namespace std;

using ll = long long;
using ull = unsigned long long;
using ld = double;

constexpr ll INF = (ll) (1e18 + 1e9 + 123);
constexpr ld EPS = 1e-9;

template<typename A, typename B>
bool mini(A& x, const B& y) {
    if (y < x) {
        x = y;
        return true;
    }
    return false;
}

template<typename A, typename B>
bool maxi(A& x, const B& y) {
    if (y > x) {
        x = y;
        return true;
    }
    return false;
}

struct Person {
    bool is_dev;
    int c;
    int b;
    vector<int> s;

    Person(bool is_dev, int c, int b, vector<int> s) : is_dev(is_dev), c(c), b(b), s(std::move(s)) {}

    Person() = default;
};

char field[1000][1000];
int ans[1000][1000];
Person person[100'000];
vector<int> devs, managers;
int w, h;

pair<vector<Person>, vector<Person>> read_people(std::istream& fin) {
    vector<Person> devs, mans;
    map<string, int> skills;
    map<string, int> companies;

    int d;
    fin >> d;
    for (int i = 0; i < d; ++i) {
        string c;
        fin >> c;
        int ci;
        if (!companies.count(c)) {
            ci = companies.size();
            companies[c] = ci;
        } else {
            ci = companies[c];
        }

        int b;
        fin >> b;

        int sn;
        fin >> sn;
        vector<int> s(sn);
        for (int j = 0; j < sn; ++j) {
            string sk;
            fin >> sk;
            int si;
            if (!skills.count(sk)) {
                si = skills.size();
                skills[sk] = si;
            } else {
                si = skills[sk];
            }
            s[j] = si;
        }
        sort(s.begin(), s.end());

        devs.emplace_back(true, ci, b, s);
    }

    int m;
    fin >> m;
    for (int i = 0; i < m; ++i) {
        string c;
        fin >> c;
        int ci;
        if (!companies.count(c)) {
            ci = companies.size();
            companies[c] = ci;
        } else {
            ci = companies[c];
        }

        int b;
        fin >> b;

        mans.emplace_back(false, ci, b, vector<int>());
    }

    return {devs, mans};
}

int score(const Person& a, const Person& b) {
    int ans = 0;
    if (a.c == b.c) {
        ans += a.b * b.b;
    }

    if (a.is_dev && b.is_dev) {
        int inters = 0;
        int j = 0;
        for (int i = 0; i < a.s.size(); ++i) {
            while (j < b.s.size() && b.s[j] < a.s[i]) {
                ++j;
            }
            if (j < b.s.size() && b.s[j] == a.s[i]) {
                ++inters;
            }
        }
        ans += inters * (a.s.size() + b.s.size() - 2 * inters);
    }
    return ans;
}

/*
 * isaf27 code start
 */

using flow_t = ll;
using cost_t = ll;
struct Edge {
    cost_t c;
    flow_t f;
    int to, rev;
    Edge(int _to, cost_t _c, flow_t _f, int _rev):c(_c), f(_f), to(_to), rev(_rev){}
};
struct mcSFlow {
    static constexpr cost_t INFCOST = numeric_limits<cost_t>::max()/2;
    cost_t eps;
    int N, S, T;
    vector<vector<Edge> > G;
    vector<unsigned int> isq, cur;
    vector<flow_t> ex;
    vector<cost_t> h;
    mcSFlow(int _N, int _S, int _T):eps(0), N(_N), S(_S), T(_T), G(_N){}
    Edge add_edge(int a, int b, cost_t cost, flow_t cap){
        assert(cap>=0);
        assert(a>=0&&a<N&&b>=0&&b<N);
        assert(a!=b);
        cost*=N;
        eps = max(eps, abs(cost));
        G[a].emplace_back(b, cost, cap, G[b].size());
        Edge ret = G[a].back();
        G[b].emplace_back(a, -cost, 0, G[a].size()-1);
        return ret;
    }
    void add_flow(Edge& e, flow_t f) {
        Edge &back = G[e.to][e.rev];
        if (!ex[e.to] && f)
            hs[h[e.to]].push_back(e.to);
        e.f -= f; ex[e.to] += f;
        back.f += f; ex[back.to] -= f;
    }
    vector<vector<int> > hs;
    vector<int> co;
    // fast max flow, lowest label version
    flow_t max_flow() {
        ex.assign(N, 0);
        h.assign(N, 0); hs.resize(2*N);
        co.assign(2*N, 0); cur.assign(N, 0);
        h[S] = N;
        ex[T] = 1;
        co[0] = N-1;
        for(auto &e:G[S]) add_flow(e, e.f);
        if(hs[0].size())
            for (int hi = 0;hi>=0;) {
                int u = hs[hi].back();
                hs[hi].pop_back();
                while (ex[u] > 0) { // discharge u
                    if (cur[u] == G[u].size()) {
                        h[u] = 1e9;
                        for(unsigned int i=0;i<G[u].size();++i){
                            auto &e = G[u][i];
                            if (e.f && h[u] > h[e.to]+1){
                                h[u] = h[e.to]+1;
                                cur[u] = i;
                            }
                        }
                        if (++co[h[u]], !--co[hi] && hi < N)
                            for(int i=0;i<N;++i){
                                if (hi < h[i] && h[i] < N){
                                    --co[h[i]];
                                    h[i] = N + 1;
                                }
                            }
                        hi = h[u];
                    } else if (G[u][cur[u]].f && h[u] == h[G[u][cur[u]].to]+1){
                        add_flow(G[u][cur[u]], min(ex[u], G[u][cur[u]].f));
                    } else ++cur[u];
                }
                while (hi>=0 && hs[hi].empty()) --hi;
            }
        return -ex[S];
    }
    // begin min cost flow
    bool look_ahead(int u){
        if(ex[u]) return false;
        cost_t newHeight = h[u]-N*eps;
        for(auto const&e:G[u]){
            if(e.f == 0) continue;
            if(h[u] + e.c - h[e.to] < 0) return false; // outgoing admissible arc
            else newHeight = max(newHeight, h[e.to] - e.c); // try to make arc admissible
        }
        h[u] = newHeight - eps;
        return true;
    }
    void push(Edge &e, flow_t amt){
        if(e.f < amt) amt=e.f;
        e.f-=amt; ex[e.to]+=amt;
        G[e.to][e.rev].f+=amt; ex[G[e.to][e.rev].to]-=amt;
    }
    void relabel(int vertex){
        cost_t newHeight = -INFCOST;
        for(unsigned int i=0;i<G[vertex].size();++i){
            Edge const&e = G[vertex][i];
            if(e.f && newHeight < h[e.to]-e.c){
                newHeight = h[e.to] - e.c;
                cur[vertex] = i;
            }
        }
        h[vertex] = newHeight - eps;
    }
    static constexpr int scale=2;
    template<bool use_look_ahead = true>
    pair<flow_t, cost_t> minCostMaxFlow(){
        cost_t retCost = 0;
        for(int i=0;i<N;++i)
            for(Edge &e:G[i])
                retCost += e.c*(e.f);
        // remove this for circulation
        flow_t retFlow = max_flow();
        h.assign(N, 0); ex.assign(N, 0);
        isq.assign(N, 0); cur.assign(N,0);
        stack<int> q;
        for(;eps;eps>>=scale){
            fill(cur.begin(), cur.end(), 0);
            for(int i=0;i<N;++i)
                for(auto &e:G[i])
                    if(h[i] + e.c - h[e.to] < 0 && e.f)
                        push(e, e.f);
            for(int i=0;i<N;++i){
                if(ex[i]>0){
                    q.push(i);
                    isq[i]=1;
                }
            }
            while(!q.empty()){
                int u=q.top();q.pop();
                isq[u]=0;
                while(ex[u]>0){
                    if(cur[u] == G[u].size())
                        relabel(u);
                    for(unsigned int &i=cur[u], max_i = G[u].size();i<max_i;++i){
                        Edge &e=G[u][i];
                        if(e.f == 0) continue;
                        if(h[u] + e.c - h[e.to] < 0){
                            if(use_look_ahead && look_ahead(e.to)){
                                --i;
                                continue;
                            }
                            push(e, ex[u]);
                            if(isq[e.to]==0){
                                q.push(e.to);
                                isq[e.to]=1;
                            }
                            if(ex[u]==0) break;
                        }
                    }
                }
            }
            if(eps>1 && eps>>scale==0){
                eps = 1<<scale;
            }
        }
        for(int i=0;i<N;++i){
            for(Edge &e:G[i]){
                retCost -= e.c*(e.f);
            }
        }
        return make_pair(retFlow, retCost/2/N);
    }
    flow_t getFlow(Edge const &e){
        return G[e.to][e.rev].f;
    }
};

const int dx[4] = {0, 1, 0, -1};
const int dy[4] = {-1, 0, 1, 0};

int get_isaf_score(int id, int i, int j)
{
    int res = 0;
    for (int d = 0; d < 4; d++)
    {
        int nx = i + dx[d];
        int ny = j + dy[d];
        if (nx < 0 || nx >= h || ny < 0 || ny >= w)
            continue;
        if (ans[nx][ny] != -1)
            res += score(person[id], person[ans[nx][ny]]);
    }
    return res;
}

void remake(int c)
{
    int pl = (int)devs.size() + (int)managers.size();
    vector<bool> used(pl, false);
    for (int i = 0; i < h; i++)
        for (int j = 0; j < w; j++)
            if ((i + j) % 2 != c && ans[i][j] != -1)
                used[ans[i][j]] = true;
    vector<int> fnd(pl, -1);
    vector<int> want;
    for (int i = 0; i < pl; i++)
        if (!used[i])
        {
            fnd[i] = (int)want.size();
            want.push_back(i);
        }
    vector<vector<int>> gt(h);
    for (int i = 0; i < h; i++)
        gt[i].assign(w, -1);
    vector<pair<int, int>> pos;
    for (int i = 0; i < h; i++)
        for (int j = 0; j < w; j++)
            if ((field[i][j] == 'M' || field[i][j] == '_') && (i + j) % 2 == c)
            {
                gt[i][j] = (int)pos.size();
                pos.push_back(make_pair(i, j));
            }
    int S = (int)want.size() + (int)pos.size();
    int F = S + 1;
    mcSFlow graph = mcSFlow(F + 1, S, F);
    for (int i = 0; i < (int)want.size(); i++)
        graph.add_edge(S, i, 0, 1);
    for (int i = 0; i < (int)pos.size(); i++)
        graph.add_edge((int)want.size() + i, F, 0, 1);
    vector<vector<Edge>> to((int)want.size());
    for (int i = 0; i < h; i++)
        for (int j = 0; j < w; j++)
            if (ans[i][j] != -1 && (i + j) % 2 == c)
            {
                int id = fnd[ans[i][j]];
                to[id].push_back(graph.add_edge(id, gt[i][j] + (int)want.size(), -get_isaf_score(ans[i][j], i, j), 1));
            }
    for (int i = 0; i < h; i++)
        for (int j = 0; j < w; j++)
            if ((i + j) % 2 == c)
                ans[i][j] = -1;
    int CS = 10;
    vector<pair<int, int>> to_score((int)pos.size());
    for (int i = 0; i < (int)want.size(); i++)
    {
        int sz = 0;
        for (pair<int, int> t : pos)
        {
            int id = gt[t.first][t.second];
            if (field[t.first][t.second] == 'M' && person[want[i]].is_dev)
                continue;
            if (field[t.first][t.second] == '_' && !person[want[i]].is_dev)
                continue;
            to_score[sz++] = make_pair(-get_isaf_score(want[i], t.first, t.second), id);
        }
        nth_element(to_score.begin(), to_score.begin() + min(CS, sz), to_score.begin() + sz);
        for (int ii = 0; ii < min(CS, sz); ii++)
            to[i].push_back(graph.add_edge(i, (int)want.size() + to_score[ii].second, to_score[ii].first, 1));
    }
    cerr << "Graph built!" << endl;
    graph.minCostMaxFlow();
    for (int i = 0; i < (int)want.size(); i++)
        for (const Edge& e : to[i])
            if (graph.getFlow(e) == 1)
            {
                int id = e.to - (int)want.size();
                ans[pos[id].first][pos[id].second] = want[i];
            }
}

void flow_opt()
{
    remake(0);
}

/*
 * isaf27 code end
 */

int main(int argc, char *argv[])
{
    cin >> w >> h;
    rep(i, 0, h) {
        rep(j, 0, w) {
            cin >> field[i][j];
            ans[i][j] = -1;
        }
    }

    auto [de, ma] = read_people(cin);
    int ptr = 0;
    for (auto p : de) {
        devs.push_back(ptr);
        person[ptr++] = p;
    }
    for (auto p : ma) {
        managers.push_back(ptr);
        person[ptr++] = p;
    }

    int tot = sz(devs) + sz(managers);

    string aname(argv[1]);
    ifstream ain(aname);
    for (int id = 0; id < tot; id++)
    {
        string fs;
        ain >> fs;
        if (fs == "X")
            continue;
        int j = atoi(fs.c_str());
        int i;
        ain >> i;
        ans[i][j] = id;
    }

    flow_opt();

    vector<string> res(tot, "X");
    rep(i, 0, h) {
        rep(j, 0, w) {
            if (ans[i][j] != -1) {
                res[ans[i][j]] = to_string(j) + " " + to_string(i);
            }
        }
    }
    for (auto x : res) {
        cout << x << "\n";
    }

    return 0;
}
