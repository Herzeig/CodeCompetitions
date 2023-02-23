//
// Created by Manuele Ghignoli on 20/02/22.
//

//Grafi

/* Classe Grafo */
template <typename T>
class graph {
public:
    struct edge {
        int from;
        int to;
        T cost;
    };
    vector<edge> edges;
    vector< vector<int> > g;
    int n;
    function<bool(int)> ignore;
    graph(int _n) : n(_n) {
        g.resize(n);
        ignore = nullptr;
    }
    virtual int add(int from, int to, T cost) = 0;
    virtual void set_ignore_edge_rule(const function<bool(int)> &f) {
        ignore = f;
    }
    virtual void reset_ignore_edge_rule() {
        ignore = nullptr;
    }
};

/* Digraph */
template <typename T>
class digraph : public graph<T> {
public:
    using graph<T>::edges;
    using graph<T>::g;
    using graph<T>::n;
    using graph<T>::ignore;
    digraph(int _n) : graph<T>(_n) {
    }
    int add(int from, int to, T cost = 1) {
        assert(0 <= from && from < n && 0 <= to && to < n);
        int id = (int) edges.size();
        g[from].push_back(id);
        edges.push_back({from, to, cost});
        return id;
    }
    digraph<T> reverse() const {
        digraph<T> rev(n);
        for (auto &e : edges) {
            rev.add(e.to, e.from, e.cost);
        }
        if (ignore != nullptr) {
            rev.set_ignore_edge_rule([&](int id) {
                return ignore(id);
            });
        }
        return rev;
    }
};

template <typename T>
class undigraph : public graph<T> {
public:
    using graph<T>::edges;
    using graph<T>::g;
    using graph<T>::n;
    undigraph(int _n) : graph<T>(_n) {
    }
    int add(int from, int to, T cost = 1) {
        assert(0 <= from && from < n && 0 <= to && to < n);
        int id = (int) edges.size();
        g[from].push_back(id);
        g[to].push_back(id);
        edges.push_back({from, to, cost});
        return id;
    }
};

template <typename T>
class forest : public graph<T> {
public:
    using graph<T>::edges;
    using graph<T>::g;
    using graph<T>::n;
    forest(int _n) : graph<T>(_n) {
    }
    int add(int from, int to, T cost = 1) {
        assert(0 <= from && from < n && 0 <= to && to < n);
        int id = (int) edges.size();
        assert(id < n - 1);
        g[from].push_back(id);
        g[to].push_back(id);
        edges.push_back({from, to, cost});
        return id;
    }
};

/******************************************/

// A DFS starting at some vertex v explores the graph by building up a tree that contains all vertices that
// are reachable from v and all edges that are used to reach these vertices. We call this tree a DFS tree.

