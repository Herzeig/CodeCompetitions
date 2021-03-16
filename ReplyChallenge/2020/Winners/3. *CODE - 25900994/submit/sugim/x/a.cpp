#include <bits/stdc++.h>
using namespace std;

#define rep(i, N) for (int i = 0; i < (N); i++)
#define all(a) (a).begin(), (a).end()
#define pb push_back

using ll = long long;
using i_i = tuple<int, int>;

struct dice {
	mt19937 mt;
	dice() {
		random_device rd;
		mt = mt19937(rd());
	}
	int operator()(int x) { return this->operator()(0, x - 1); }
	int operator()(int x, int y) {
		uniform_int_distribution<int> dist(x, y);
		return dist(mt);
	}
} dc;

const int F = 100;

int H, W, D, M, N, C, S;
vector<vector<int>> a;
vector<int> company, bonus;
vector<vector<int>> skills;
vector<bitset<400>> skills_b;
vector<vector<int>> friends;
vector<int> company_size;

vector<string> uniq(vector<string> a) {
    sort(all(a));
    a.erase(unique(all(a)), a.end());
    return a;
}

void input() {
    cin >> W >> H;
    a = vector<vector<int>>(H, vector<int>(W));
    rep(y, H) {
        string s; cin >> s;
        rep(x, W) a[y][x] = string("#_M").find(s[x]);
    }
    cin >> D;
    vector<string> company_s, skill_s;
    vector<vector<string>> skills_s;
    rep(d, D) {
        string c;
        int b, n;
        cin >> c >> b >> n;
        company_s.pb(c);
        bonus.pb(b);
        vector<string> v;
        while (n--) {
            string s; cin >> s;
            v.pb(s);
            skill_s.pb(s);
        }
        skills_s.pb(v);
    }
    cin >> M;
    rep(m, M) {
        int n;
        string c;
        int b;
        cin >> c >> b;
        company_s.pb(c);
        bonus.pb(b);
        vector<string> v;
        skills_s.pb(v);
    }
    vector<string> company_uniq = uniq(company_s);
    vector<string> skill_uniq = uniq(skill_s);
    C = company_uniq.size();
    S = skill_uniq.size();
    N = D + M;
    company.resize(N);
    skills.resize(N);
    rep(i, N) {
        company[i] = lower_bound(all(company_uniq), company_s[i]) - company_uniq.begin();
        for (string s: skills_s[i]) skills[i].pb(lower_bound(all(skill_uniq), s) - skill_uniq.begin());
    }
    skills_b.resize(N);
    rep(i, N) for (int s: skills[i]) skills_b[i][s] = 1;
}

void info() {
    cerr << H << ' ' << W << ' ' << D << ' ' << M << ' ' << C << ' ' << S << endl;
    int d_count = 0, m_count = 0;
    rep(y, H) rep(x, W) if (a[y][x] == 1) d_count++;
    rep(y, H) rep(x, W) if (a[y][x] == 2) m_count++;
    cerr << D << ' ' << d_count << endl;
    cerr << M << ' ' << m_count << endl;
    cerr << C << endl;
    vector<vector<int>> bonus_by_company(C);
    rep(i, N) bonus_by_company[company[i]].pb(bonus[i]);
    rep(c, C) {
        for (int b: bonus_by_company[c]) cerr << b << ' ';
        cerr << endl << endl;
    }
}

int f(int i, int j) {
    int x = (skills_b[i] & skills_b[j]).count();
    int y = (skills_b[i] | skills_b[j]).count();
    return x * (y - x) + (company[i] == company[j] ? bonus[i] * bonus[j] : 0);
}

void calc_friends() {
    friends.resize(N);
    rep(i, N) {
        vector<i_i> score_i(N);
        rep(j, N) score_i[j] = i_i(f(i, j), j);
        sort(all(score_i));
        reverse(all(score_i));
        rep(f, min(N, F)) friends[i].pb(get<1>(score_i[f]));
    }
}

int dy[] = {0, -1, 0, 1};
int dx[] = {-1, 0, 1, 0};

vector<vector<int>> greedy() {
    vector<vector<int>> b(H, vector<int>(W, -1));
    vector<bool> used(N);
    int score = 0;
    rep(t, N) {
        cerr << t << endl;
        vector<tuple<int, int, int>> v;
        rep(y, H) rep(x, W) if (b[y][x] != -1) {
            int i = b[y][x];
            for (int j: friends[i]) if (!used[j]) rep(k, 4) {
                int _y = y + dy[k];
                int _x = x + dx[k];
                if (0 <= _y && _y < H && 0 <= _x && _x < W && b[_y][_x] == -1)
                    if ((j < D && a[_y][_x] == 1) || (j >= D && a[_y][_x] == 2))
                        v.pb(make_tuple(j, _y, _x));
            }
        }
        if (v.empty()) {
            cerr << "empty" << endl;
            int i0 = -1, y0 = -1, x0 = -1;
            int ma = -1;
            rep(i, N) if (!used[i] && i >= D && bonus[i] > ma) i0 = i, ma = bonus[i];
            if (i0 != -1) {
                int cnt = 0;
                rep(y, H) rep(x, W) if (b[y][x] == -1 && a[y][x] == 2) {
                    cnt++;
                    if (dc(cnt) == 0) y0 = y, x0 = x;
                }
            }
            if (y0 == -1) {
                i0 = -1;
                int ma = -1;
                rep(i, N) if (!used[i] && i < D && bonus[i] > ma) i0 = i, ma = bonus[i];
                int cnt = 0;
                rep(y, H) rep(x, W) if (b[y][x] == -1 && a[y][x] == 1) {
                    cnt++;
                    if (dc(cnt) == 0) y0 = y, x0 = x;
                }
            }
            if (i0 == -1 || y0 == -1) break;
            v.pb(make_tuple(i0, y0, x0));
        }
        int ma = -1;
        int i0, y0, x0;
        for (auto iyx: v) {
            int i, y, x;
            tie(i, y, x) = iyx;
            int sum = 0;
            rep(k, 4) {
                int _y = y + dy[k];
                int _x = x + dx[k];
                if (0 <= _y && _y < H && 0 <= _x && _x < W && b[_y][_x] != -1)
                    sum += f(i, b[_y][_x]);
            }
            if (sum > ma) ma = sum, i0 = i, y0 = y, x0 = x;
        }
        b[y0][x0] = i0;
        used[i0] = true;
        score += ma;
        cerr << i0 << ' ' << y0 << ' ' << x0 << endl;
    }
    cerr << score << endl;
    return b;
}

int main() {
    input();
    
    // info();
    
    calc_friends();
    
    company_size.resize(C);
    rep(i, N) company_size[company[i]]++;
    
    vector<vector<int>> b = greedy();
    vector<i_i> ans(N, i_i(-1, -1));
    rep(y, H) rep(x, W) if (b[y][x] != -1) ans[b[y][x]] = i_i(y, x);
    rep(i, N) {
        int y, x;
        tie(y, x) = ans[i];
        if (y == -1) cout << 'X' << endl;
        else cout << x << ' ' << y << endl;
    }
}
