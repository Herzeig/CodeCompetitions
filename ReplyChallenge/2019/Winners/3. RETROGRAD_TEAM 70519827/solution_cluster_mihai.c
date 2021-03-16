#include <bits/stdc++.h>

using namespace std;

struct Customer {
    int row, col, score;
};

int KMEANS_ITERS = 10;
int TEST_IDX, N_CCS;
int ROWS, COLS, OFFICES;
vector<Customer> CUSTOMERS;
vector<string> MAP;
vector<vector<int>> SCORE_MAP;
vector<vector<int>> CC;
vector<vector<vector<int>>> SCORES;
vector<vector<int>> POTENCE;
vector<vector<int>> CUSTS_IN_CC;

void GetParams(int argc, char** argv) {
    TEST_IDX = stoi(argv[1]);
    KMEANS_ITERS = stoi(argv[2]);
}

int customer_at(int i, int j) {
    for (int k = 0; k < (int)CUSTOMERS.size(); ++k)
        if (CUSTOMERS[k].row == i && CUSTOMERS[k].col == j)
            return k;
    return -1;
}

char get_dir(int i, int j, int ip, int jp) {
    int di = ip - i, dj = jp - j;
    assert(abs(di) + abs(dj) == 1);
    if (di == -1) return 'U';
    if (di == +1) return 'D';
    if (dj == -1) return 'L';
    if (dj == +1) return 'R';
    assert(false);
}

bool USE_CSCORE = true;
int get_score(int i, int j, int k) {
    int ret = SCORES[k][i][j];
    if (ret == -1) return -1e9;
    if (USE_CSCORE) return CUSTOMERS[k].score - ret;
    return -ret;
}

void BuildCC() {
    CC.assign(ROWS, vector<int>(COLS, -1));

    N_CCS = 0;
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            if (SCORE_MAP[i][j] == -1) continue;
            if (CC[i][j] != -1) continue;

            vector<pair<int, int>> q;
            auto push = [&](int i, int j) {
                if (i < 0 || j < 0 || i >= ROWS || j >= COLS) return;
                if (SCORE_MAP[i][j] == -1 || CC[i][j] != -1) return;
                CC[i][j] = N_CCS;
                q.emplace_back(i, j);
            };

            push(i, j);
            for (int k = 0; k < (int)q.size(); ++k) {
                int i, j; tie(i, j) = q[k];
                push(i + 1, j);
                push(i, j + 1);
                push(i - 1, j);
                push(i, j - 1);
            }
            ++N_CCS;
        }
    }

    cerr << N_CCS << " connected components." << endl;
    CUSTS_IN_CC.resize(N_CCS);

    for (int i = 0; i < (int)CUSTOMERS.size(); ++i) {
        auto& c = CUSTOMERS[i];
        CUSTS_IN_CC[CC[c.row][c.col]].push_back(i);
    }

    for (int i = 0; i < N_CCS; ++i) {
        if (CUSTS_IN_CC[i].empty()) continue;
        cerr << "Component #" << i << " has " << CUSTS_IN_CC[i].size() << " customers." << endl;
    }
}

void ReadInput() {
    int C;
    cin >> COLS >> ROWS >> C >> OFFICES;
    MAP.assign(ROWS, "");
    for (int i = 0; i < C; ++i) {
        int row, col, score; cin >> col >> row >> score;
        CUSTOMERS.push_back(Customer{row, col, score});
    }
    for (int i = 0; i < ROWS; ++i) {
        cin >> MAP[i];
        assert(MAP[i].size() == COLS);
    }

    for (auto c : CUSTOMERS) assert(MAP[c.row][c.col] != '#');

    // Make score map.
    map<char, int> char_to_score = {
        {'#', -1}, {'~', 800}, {'*', 200}, {'+', 150}, {'X', 120}, {'_', 100}, {'H', 70}, {'T', 50}
    };
    SCORE_MAP.assign(ROWS, vector<int>(COLS, 0));
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            SCORE_MAP[i][j] = char_to_score[MAP[i][j]];
        }
    }
}

