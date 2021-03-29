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

int A, B;

const ll M = 1e9;

struct Testcase {
	int test_idx_;
	Testcase(int tidx) : test_idx_(tidx) {}
	//~ bool win = false;

	string Ask(int x, int y) {
		if (abs(x) > M || abs(y) > M) { return "MISS"; }
		cout << x << " " << y << endl;
		string ans;
		cin >> ans;
		if (ans == "CENTER") { throw 42; }
		return ans;
	}

	void Run() {
		debug() << imie(test_idx_);
		int in_x, in_y;
		while (true) {
			in_x = rand() % (2 * M + 1) - M;
			in_y = rand() % (2 * M + 1) - M;
			string ans = Ask(in_x, in_y);
			if (ans == "HIT") { break; }
		}
		debug() << imie(in_x) << imie(in_y);

		debug() << imie("left");
		int min_x = -M - 1, max_x = in_x;
		while (max_x - min_x > 1) {
			const int x = (min_x + max_x) / 2;
			if (Ask(x, in_y) == "HIT") {
				max_x = x;
			} else {
				min_x = x;
			}
		}

		const int Lx = max_x;

		debug() << imie("up");
		int min_y = in_y, max_y = M + 1;
		while (max_y - min_y > 1) {
			const int y = (min_y + max_y) / 2;
			if (Ask(Lx, y) == "HIT") {
				min_y = y;
			} else {
				max_y = y;
			}
		}
		debug() << imie(min_y);
		const int Uy = min_y;
		debug() << imie(Uy);

		debug() << imie("right");
		min_x = Lx, max_x = M + 1;
		while (max_x - min_x > 1) {
			const int x = (min_x + max_x) / 2;
			if (Ask(x, Uy) == "HIT") {
				min_x = x;
			} else {
				max_x = x;
			}
		}

		const int Rx = min_x;
		debug() << imie(Rx);
		
		debug() << "down";
		min_y = -M - 1; max_y = Uy;
		while (max_y - min_y > 1) {
			const int y = (min_y + max_y) / 2;
			if (Ask(Rx, y) == "HIT") {
				max_y = y;
			} else {
				min_y = y;
			}
		}
		debug() << imie(max_y);
		const int Dy = max_y;
		debug() << imie(Dy);

		const int ans_x = (Lx + Rx) / 2;
		const int ans_y = (Dy + Uy) / 2;
		debug() << imie(ans_x) << imie(ans_y);

		Ask(ans_x, ans_y);
		assert(false);
	}
};

int main() {
	int T;
	cin >> T >> A >> B;

	for (int i = 1; i <= T; ++i) {
		try {
			Testcase(i).Run();
		} catch(int x) {}
	}
}
