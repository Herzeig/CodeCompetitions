#ifndef LOCAL
#pragma GCC optimize("O3")
#endif
#include "bits/stdc++.h"

#define ALL(x) (x).begin(), (x).end()
#define SZ(x) ((int)(x).size())
#define st first
#define nd second

using namespace std;

//~ #include <ext/pb_ds/assoc_container.hpp>
//~ #include <ext/pb_ds/tree_policy.hpp>
//~ using namespace __gnu_pbds;
//~ template <typename T>
//~ using ordered_set =
	//~ tree<T, null_type, less<T>, rb_tree_tag, tree_order_statistics_node_update>;


// Weird debugging stuff.
#define sim template < class c
#define ris return * this
#define dor > debug & operator <<
#define eni(x) sim > typename \
  enable_if<sizeof dud<c>(0) x 1, debug&>::type operator<<(c i) {
sim > struct rge { c b, e; };
sim > rge<c> range(c i, c j) { return rge<c>{i, j}; }
sim > auto dud(c* x) -> decltype(cerr << *x, 0);
sim > char dud(...);
struct debug {
#ifdef LOCAL
~debug() { cerr << endl; }
eni(!=) cerr << boolalpha << i; ris; }
eni(==) ris << range(begin(i), end(i)); }
sim, class b dor(pair < b, c > d) {
  ris << "(" << d.first << ", " << d.second << ")";
}
sim dor(rge<c> d) {
  *this << "[";
  for (auto it = d.b; it != d.e; ++it)
	*this << ", " + 2 * (it == d.b) << *it;
  ris << "]";
}
#else
sim dor(const c&) { ris; }
#endif
};
#define imie(...) " [" << #__VA_ARGS__ ": " << (__VA_ARGS__) << "] "
// End of weird debugging stuff.

#ifdef LOCAL
mt19937 rng(unsigned(69));
#else
mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
#endif
#define rand rng

using ll=long long;
using pii=pair<int,int>;
using pll=pair<ll,ll>;
using vi=vector<int>;
using vll=vector<ll>;

struct Testcase {
	int test_idx_;
	Testcase(int tidx) : test_idx_(tidx) {}


	void Run() {
		int R, S;
		cin >> R >> S;
		debug() << imie(R) << imie(S);

		const int guess = (R * (S - 1) + 1) / 2;
		vector<pii> layout;
		for (int i = 0; i < S; ++i) {
			for (int j = 1; j <= R; ++j) {
				layout.emplace_back(j, 1);
			}
		}
		debug() << imie(layout);

		vector<pii> ops;

		while (SZ(layout) >= R + 2) {
			int a = layout[0].st;
			int b = layout[1].st;
			int cnta = layout[0].nd;
			int cntb = layout[1].nd;

			int ptr = SZ(layout) - 2;
			while (layout[ptr].st != a || layout[ptr+1].st != b) {
				--ptr;
			}
			assert(ptr > 0);

			int move_left = cnta + cntb;
			int move_right = 0;
			for (int i = 2; i <= ptr; ++i) {
				move_right += layout[i].nd;
			}
			ops.emplace_back(move_left, move_right);
			layout[ptr].nd += cnta;
			layout[ptr+1].nd += cntb;

			layout.erase(layout.begin(), layout.begin() + 2);
			debug() << imie(layout);
		}

		if (SZ(layout) == R + 1) {
			assert(layout[0].st == R);
			int move_left = layout[0].nd;
			int move_right = 0;
			for (int i = 1; i < SZ(layout) - 1; ++i) {
				move_right += layout[i].nd;
			}
			ops.emplace_back(move_left, move_right);
			layout.back().nd += move_left;
			layout.erase(layout.begin());
			debug() << imie(layout);
		}

		assert(SZ(ops) == guess);

		cout << "Case #" << test_idx_ << ": " << guess << "\n";
		for (auto &op : ops) {
			cout << op.st << " " << op.nd << "\n";
		}
	}
};

int main() {
	ios_base::sync_with_stdio(0);
	cin.tie(0);
	cout << fixed << setprecision(11);
	cerr << fixed << setprecision(6);

	int T;
	cin >> T;
	for (int i = 1; i <= T; ++i) {
		Testcase(i).Run();
	}
}
