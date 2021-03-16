#include <bits/stdc++.h>

using namespace std;

struct Customer {
    int row, col, score;
};

int TEST_IDX;
int ROWS, COLS, OFFICES;
vector<Customer> CUSTOMERS;
vector<string> MAP;
vector<vector<int>> SCORE_MAP;

void GetParams(int argc, char** argv) {
    TEST_IDX = stoi(argv[1]);
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

void Solve() {
    /*
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j)
            cerr << SCORE_MAP[i][j] << " ";
        cerr << endl;
    }
    */

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
        /*
        for (int i = 0; i < ROWS; ++i) {
            for (int j = 0; j < COLS; ++j)
                cerr << ret[i][j] << " ";
        cerr << endl;
        }
        */

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
    /*
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j)
            cerr << potence[i][j] << " ";
        cerr << endl;
    }
    */

    vector<tuple<int, int, int>> candidates;
    for (int i = 0; i < ROWS; ++i)
    for (int j = 0; j < COLS; ++j) {
        if (customer_at(i, j) == -1)
            candidates.emplace_back(potence[i][j], i, j);
    }

    sort(candidates.rbegin(), candidates.rend());
    candidates.resize(OFFICES);

    auto output_path = [&](int i, int j, vector<vector<int>>& scores) {
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
    };

    for (auto itr : candidates) {
        int i, j; tie(ignore, i, j) = itr;
        for (int k = 0; k < (int)CUSTOMERS.size(); ++k) {
            if (scores[k][i][j] != -1 && CUSTOMERS[k].score - scores[k][i][j] >= 0) {
                output_path(i, j, scores[k]);
            }
        }
    }
}

int main(int argc, char** argv) {
    GetParams(argc, argv);
    ReadInput();
    Solve();
    return 0;
}