/* DFS UNDIGRAPH - Deep-first search su grafo non direzionato  */
template <typename T>
class dfs_undigraph : public undigraph<T> {
public:
    using undigraph<T>::edges;
    using undigraph<T>::g;
    using undigraph<T>::n;
    using undigraph<T>::ignore;
    vector<int> pv;
    vector<int> pe;
    vector<int> order;
    vector<int> pos;
    vector<int> end;
    vector<int> sz;
    vector<int> root;
    vector<int> depth;
    vector<int> min_depth;
    vector<T> dist;
    vector<int> was;
    int attempt;
    dfs_undigraph(int _n) : undigraph<T>(_n) {
    }
    void init() {
        pv = vector<int>(n, -1);
        pe = vector<int>(n, -1);
        order.clear();
        pos = vector<int>(n, -1);
        end = vector<int>(n, -1);
        sz = vector<int>(n, 0);
        root = vector<int>(n, -1);
        depth = vector<int>(n, -1);
        min_depth = vector<int>(n, -1);
        dist = vector<T>(n);
        was = vector<int>(n, -1);
        attempt = 0;
    }
    void clear() {
        pv.clear();
        pe.clear();
        order.clear();
        pos.clear();
        end.clear();
        sz.clear();
        root.clear();
        depth.clear();
        min_depth.clear();
        dist.clear();
        was.clear();
    }
private:
    void do_dfs(int v) {
        was[v] = attempt;
        pos[v] = (int) order.size();
        order.push_back(v);
        sz[v] = 1;
        min_depth[v] = depth[v];
        for (int id : g[v]) {
            if (id == pe[v] || (ignore != nullptr && ignore(id))) {
                continue;
            }
            auto &e = edges[id];
            int to = e.from ^ e.to ^ v;
            if (was[to] == attempt) {
                min_depth[v] = min(min_depth[v], depth[to]);
                continue;
            }
            depth[to] = depth[v] + 1;
            dist[to] = dist[v] + e.cost;
            pv[to] = v;
            pe[to] = id;
            root[to] = (root[v] != -1 ? root[v] : to);
            do_dfs(to);
            sz[v] += sz[to];
            min_depth[v] = min(min_depth[v], min_depth[to]);
        }
        end[v] = (int) order.size() - 1;
    }
    void do_dfs_from(int v) {
        ++attempt;
        depth[v] = 0;
        dist[v] = T{};
        root[v] = v;
        pv[v] = pe[v] = -1;
        do_dfs(v);
    }
public:
    void dfs(int v, bool clear_order = true) {
        if (pv.empty()) {
            init();
        } else {
            if (clear_order) {
                order.clear();
            }
        }
        do_dfs_from(v);
    }
    void dfs_all() {
        init();
        for (int v = 0; v < n; v++) {
            if (depth[v] == -1) {
                do_dfs_from(v);
            }
        }
        assert((int) order.size() == n);
    }
};

/* DFS Forest */
// A complete DFS exploring the full graph (and not only the part reachable from a given vertex v)
// builds up a collection of trees, or forest, called a DFS forest.
template <typename T>
class dfs_forest : public forest<T> {
public:
    using forest<T>::edges;
    using forest<T>::g;
    using forest<T>::n;
    using forest<T>::ignore;
    vector<int> pv;
    vector<int> pe;
    vector<int> order;
    vector<int> pos;
    vector<int> end;
    vector<int> sz;
    vector<int> root;
    vector<int> depth;
    vector<T> dist;
    dfs_forest(int _n) : forest<T>(_n) {
    }
    void init() {
        pv = vector<int>(n, -1);
        pe = vector<int>(n, -1);
        order.clear();
        pos = vector<int>(n, -1);
        end = vector<int>(n, -1);
        sz = vector<int>(n, 0);
        root = vector<int>(n, -1);
        depth = vector<int>(n, -1);
        dist = vector<T>(n);
    }
    void clear() {
        pv.clear();
        pe.clear();
        order.clear();
        pos.clear();
        end.clear();
        sz.clear();
        root.clear();
        depth.clear();
        dist.clear();
    }
private:
    void do_dfs(int v) {
        pos[v] = (int) order.size();
        order.push_back(v);
        sz[v] = 1;
        for (int id : g[v]) {
            if (id == pe[v] || (ignore != nullptr && ignore(id))) {
                continue;
            }
            auto &e = edges[id];
            int to = e.from ^ e.to ^ v;
            depth[to] = depth[v] + 1;
            dist[to] = dist[v] + e.cost;
            pv[to] = v;
            pe[to] = id;
            root[to] = (root[v] != -1 ? root[v] : to);
            do_dfs(to);
            sz[v] += sz[to];
        }
        end[v] = (int) order.size() - 1;
    }
    void do_dfs_from(int v) {
        depth[v] = 0;
        dist[v] = T{};
        root[v] = v;
        pv[v] = pe[v] = -1;
        do_dfs(v);
    }
public:
    void dfs(int v, bool clear_order = true) {
        if (pv.empty()) {
            init();
        } else {
            if (clear_order) {
                order.clear();
            }
        }
        do_dfs_from(v);
    }
    void dfs_all() {
        init();
        for (int v = 0; v < n; v++) {
            if (depth[v] == -1) {
                do_dfs_from(v);
            }
        }
        assert((int) order.size() == n);
    }
};

