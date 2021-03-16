#include <utility>

#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <set>
#include <map>

using namespace std;

inline void ensure(bool b, string msg) {
    if (!b) {
        cerr << msg << endl;
        exit(239);
    }
}

struct Person {
    bool is_dev;
    int c;
    int b;
    vector<int> s;

    Person(bool is_dev, int c, int b, vector<int> s) : is_dev(is_dev), c(c), b(b), s(std::move(s)) {}

    Person() = default;
};

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

int main(int argc, char* argv[]) {
    string inf(argv[1]);
    string ouf(argv[2]);
    ifstream fin(inf);
    ifstream fout(ouf);

    int w, h;
    fin >> w >> h;
    vector<string> f(h);
    for (int i = 0; i < h; ++i) {
        fin >> f[i];
    }

    auto [devs, mans] = read_people(fin);


    vector<vector<int>> ans(h, vector<int> (w, -1));
    for (int i = 0; i < devs.size(); ++i) {
        string s;
        fout >> s;
        if (s != "X") {
            int y = atoi(s.c_str());
            int x;
            fout >> x;
            ensure(ans[x][y] == -1, "two guys on same place");
            ensure(f[x][y] == '_', "developer on illegal place");
            ans[x][y] = i;
        }
    }
    for (int i = 0; i < mans.size(); ++i) {
        string s;
        fout >> s;
        if (s != "X") {
            int y = atoi(s.c_str());
            int x;
            fout >> x;
            ensure(ans[x][y] == -1, "two guys on same place");
            ensure(f[x][y] == 'M', "manager on illegal place");
            ans[x][y] = devs.size() + i;
        }
    }

    for (auto man : mans) {
        devs.push_back(std::move(man));
    }

    vector<int> dx = {0, 1};
    vector<int> dy = {1, 0};
    long long total = 0;
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            for (int d = 0; d < 2; ++d) {
                int ii = i + dx[d], jj = j + dy[d];
                if (0 <= ii && ii < h && 0 <= jj && jj < w) {
                    if (ans[i][j] != -1 && ans[ii][jj] != -1) {
                        total += score(devs[ans[i][j]], devs[ans[ii][jj]]);
                    }
                }
            }
        }
    }

    cout << total << endl;

    return 0;
}
