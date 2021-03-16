#include <iostream>
#include <vector>
#include <utility>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <map>
#include <bitset>
#include <random>
#include <algorithm>
#include <cassert>
#include <stack>

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

void run();

int main(int argc, char* argv[]) {
    run();
    return 0;
}

struct Person {
    bool is_dev;
    int c;
    int b;
    vector<int> s;

    Person(bool is_dev, int c, int b, vector<int> s) : is_dev(is_dev), c(c), b(b), s(std::move(s)) {}

    Person() = default;
};

int w, h;
char field[1000][1000];
int ans[1000][1000];
Person person[100'000];
vector<int> devs, managers;

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

int dx[4] = {-1, 1, 0, 0};
int dy[4] = {0, 0, -1, 1};

namespace greedy {

    bool used[100000];

    bool check(int i, int j) {
        return 0 <= i && i < h && 0 <= j && j < w && field[i][j] != '#';
    }

    int calc_profit(int xi, int xj, int i) {
        int res = 0;
        rep(dir, 0, 4) {
            int ni = xi + dx[dir];
            int nj = xj + dy[dir];
            if (!check(ni, nj) || ans[ni][nj] == -1) {
                continue;
            }
            res += score(person[i], person[ans[ni][nj]]);
        }
        return res;
    }

    void bfs(int start_i, int start_j) {
        queue<pair<int, int>> q;
        q.emplace(start_i, start_j);
        while (!q.empty()) {
            auto[xi, xj] = q.front();
            q.pop();

            int best_idx = -1;
            int best_profit = -1;

            rep(i, 0, sz(devs) + sz(managers)) {
                if (used[i]) {
                    continue;
                }
                if (person[i].is_dev == (field[xi][xj] == '_')) {
                    if (maxi(best_profit, calc_profit(xi, xj, i))) {
                        best_idx = i;
                    }
                }
            }

            if (best_idx != -1) {
                ans[xi][xj] = best_idx;
                used[best_idx] = true;
            }

            rep(dir, 0, 4) {
                int ni = xi + dx[dir];
                int nj = xj + dy[dir];
                if (check(ni, nj) && ans[ni][nj] == -1) {
                    ans[ni][nj] = -2;
                    q.emplace(ni, nj);
                }
            }
        }
    }

    void greedy() {
        rep(i, 0, h) {
            rep(j, 0, w) {
                if (field[i][j] != '#' && ans[i][j] == -1) {
                    bfs(i, j);
                }
            }
        }
        rep(i, 0, h) {
            rep(j, 0, w) {
                if (ans[i][j] == -2) ans[i][j] = -1;
            }
        }
    }

}

namespace drochka {
    vector<pair<int, int>> pos;
    map<int, vector<int>> ds, ms;
    vector<int> nzds, nzms;
    int n;

    int personal_score(int index) {
        auto [x, y] = pos[index];
        if (x == -1) {
            return 0;
        }

        vector<int> dx = {0, 0, -1, 1};
        vector<int> dy = {-1, 1, 0, 0};
        int sc = 0;
        for (int d = 0; d < 4; ++d) {
            int xx = x + dx[d], yy = y + dy[d];
            if (0 <= xx && xx < h && 0 <= yy && yy < w) {
                sc += score(person[ans[x][y]], person[ans[xx][yy]]);
            }
        }
        return sc;
    }

    inline void new_place(int index, int x, int y) {
        if (x != -1) {
            ans[x][y] = index;
        }
        pos[index] = {x, y};
    }

    inline void swap_people(int i1, int i2) {
        auto [x1, y1] = pos[i1];
        auto [x2, y2] = pos[i2];
        new_place(i1, x2, y2);
        new_place(i2, x1, y1);
    }

    void mod1() {
        // swap two managers from same company
        if (!nzms.empty()) {
            int i1 = nzms[rand() % nzms.size()];
            int c1 = person[i1].c;
            int i2 = i1;
            while (i2 == i1) {
                i2 = ms[c1][rand() % ms[c1].size()];
            }

            int sc1 = personal_score(i1) + personal_score(i2);
            swap_people(i1, i2);
            int sc2 = personal_score(i1) + personal_score(i2);

            if (sc2 > sc1) {
                cerr << "mod1 succ!" << endl;
            } else {
                swap_people(i1, i2);
            }
        }
    }

    void drochilka() {
        srand(239);

        for (const auto& d : devs) {
            ds[person[d].c].push_back(d);
        }
        for (const auto& m : managers) {
            ms[person[m].c].push_back(m);
        }

        for (auto& p : ds) {
            if (p.second.size() > 1) {
                for (int x : p.second) {
                    nzds.push_back(x);
                }
            }
        }
        for (auto& p : ms) {
            if (p.second.size() > 1) {
                for (int x : p.second) {
                    nzms.push_back(x);
                }
            }
        }

        n = devs.size() + managers.size();
        pos.assign(n, pair<int, int>(-1, -1));
        for (int i = 0; i < h; ++i) {
            for (int j = 0; j < w; ++j) {
                if (ans[i][j] != -1) {
                    pos[ans[i][j]] = {i, j};
                }
            }
        }

        for (int i = 0; i < 10000; ++i) {
            mod1();
        }
    }
}

void run() {
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

    greedy::greedy();

    drochka::drochilka();

    int tot = sz(devs) + sz(managers);
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
}