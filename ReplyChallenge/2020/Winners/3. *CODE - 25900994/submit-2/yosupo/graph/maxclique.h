#pragma once

#include "base.h"

template <int N, class E> struct MaxClique {
    using B = bitset<N>;
    int n;
    V<B> g, col_buf;
    V<int> clique, now;
    struct P {
        int id, col, deg;
    };
    VV<P> rems;
    void dfs(int dps = 0) {
        if (clique.size() < now.size()) clique = now;
        auto& rem = rems[dps];
        sort(rem.begin(), rem.end(), [&](P a, P b) { return a.deg > b.deg; });
        int max_c = 1;
        for (auto& p : rem) {
            p.col = 0;
            while ((g[p.id] & col_buf[p.col]).any()) p.col++;
            max_c = max(max_c, p.id + 1);
            col_buf[p.col].set(p.id);
        }
        for (int i = 0; i < max_c; i++) col_buf[i].reset();
        sort(rem.begin(), rem.end(), [&](P a, P b) { return a.col < b.col; });

        while (!rem.empty()) {
            auto p = rem.back();
            if (now.size() + p.col + 1 <= clique.size()) break;

            auto& nrem = rems[dps + 1];
            nrem.clear();
            B bs = B();
            for (auto q : rem) {
                if (g[p.id][q.id]) {
                    nrem.push_back({q.id, -1, 0});
                    bs.set(q.id);
                }
            }
            for (auto& q : nrem) {
                q.deg = int((bs & g[q.id]).count());
            }
            now.push_back(p.id);
            dfs(dps + 1);
            now.pop_back();

            rem.pop_back();
        }
    }

    MaxClique(VV<E> _g) : n(int(_g.size())), g(n), col_buf(n), rems(n + 1) {
        for (int i = 0; i < n; i++) {
            rems[0].push_back({i, -1, int(_g[i].size())});
            for (auto e : _g[i]) g[i][e.to] = 1;
        }
        dfs();
    }
};
