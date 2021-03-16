//#pragma GCC optimize("Ofast")
//#pragma GCC target("avx")
//#undef LOCAL

#include <omp.h>
#include "../../yosupo/base.h"
#include "../../yosupo/util/fast_io.h"
#include "../../yosupo/util/random.h"

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

template<class t>
void chmax(t&a,const t&b){
	if(a<b)a=b;
}

int main() {
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

    dbg(comp_mp.size(), skill_mp.size());

    //V<P> list;
    VV<P> waf;
    queue<P> que;
    VV<bool> vis(H, V<bool>(W));
    for (int r = 0; r < H; r++) {
        for (int c = 0; c < W; c++) {
            if (vis[r][c]) continue;
            if (!mp_dm[r][c]) continue;
            V<P> list;
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
            waf.push_back(list);
        }
    }

	sort(waf.begin(),waf.end(),[&](const V<P>&a,const V<P>&b){return a.size()>b.size();});

    V<P> ans(n, P{-1, -1});
    VV<int> ans_mp(H, V<int>(W, -1));

    auto calc_score = [&](int i, int j) {
        int ans = 0;
        if (comp[i] == comp[j]) ans += bonus[i] * bonus[j];
        if (i < dn && j < dn) {
            int ins = int((skill[i] & skill[j]).count());
            int uni = int((skill[i] | skill[j]).count());
            ans += ins * (uni - ins);
        }
        return ans;
    };
    V<bool> used(n);

	const int T=20;
	V<Random> rnds(T);
	for(int i=0;i<T;i++)
		rnds[i]=Random(i);

    int score = 0;
    for(auto list:waf){
		static int cnt=0;
		cerr<<cnt++<<"/"<<waf.size()<<endl;
		//pair<int,V<pair<int,P>>> add(0,{});
		V<pair<int,V<pair<int,P>>>> buf(T);
		#pragma omp parallel for
		for(int _=0;_<T;_++){
			V<bool> used_buf=used;
			VV<int> ans_mp_buf=ans_mp;
			
			V<pair<int,P>> z;
			int dif=0;
			bool first=true;
			for (auto p: list) {
				int ma = -1, mi = -1;
				for (int i = (mp_d[p.r][p.c] ? 0 : dn); i < (mp_m[p.r][p.c] ? n : dn); i++) {
					if (used_buf[i]) continue;
					int sum = 0;
					if(first){
						sum=global_gen.uniform(1,2000000000);
					}else{
						for (auto d: d4) {
							P np = p + d;
							if (ans_mp_buf[np.r][np.c] != -1) sum += calc_score(ans_mp_buf[np.r][np.c], i);
						}
					}
					if (ma < sum) {
						ma = sum;
						mi = i;
					}
				}
				if(first)ma=0;
				first=false;
				if (mi == -1) break;
				//score += ma;
				dif+=ma;
				/*static int sc = 0;
				sc++;
				if (sc == 1000) {
					dbg(score, mi);
					sc = 0;
				}*/
				used_buf[mi] = true;
				//ans[mi] = p;
				ans_mp_buf[p.r][p.c] = mi;
				z.emplace_back(mi,p);
			}
			buf[_]=make_pair(dif,z);
		}
		int mx=0;
		for(int i=0;i<T;i++)if(buf[mx].first<buf[i].first)mx=i;
		score+=buf[mx].first;
		for(auto kv:buf[mx].second){
			used[kv.first]=true;
			ans[kv.first]=kv.second;
			ans_mp[kv.second.r][kv.second.c]=kv.first;
		}
	}
    dbg(score);
	
	cerr<<score<<endl;
	cerr<<waf.size()<<endl;
	
    for (auto p: ans) {
        if (p.r == -1) {
            pr.writeln("X");
        } else {
            pr.writeln(p.c - 1, p.r - 1);
        }
    }
    return 0;
}