void OutputPath(int i, int j, int k) {
    auto& scores = SCORES[k];
    cout << j << " " << i << " ";
    while (scores[i][j] > 0) {
        int ni = -1, nj = -1;
        auto chk = [&](int ci, int cj) {
            if (ci < 0 || ci >= ROWS || cj < 0 || cj >= COLS) return;
            if (SCORE_MAP[ci][cj] == -1) return;
            if (ni == -1 || scores[ni][nj] + SCORE_MAP[ni][nj]
                > scores[ci][cj] + SCORE_MAP[ci][cj]) {
                    ni = ci; nj = cj;
                }
        };
        chk(i - 1, j);
        chk(i + 1, j);
        chk(i, j - 1);
        chk(i, j + 1);

        assert(scores[ni][nj] + SCORE_MAP[ni][nj] == scores[i][j]);
        cout << get_dir(i, j, ni, nj);
        i = ni; j = nj;
    }
    assert(scores[i][j] == 0);
    cout << endl;
}

void MakeScoresPotence() {
    auto make_score_matrix = [&](Customer& c) {
        priority_queue<tuple<int, int, int>> pq;
        vector<vector<int>> ret(ROWS, vector<int>(COLS, -1));
        auto push = [&](int i, int j, int d) {
            if (i < 0 || j < 0 || i >= ROWS || j >= COLS) return;
            if (ret[i][j] != -1 && ret[i][j] <= d) return;
            if (SCORE_MAP[i][j] == -1) return;

            ret[i][j] = d;
            pq.emplace(-d, i, j);
        };
        assert(SCORE_MAP[c.row][c.col] != -1);
        push(c.row, c.col, 0);

        while (!pq.empty()) {
            int d, i, j; tie(d, i, j) = pq.top(); pq.pop(); d = -d;
            if (ret[i][j] != d) continue;

            d += SCORE_MAP[i][j];
            push(i - 1, j, d);
            push(i + 1, j, d);
            push(i, j - 1, d);
            push(i, j + 1, d);
        }

        return ret;
    };

    vector<vector<vector<int>>> scores(CUSTOMERS.size());
    vector<vector<int>> potence(ROWS, vector<int>(COLS, 0));
    
    for (int i = 0; i < (int)CUSTOMERS.size(); ++i) {
        scores[i] = make_score_matrix(CUSTOMERS[i]);
        for (int j = 0; j < ROWS; ++j) 
        for (int k = 0; k < COLS; ++k) {
            if (scores[i][j][k] == -1) continue;
            potence[j][k] += max(0, CUSTOMERS[i].score - scores[i][j][k]);
        }
    }

    SCORES = scores;
    POTENCE = potence;
}

pair<int, vector<pair<int, int>>> KMeans(int cc, int k) {
    cerr << "Entered KMeans on cc #" << cc << " with k = " << k << endl;
    USE_CSCORE = false;
    vector<pair<int, int>> positions;
    for (int i = 0; i < ROWS; ++i)
    for (int j = 0; j < COLS; ++j)
        if (CC[i][j] == cc && customer_at(i, j) == -1)
            positions.emplace_back(i, j);
    random_shuffle(positions.begin(), positions.end());
    // cerr << "SIZE: " << positions.size() << endl;

    if (k > positions.size()) return {1e9, {}};
    vector<pair<int, int>> cands(positions.begin(), positions.begin() + k);

    auto custs = CUSTS_IN_CC[cc];
    vector<int> assigned(custs.size());
    auto assign = [&]() {
        for (int i = 0; i < (int)custs.size(); ++i) {
            assigned[i] = -1;
            for (int j = 0; j < (int)cands.size(); ++j) {
                int old = assigned[i];
                if (old == -1 || get_score(cands[old].first, cands[old].second, custs[i])
                    < get_score(cands[j].first, cands[j].second, custs[i])) {
                        assigned[i] = j;
                    }
            }
        }
    };

    // cerr << "Assigning" << endl;

    for (int its = 0; its < KMEANS_ITERS; ++its) {
        cerr << ".";
        assign();

        auto old_cands = cands;
        auto get_score_at = [&](int i, int j, int k) {
            int ans = 0;
            for (int c = 0; c < (int)custs.size(); ++c) {
                if (assigned[c] != k) continue;
                ans += get_score(i, j, custs[c]);
            }
            return ans;
        };
        for (int k = 0; k < (int)cands.size(); ++k) {
            int best = -1e9, ci = -1, cj = -1;
            for (auto itr : positions) {
                int i, j; tie(i, j) = itr;
                int now = get_score_at(i, j, k);
                if (now > best) {
                    best = now;
                    ci = i; cj = j;
                }
            }
            assert(ci != -1);
            cands[k] = {ci, cj};
        }

        if (cands == old_cands) { cerr << "EARLYSTOP"; break; }
    }
    assign();
    cerr << endl;

    int result = 0;
    for (int i = 0; i < (int)custs.size(); ++i) {
        int j = assigned[i];
        result += get_score(cands[j].first, cands[j].second, custs[i]);
    }

    // cerr << "Exit KMeans" << endl;
    USE_CSCORE = true;
    
    return {result, cands};
}