/* Dijkstra */
template <typename T>
vector <T> dijkstra(const graph<T> &g, int start) {
    assert(0 <= start && start < g.n);
    vector<T> dist(g.n, numeric_limits<T>::max());
    priority_queue<pair<T, int>, vector<pair<T, int> >, greater<pair<T, int> > > s;
    dist[start] = 0;
    s.emplace(dist[start], start);
    while (!s.empty()) {
        T expected = s.top().first;
        int i = s.top().second;
        s.pop();
        if (dist[i] != expected) {
            continue;
        }
        for (int id : g.g[i]) {
            if (g.ignore != nullptr && g.ignore(id)) {
                continue;
            }
            auto &e = g.edges[id];
            int to = e.from ^ e.to ^ i;
            if (dist[i] + e.cost < dist[to]) {
                dist[to] = dist[i] + e.cost;
                s.emplace(dist[to], to);
            }
        }
    }
    return dist;
    // returns numeric_limits<T>::max() if there's no path
}

/* Dijkstra-set */
template <typename T>
vector <T> dijkstra(const graph<T> &g, int start) {
    assert(0 <= start && start < g.n);
    vector<T> dist(g.n, numeric_limits<T>::max());
    dist[start] = 0;
    set< pair<T, int> > s;
    s.emplace(dist[start], start);
    while (!s.empty()) {
        int i = s.begin()->second;
        s.erase(s.begin());
        for (int id : g.g[i]) {
            if (g.ignore != nullptr && g.ignore(id)) {
                continue;
            }
            auto &e = g.edges[id];
            int to = e.from ^ e.to ^ i;
            if (dist[i] + e.cost < dist[to]) {
                s.erase({dist[to], to});
                dist[to] = dist[i] + e.cost;
                s.emplace(dist[to], to);
            }
        }
    }
    return dist;
    // returns numeric_limits<T>::max() if there's no path
}

/**************************************/

/* HeavyLigthDecomposition Forest*/
template <typename T>
class hld_forest : public lca_forest<T> {
public:
    using lca_forest<T>::edges;
    using lca_forest<T>::g;
    using lca_forest<T>::n;
    using lca_forest<T>::ignore;
    using lca_forest<T>::pv;
    using lca_forest<T>::sz;
    using lca_forest<T>::pos;
    using lca_forest<T>::order;
    using lca_forest<T>::depth;
    using lca_forest<T>::dfs;
    using lca_forest<T>::dfs_all;
    using lca_forest<T>::lca;
    using lca_forest<T>::build_lca;
    vector<int> head;
    vector<int> visited;
    hld_forest(int _n) : lca_forest<T>(_n) {
        visited.resize(n);
    }
    void build_hld(const vector<int> &vs) {
        for (int tries = 0; tries < 2; tries++) {
            if (vs.empty()) {
                dfs_all();
            } else {
                order.clear();
                for (int v : vs) {
                    assert(depth[v] == -1);
                    dfs(v, false);
                }
                assert((int) order.size() == n);
            }
            if (tries == 1) {
                break;
            }
            for (int i = 0; i < n; i++) {
                if (g[i].empty()) {
                    continue;
                }
                int best = -1, bid = 0;
                for (int j = 0; j < (int) g[i].size(); j++) {
                    int id = g[i][j];
                    if (ignore != nullptr && ignore(id)) {
                        continue;
                    }
                    int v = edges[id].from ^ edges[id].to ^ i;
                    if (pv[v] != i) {
                        continue;
                    }
                    if (sz[v] > best) {
                        best = sz[v];
                        bid = j;
                    }
                }
                swap(g[i][0], g[i][bid]);
            }
        }
        build_lca();
        head.resize(n);
        for (int i = 0; i < n; i++) {
            head[i] = i;
        }
        for (int i = 0; i < n - 1; i++) {
            int x = order[i];
            int y = order[i + 1];
            if (pv[y] == x) {
                head[y] = head[x];
            }
        }
    }
    void build_hld(int v) {
        build_hld(vector<int>(1, v));
    }
    void build_hld_all() {
        build_hld(vector<int>());
    }
/*
  vector< pair<int, int> > get_path(int x, int y) {
    vector< pair<int, int> > path[2];
    int z = lca(x, y);
    for (int id = 0; id < 2; id++) {
      int v = (id == 0 ? x : y);
      while (v != z) {
        if (depth[head[v]] <= depth[z]) {
          path[id].emplace_back(pos[z] + 1, pos[v]);
          break;
        }
        path[id].emplace_back(pos[head[v]], pos[v]);
        v = pv[head[v]];
      }
    }
    vector< pair<int, int> > ret;
    for (int i = 0; i < (int) path[0].size(); i++) {
      ret.emplace_back(path[0][i].second, path[0][i].first);
    }
    ret.emplace_back(-1, pos[z]);
    for (int i = (int) path[1].size() - 1; i >= 0; i--) {
      ret.push_back(path[1][i]);
    }
    return ret;
    // returns segments of the path:
    //   first from x to lca, reversed segments
    //   then (-1, pos[lca])
    //   then from lca to y, direct segments
    // but in most cases, apply_on_path should be enough
  }
*/
    bool apply_on_path(int x, int y, bool with_lca, function<void(int,int,bool)> f) {
        // f(x, y, up): up -- whether this part of the path goes up
        assert(!head.empty());
        int z = lca(x, y);
        if (z == -1) {
            return false;
        }
        {
            int v = x;
            while (v != z) {
                if (depth[head[v]] <= depth[z]) {
                    f(pos[z] + 1, pos[v], true);
                    break;
                }
                f(pos[head[v]], pos[v], true);
                v = pv[head[v]];
            }
        }
        if (with_lca) {
            f(pos[z], pos[z], false);
        }
        {
            int v = y;
            int cnt_visited = 0;
            while (v != z) {
                if (depth[head[v]] <= depth[z]) {
                    f(pos[z] + 1, pos[v], false);
                    break;
                }
                visited[cnt_visited++] = v;
                v = pv[head[v]];
            }
            for (int at = cnt_visited - 1; at >= 0; at--) {
                v = visited[at];
                f(pos[head[v]], pos[v], false);
            }
        }
        return true;
    }
};

