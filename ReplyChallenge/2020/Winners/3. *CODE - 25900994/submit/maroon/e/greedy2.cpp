//#pragma GCC optimize("Ofast")
//#pragma GCC target("avx")
//#undef LOCAL

#include "base.h"
#include "util/fast_io.h"

Scanner sc = Scanner(stdin);
Printer pr = Printer(stdout);

int book_n, lib_n, days;
VV<int> joins;
V<int> value;

struct Lib {
    int sign, scan;
    V<int> books;
};
V<Lib> libs;
struct Que {
    Que(int _id): id(_id) {}
    int id;
    V<int> books;
};
using Ans = V<Que>;

// write your code

ll score(const Ans& ans) {
    ll score = 0;
    for (const auto& que: ans) {        
        for (const auto& x: que.books) {
            score += value[x];
        }
    }
    return score;
}

using D = double;
Ans solve() {
    V<bool> used_b(book_n);
    V<bool> used_l(lib_n);
    Ans ans;
    int nw = 0;
    int all_score = 0;
    while (true) {
        D ma_sc = 0;
        int ma = -1;
        for (int i = 0; i < lib_n; i++) {
            if (used_l[i]) continue;
            const auto& lib = libs[i];
            if (nw + lib.sign >= days) continue;
            ll rem = ll(days - nw - lib.sign) * lib.scan;
            D sm = 0;
            for (int j: lib.books) {
                if (used_b[j]) continue;                
                rem--;
                sm += value[j];
                if (rem == 0) break;
            }
            sm /= sqrt(lib.sign);
            if (ma_sc < sm) {
                ma_sc = sm;
                ma = i;
            }
        }
        if (ma == -1) break;        
//        cerr << "USE " << ma << " : " << all_score << endl;
        used_l[ma] = true;
        ans.push_back(Que{ma});
        nw += libs[ma].sign;
        ll rem = ll(days - nw) * libs[ma].scan;
        for (int j: libs[ma].books) {
            if (used_b[j]) continue;                
            rem--;
            all_score += value[j];
            ans.back().books.push_back(j);
            used_b[j] = true;
            if (rem == 0) break;
        }
    }
    cerr << all_score << endl;
    return ans;
}

int main() {
    sc.read(book_n, lib_n, days);
    value = V<int>(book_n);
    for (int i = 0; i < book_n; i++) {
        sc.read(value[i]);
    }
    joins = VV<int>(book_n);
    V<int> val(days);
    libs = V<Lib>(lib_n);
    for (int i = 0; i < lib_n; i++) {
        int k;
        sc.read(k, libs[i].sign, libs[i].scan);
        for (int j = 0; j < k; j++) {
            int x;
            sc.read(x);
            joins[x].push_back(i);
            libs[i].books.push_back(x);
        }
        sort(libs[i].books.begin(), libs[i].books.end(), [&](int x, int y) {
            return value[x] > value[y];
        });
    }
    for (int i = 0; i < book_n; i++) {
        sort(joins[i].begin(), joins[i].end());
        joins[i].erase(unique(joins[i].begin(), joins[i].end()), joins[i].end());
    }

    auto ans = solve();

    //normalize
    int cnt = 0;    
    for (const auto& a: ans) {
        if (!a.books.empty()) cnt++;
    }
    pr.writeln(cnt);
    for (auto a: ans) {
        if (a.books.empty()) continue;
        pr.writeln(a.id, a.books.size());
        for (int x: a.books) {
            pr.write(x);
            pr.write(' ');
        }
        pr.writeln();
    }
    return 0;
}