int DoMagic(vector<pair<int, int>> cands, bool output) {
    int expected_score = 0;
    for (int k = 0; k < (int)CUSTOMERS.size(); ++k) {
        int ci = -1, cj = -1, best = -1e9;

        for (auto itr : cands) {
            int i, j; tie(i, j) = itr;
            int now = get_score(i, j, k);
            if (now > best) {
                best = now;
                ci = i; cj = j;
            }
        }

        if (ci == -1) continue;

        if (best >= 0) {
            for (auto itr : cands) {
                int i, j; tie(i, j) = itr;
                if (get_score(i, j, k) >= 0) {
                    expected_score += get_score(i, j, k);
                    if (output) OutputPath(i, j, k);
                }
            }
        } else {
            expected_score += best;
            if (output) OutputPath(ci, cj, k);
        }
    }
    if (output) cerr << "EXPECTED SCORE: " << expected_score << endl;
    return expected_score;
}

void SolveCluster() {
    vector<vector<int>> dp, use;
    vector<int> useful;

    dp.emplace_back(OFFICES + 1, -1e9);
    use.emplace_back(OFFICES + 1, -1);
    dp[0][0] = 0;

    for (int cc = 0; cc < N_CCS; ++cc) {
        if (CUSTS_IN_CC[cc].empty()) continue;
        cerr << "Solving cluster problem for cc #" << cc << endl;
        useful.push_back(cc);
        dp.emplace_back(OFFICES + 1, -1e9);
        use.emplace_back(OFFICES + 1, -1);
        int last = dp.size() - 2;

        for (int k = 1; k <= OFFICES; ++k) {
            int now_cost = KMeans(cc, k).first;
            for (int x = 0; x + k <= OFFICES; ++x) {
                if (dp[last + 1][x + k] >= dp[last][x] + now_cost) continue;
                dp[last + 1][x + k] = dp[last][x] + now_cost;
                use[last + 1][x + k] = k;
            }
        }
    }

    vector<tuple<int, int, int>> other_cands;
    for (int i = 0; i < ROWS; ++i)
    for (int j = 0; j < COLS; ++j) {
        if (SCORE_MAP[i][j] == -1) continue;
        if (customer_at(i, j) != -1) continue;
        other_cands.emplace_back(POTENCE[i][j], i, j);
    }
    sort(other_cands.rbegin(), other_cands.rend());

    vector<pair<int, int>> solution;
    int best = -1e9;

    for (int rem = 1; rem <= OFFICES; ++rem) {
        cerr << dp.back()[rem] << " ";
    }
    cerr << endl;

    for (int rem = 1; rem <= OFFICES; ++rem) {
        int at = dp.size() - 1;
        vector<pair<int, int>> cands;
        bool failed = false;
        int aux_rem = rem;

        while (at > 0) {
            int k = use[at][rem];
            if (k == -1) { failed = true; break; }
            auto now_cands = KMeans(useful[at - 1], k).second;
            for (auto cand : now_cands)
                cands.push_back(cand);
            at -= 1;
            rem -= k;
        }

        rem = aux_rem;

        if (failed) {
            cerr << "rem = " << rem << " FAILED" << endl;
            continue;
        }

        assert(cands.size() == rem);
        
        set<pair<int, int>> cset(cands.begin(), cands.end());
        for (int i = 0; i < (int)other_cands.size() && cset.size() < OFFICES; ++i) {
            int x, y; tie(ignore, x, y) = other_cands[i];
            cset.insert({x, y});
        }

        cands = vector<pair<int, int>>(cset.begin(), cset.end());
        
        int now = DoMagic(cands, false);
        if (best < now) {
            best = now;
            solution = cands;
        }
    }
    cerr << "BEST: " << best << endl;
    DoMagic(solution, true);
}

int main(int argc, char** argv) {
    GetParams(argc, argv);
    ReadInput();
    BuildCC();
    MakeScoresPotence();
    SolveCluster();

    return 0;
}
