//#pragma GCC optimize("Ofast")
//#pragma GCC target("avx")
//#undef LOCAL

#include "base.h"
#include "util/fast_io.h"
#include "util/random.h"
#include "util/stopwatch.h"
#include "bitop.h"
#include "datastructure/radixheap.h"
#define priority_queue RadixHeap

#include "graph/mincostflow.h"

Scanner sc = Scanner(stdin);
Printer pr = Printer(stdout);

int H, W;

struct P {
    int r, c;
    bool inside() const {
        return 0 <= r && r < H && 0 <= c && c < W;
    }
    P operator+(const P& rhs) const {
        return {r + rhs.r, c + rhs.c};
    }
    P operator-(const P& rhs) const {
        return {r - rhs.r, c - rhs.c};
    }
    int abs() const {
        return ::abs(r) + ::abs(c);
    }
};
ostream& operator<<(ostream& os, const P& p) {
    return os << "P(" << p.r << ", " << p.c << ")";
}

// dir: DRUL
const P d4[4] = {
    {1, 0},
    {0, 1},
    {-1, 0},
    {0, -1},
};

using B = bitset<400>;

int main(int argc, char** argv) {
    // ./a.out sol < input
    assert(argc == 2);
    StopWatch sw;
    sw.start();
    auto gen = Random();

    sc.read(W, H);
    VV<bool> mp_dm(H + 2, V<bool>(W + 2));
    VV<bool> mp_d(H + 2, V<bool>(W + 2));
    VV<bool> mp_m(H + 2, V<bool>(W + 2));
    for (int i = 0; i < H; i++) {
        string s;
        sc.read(s);
        for (int j = 0; j < W; j++) {
            mp_d[i + 1][j + 1] = (s[j] == '_');
            mp_m[i + 1][j + 1] = (s[j] == 'M');
            mp_dm[i + 1][j + 1] = mp_d[i + 1][j + 1] || mp_m[i + 1][j + 1];
        }
    }
    H += 2;
    W += 2;
    int dn;
    sc.read(dn);
    V<int> comp(dn);
    V<int> bonus(dn);
    V<B> skill(dn, B());
    
    map<string, int> comp_mp;
    auto get_comp = [&](string s) {
        if (comp_mp.count(s)) return comp_mp[s];
        int id = int(comp_mp.size());
        comp_mp[s] = id;
        return id;
    };
    map<string, int> skill_mp;
    auto get_skill = [&](string s) {
        if (skill_mp.count(s)) return skill_mp[s];
        int id = int(skill_mp.size());
        skill_mp[s] = id;
        return id;
    };
    for (int i = 0; i < dn; i++) {
        string s;
        int k;
        sc.read(s, bonus[i], k);
        comp[i] = get_comp(s);
        for (int j = 0; j < k; j++) {
            string t;
            sc.read(t);
            skill[i].set(get_skill(t));
        }
    }
    int mn;
    sc.read(mn);

    int n = dn + mn;
    comp.resize(n);
    bonus.resize(n);

    for (int i = 0; i < mn; i++) {
        string s;
        sc.read(s, bonus[dn + i]);
        comp[dn + i] = get_comp(s);
    }

    // read ans
    V<P> ans(n);
    Scanner sc_ans(fopen(argv[1], "r"));
    for (int i = 0; i < n; i++) {
        string s;
        sc_ans.read(s);
        if (s == "X") ans[i] = P{-1, -1};
        else {
            string t;
            sc_ans.read(t);
            ans[i] = {stoi(t) + 1, stoi(s) + 1};
            assert(mp_dm[ans[i].r][ans[i].c]);
        }
    }

    V<P> list;
    {
        queue<P> que;
        VV<bool> vis(H, V<bool>(W));
        for (int r = 0; r < H; r++) {
            for (int c = 0; c < W; c++) {
                if (vis[r][c]) continue;
                if (!mp_dm[r][c]) continue;
                vis[r][c] = true;
                que.push({r, c});
                while (que.size()) {
                    P p = que.front(); que.pop();
                    list.push_back(p);
                    for (auto d: d4) {
                        P np = p + d;
                        if (vis[np.r][np.c]) continue;
                        if (!mp_dm[np.r][np.c]) continue;
                        vis[np.r][np.c] = true;
                        que.push(np);
                    }
                }
            }
        }
    }

    VV<int> ans_mp(H, V<int>(W, -1));
    V<bool> used(n);
    for (int i = 0; i < n; i++) {
        if (ans[i].r != -1) {
            ans_mp[ans[i].r][ans[i].c] = i;
            used[i] = true;
        }
    }
    auto cng_ans = [&](P p, int i) {
        int pre = ans_mp[p.r][p.c];
        if (pre != -1) {
            used[pre] = false;
            ans_mp[p.r][p.c] = -1;
            ans[pre] = P{-1, -1};
        }
        if (i != -1) {
            used[i] = true;
            ans_mp[p.r][p.c] = i;
            ans[i] = p;
        }
    };
    auto calc_score = [&](int i, int j) {
        assert(0 <= i && i < n);
        assert(0 <= j && j < n);
        int ans = 0;
        if (comp[i] == comp[j]) ans += bonus[i] * bonus[j];
        if (i < dn && j < dn) {
            int ins = int((skill[i] & skill[j]).count());
            int uni = int((skill[i] | skill[j]).count());
            ans += ins * (uni - ins);
        }
        return ans;
    };

    auto calc_answer = [&]() {
        int sum = 0;
        for (int r = 0; r < H - 1; r++) {
            for (int c = 0; c < W - 1; c++) {
                if (ans_mp[r][c] == -1) continue;
                if (ans_mp[r + 1][c] != -1) {
                    sum += calc_score(ans_mp[r][c], ans_mp[r + 1][c]);
                }
                if (ans_mp[r][c + 1] != -1) {
                    sum += calc_score(ans_mp[r][c], ans_mp[r][c + 1]);
                }
            }
        }
        return sum;
    };

    auto opt = [&](V<P> preds) {        
        V<int> lh;
        for (int i = 0; i < n; i++) {
            if (!used[i]) lh.push_back(i);
        }
        for (auto p: preds) {
            if (ans_mp[p.r][p.c] != -1) lh.push_back(ans_mp[p.r][p.c]);
        }
        sort(lh.begin(), lh.end());
        lh.erase(unique(lh.begin(), lh.end()), lh.end());
        int ln = int(lh.size()), rn = int(preds.size());
        struct E {
            int to, rev, cap, dist;
        };
        dbg(ln, rn);
        VV<E> g(ln + rn + 2);
        int sv = ln + rn, tv = sv + 1;
        auto add_edge = [&](int from, int to, int cap, int dist) {
            g[from].push_back(E{to, int(g[to].size()), cap, dist});
            g[to].push_back(E{from, int(g[from].size())-1, 0, -dist});
        };
        const int INF = TEN(5);
        for (int i = 0; i < ln; i++) {
            for (int j = 0; j < rn; j++) {
                P p = preds[j];
                if (lh[i] < dn) {
                    if (!mp_d[p.r][p.c]) continue;
                } else {
                    if (!mp_m[p.r][p.c]) continue;
                }
                int sum = 0;
                for (auto d: d4) {
                    P np = p + d;
                    if (ans_mp[np.r][np.c] != -1) sum += calc_score(ans_mp[np.r][np.c], lh[i]);
                }                
                add_edge(i, ln + j, 1, INF - sum);
            }
        }
        for (int i = 0; i < ln; i++) {
            add_edge(sv, i, 1, 0);
        }
        for (int i = 0; i < rn; i++) {
            add_edge(ln + i, tv, 1, 0);
        }
        auto res = get_mcf<int, int>(g, sv, tv, false);
        res.max_flow(TEN(9));
        for (int j = 0; j < rn; j++) {
            cng_ans(preds[j], -1);
        }
        for (int i = 0; i < ln; i++) {
            for (auto e: res.g[i]) {
                int j = e.to - ln;
                if (j < 0 || rn <= j) continue;
                if (e.cap) continue;
                cng_ans(preds[j], lh[i]);
            }
        }
    };

    int ln = int(list.size());
    VV<int> vis(H, V<int>(W));
    int id = 0;
    dbg(calc_answer());
    for (int ph = 0; ph < 30; ph++) {
        id++;
        V<P> preds;
        int st = (ph == 0 ? 0 : gen.uniform(0, ln - 1));
        for (int i = 0; i < ln; i++) {
            P p = list[(st + i) % ln];
            if (vis[p.r][p.c] == id) continue;
            preds.push_back(p);
            if (preds.size() >= 1000) break;
            for (auto d: d4) {
                P np = p + d;
                vis[np.r][np.c] = id;
            }
        }
        opt(preds);
        dbg(calc_answer());
    }
    // print ans
    for (auto p: ans) {
        if (p.r == -1) {
            pr.writeln("X");
        } else {
            pr.writeln(p.c - 1, p.r - 1);
        }
    }
    return 0;
}