/* LowestCommonAncestor Forest */
template <typename T>
class lca_forest : public dfs_forest<T> {
public:
    using dfs_forest<T>::edges;
    using dfs_forest<T>::g;
    using dfs_forest<T>::n;
    using dfs_forest<T>::pv;
    using dfs_forest<T>::pos;
    using dfs_forest<T>::end;
    using dfs_forest<T>::depth;
    int h;
    vector< vector<int> > pr;
    lca_forest(int _n) : dfs_forest<T>(_n) {
    }
    inline void build_lca() {
        assert(!pv.empty());
        int max_depth = 0;
        for (int i = 0; i < n; i++) {
            max_depth = max(max_depth, depth[i]);
        }
        h = 1;
        while ((1 << h) <= max_depth) {
            h++;
        }
        pr.resize(n);
        for (int i = 0; i < n; i++) {
            pr[i].resize(h);
            pr[i][0] = pv[i];
        }
        for (int j = 1; j < h; j++) {
            for (int i = 0; i < n; i++) {
                pr[i][j] = (pr[i][j - 1] == -1 ? -1 : pr[pr[i][j - 1]][j - 1]);
            }
        }
    }
    inline bool anc(int x, int y) {
        return (pos[x] <= pos[y] && end[y] <= end[x]);
    }
    inline int lca(int x, int y) {
        // maybe optimize?
        // if depth[x] > depth[y], swap
        // then go from j = log(depth[x])?
        assert(!pr.empty());
        if (anc(x, y)) {
            return x;
        }
        if (anc(y, x)) {
            return y;
        }
        for (int j = h - 1; j >= 0; j--) {
            if (pr[x][j] != -1 && !anc(pr[x][j], y)) {
                x = pr[x][j];
            }
        }
        return pr[x][0];
    }
};

/**************************************/

template <typename T>
vector<int> find_bicone(dfs_undigraph<T> &g, int &cnt) {
    g.dfs_all();
    vector<int> vertex_comp(g.n);
    cnt = 0;
    for (int i : g.order) {
        if (g.pv[i] == -1 || g.min_depth[i] == g.depth[i]) {
            vertex_comp[i] = cnt++;
        } else {
            vertex_comp[i] = vertex_comp[g.pv[i]];
        }
    }
    return vertex_comp;
}
