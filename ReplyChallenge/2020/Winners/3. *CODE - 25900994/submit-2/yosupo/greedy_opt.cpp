//#pragma GCC optimize("Ofast")
//#pragma GCC target("avx")
//#undef LOCAL

#include "base.h"
#include "util/fast_io.h"
#include "util/random.h"
#include "util/stopwatch.h"

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

    int ln = int(list.size());
    static int gc = 0;
    while (sw.msecs() < 59000) {
        gc++;
        auto p = list[gen.uniform(0, ln - 1)];
        auto sc = [&](int i) {
            if (i == -1) return -1;
            int sum = 0;
            for (auto d: d4) {
                P np = p + d;
                if (ans_mp[np.r][np.c] != -1) sum += calc_score(ans_mp[np.r][np.c], i);
            }
            return sum;
        };
        int pre = ans_mp[p.r][p.c];
        int mi = pre, ma = sc(pre), old_ma = ma;
        for (int i = (mp_d[p.r][p.c] ? 0 : dn); i < (mp_m[p.r][p.c] ? n : dn); i++) {
            if (used[i]) continue;
            int nw = sc(i);
            if (ma <= nw) {
                ma = nw;
                mi = i;
            }
        }
        if (mi == pre) continue;
        cng_ans(p, mi);
        if(ma>old_ma)dbg(gc, ma - old_ma);
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

