//
// Created by Manuele Ghignoli on 20/02/22.
//

//Stringhe

/* DUVAL - returns 0-indexed position of the least cyclic shift */
template <typename T>
int duval(int n, const T &s) {
    assert(n >= 1);
    int i = 0, ans = 0;
    while (i < n) {
        ans = i;
        int j = i + 1, k = i;
        while (j < n + n && !(s[j % n] < s[k % n])) {
            if (s[k % n] < s[j % n]) {
                k = i;
            } else {
                k++;
            }
            j++;
        }
        while (i <= k) {
            i += j - k;
        }
    }
    return ans;
    // returns 0-indexed position of the least cyclic shift
}
template <typename T>
int duval(const T &s) {
    return duval((int) s.size(), s);
}

/* DUVAL PREFIX - returns 0-indexed positions of the least cyclic shifts of all prefixes */
template <typename T>
vector<int> duval_prefixes(int n, const T &s) {
    vector<int> z = z_function(n, s);
    vector<int> ans(n, 0);
    int i = 0, pos = 0;
    while (i < n) {
        int j = i, k = i;
        while (j < n) {
            j++;
            if (j > pos) {
                if (z[k] <= pos - k && s[z[k]] < s[k + z[k]]) {
                    int shift = (pos - i) / (j - k) * (j - k);
                    ans[pos] = ans[pos - shift] + shift;
                } else {
                    ans[pos] = i;
                }
                pos++;
            }
            if (s[k] < s[j]) k = i; else
            if (!(s[j] < s[k])) k++; else
                    else break;
        }
        while (i <= k) {
            i += j - k;
        }
    }
    return ans;
    // returns 0-indexed positions of the least cyclic shifts of all prefixes
}
template <typename T>
vector<int> duval_prefixes(const T &s) {
    return duval_prefixes((int) s.size(), s);
}

/* KWP - returns 0-indexed positions of occurrences of w in s */
template <typename T>
vector<int> kmp_table(int n, const T &s) {
    vector<int> p(n, 0);
    int k = 0;
    for (int i = 1; i < n; i++) {
        while (k > 0 && !(s[i] == s[k])) {
            k = p[k - 1];
        }
        if (s[i] == s[k]) {
            k++;
        }
        p[i] = k;
    }
    return p;
}
template <typename T>
vector<int> kmp_table(const T &s) {
    return kmp_table((int) s.size(), s);
}
template <typename T>
vector<int> kmp_search(int n, const T &s, int m, const T &w, const vector<int> &p) {
    assert(n >= 1 && (int) p.size() == n);
    vector<int> res;
    int k = 0;
    for (int i = 0; i < m; i++) {
        while (k > 0 && (k == n || !(w[i] == s[k]))) {
            k = p[k - 1];
        }
        if (w[i] == s[k]) {
            k++;
        }
        if (k == n) {
            res.push_back(i - n + 1);
        }
    }
    return res;
    // returns 0-indexed positions of occurrences of w in s
}
template <typename T>
vector<int> kmp_search(const T &s, const T &w, const vector<int> &p) {
    return kmp_search((int) s.size(), s, (int) w.size(), w, p);
}

