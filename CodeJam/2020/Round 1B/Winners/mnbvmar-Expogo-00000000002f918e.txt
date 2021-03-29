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
		int x, y;
		cin >> x >> y;
		cout << "Case #" << test_idx_ << ": ";

		if (abs(x) % 2 == abs(y) % 2) {
			cout << "IMPOSSIBLE\n";
			return;
		}

		int px = abs(x), py = abs(y);
		string ans(100, '-');

		int idx = 0;
		while (px || py) {
			debug() << imie(x) << imie(y) << imie(px) << imie(py);
			if (px % 2 == 1) {
				bool inc_dist;
				if (px == 1 && py == 0) {
					inc_dist = false;
				} else if (py % 4 == 0) {
					// make px "%4 == 2"
					inc_dist = (px % 4 == 1);
				} else {
					// make px "%4 == 0"
					inc_dist = (px % 4 == 3);
				}
				
				bool inc_x = (inc_dist == (x > 0));
				ans[idx] = (!inc_x ? 'E' : 'W');
				debug() << imie(idx) << imie(inc_dist) << imie(inc_x);
				px += (inc_dist ? 1 : -1);
				x += (inc_x ? 1 : -1);
			} else if (py % 2 == 1) {
				bool inc_dist;
				if (py == 1 && px == 0) {
					inc_dist = false;
				} else if (px % 4 == 0) {
					// make px "%4 == 2"
					inc_dist = (py % 4 == 1);
				} else {
					// make px "%4 == 0"
					inc_dist = (py % 4 == 3);
				}
				bool inc_y = (inc_dist == (y > 0));
				debug() << imie(idx) << imie(inc_dist) << imie(inc_y);
				ans[idx] = (!inc_y ? 'N' : 'S');
				py += (inc_dist ? 1 : -1);
				y += (inc_y ? 1 : -1);
			} else {
				assert(false);
			}

			px /= 2; py /= 2; x /= 2; y /= 2;
			++idx;
		}

		while (ans.back() == '-') { ans.pop_back(); }

		cout << ans << "\n";
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
