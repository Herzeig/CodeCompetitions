#include <bits/stdc++.h>
using namespace std;

using ll=long long;
//#define int ll

#define rng(i,a,b) for(int i=int(a);i<int(b);i++)
#define rep(i,b) rng(i,0,b)
#define gnr(i,a,b) for(int i=int(b)-1;i>=int(a);i--)
#define per(i,b) gnr(i,0,b)
#define pb push_back
#define eb emplace_back
#define a first
#define b second
#define bg begin()
#define ed end()
#define all(x) x.bg,x.ed
#define si(x) int(x.size())
#ifdef LOCAL
#define dmp(x) cerr<<__LINE__<<" "<<#x<<" "<<x<<endl
#else
#define dmp(x) void(0)
#endif

template<class t,class u> void chmax(t&a,u b){if(a<b)a=b;}
template<class t,class u> void chmin(t&a,u b){if(b<a)a=b;}

template<class t> using vc=vector<t>;
template<class t> using vvc=vc<vc<t>>;

using pi=pair<int,int>;
using vi=vc<int>;

template<class t,class u>
ostream& operator<<(ostream& os,const pair<t,u>& p){
	return os<<"{"<<p.a<<","<<p.b<<"}";
}

template<class t> ostream& operator<<(ostream& os,const vc<t>& v){
	os<<"{";
	for(auto e:v)os<<e<<",";
	return os<<"}";
}

#define mp make_pair
#define mt make_tuple
#define one(x) memset(x,-1,sizeof(x))
#define zero(x) memset(x,0,sizeof(x))
#ifdef LOCAL
void dmpr(ostream&os){os<<endl;}
template<class T,class... Args>
void dmpr(ostream&os,const T&t,const Args&... args){
	os<<t<<" ";
	dmpr(os,args...);
}
#define dmp2(...) dmpr(cerr,__LINE__,##__VA_ARGS__)
#else
#define dmp2(...) void(0)
#endif

using uint=unsigned;
using ull=unsigned long long;

template<class t,size_t n>
ostream& operator<<(ostream&os,const array<t,n>&a){
	return os<<vc<t>(all(a));
}

template<int i,class T>
void print_tuple(ostream&,const T&){
}

template<int i,class T,class H,class ...Args>
void print_tuple(ostream&os,const T&t){
	if(i)os<<",";
	os<<get<i>(t);
	print_tuple<i+1,T,Args...>(os,t);
}

template<class ...Args>
ostream& operator<<(ostream&os,const tuple<Args...>&t){
	os<<"{";
	print_tuple<0,tuple<Args...>,Args...>(os,t);
	return os<<"}";
}

template<class t>
void print(t x,int suc=1){
	cout<<x;
	if(suc==1)
		cout<<"\n";
	if(suc==2)
		cout<<" ";
}

ll read(){
	ll i;
	cin>>i;
	return i;
}

vi readvi(int n,int off=0){
	vi v(n);
	rep(i,n)v[i]=read()+off;
	return v;
}

template<class T>
void print(const vector<T>&v,int suc=1){
	rep(i,v.size())
		print(v[i],i==int(v.size())-1?suc:2);
}

string readString(){
	string s;
	cin>>s;
	return s;
}

template<class T>
T sq(const T& t){
	return t*t;
}

//#define CAPITAL
void yes(bool ex=true){
	#ifdef CAPITAL
	cout<<"YES"<<"\n";
	#else
	cout<<"Yes"<<"\n";
	#endif
	if(ex)exit(0);
}
void no(bool ex=true){
	#ifdef CAPITAL
	cout<<"NO"<<"\n";
	#else
	cout<<"No"<<"\n";
	#endif
	if(ex)exit(0);
}
void possible(bool ex=true){
	#ifdef CAPITAL
	cout<<"POSSIBLE"<<"\n";
	#else
	cout<<"Possible"<<"\n";
	#endif
	if(ex)exit(0);
}
void impossible(bool ex=true){
	#ifdef CAPITAL
	cout<<"IMPOSSIBLE"<<"\n";
	#else
	cout<<"Impossible"<<"\n";
	#endif
	if(ex)exit(0);
}

constexpr ll ten(int n){
	return n==0?1:ten(n-1)*10;
}

const ll infLL=LLONG_MAX/3;

#ifdef int
const int inf=infLL;
#else
const int inf=INT_MAX/2-100;
#endif

int topbit(signed t){
	return t==0?-1:31-__builtin_clz(t);
}
int topbit(ll t){
	return t==0?-1:63-__builtin_clzll(t);
}
int botbit(signed a){
	return a==0?32:__builtin_ctz(a);
}
int botbit(ll a){
	return a==0?64:__builtin_ctzll(a);
}
int popcount(signed t){
	return __builtin_popcount(t);
}
int popcount(ll t){
	return __builtin_popcountll(t);
}
bool ispow2(int i){
	return i&&(i&-i)==i;
}
ll mask(int i){
	return (ll(1)<<i)-1;
}

bool inc(int a,int b,int c){
	return a<=b&&b<=c;
}

template<class t> void mkuni(vc<t>&v){
	sort(all(v));
	v.erase(unique(all(v)),v.ed);
}

ll rand_int(ll l, ll r) { //[l, r]
	static mt19937_64 gen;
    return uniform_int_distribution<ll>(l, r)(gen);
}

double rnd_d(){
	return double(rand_int(0,INT_MAX))/INT_MAX;
}

template<class t>
int lwb(const vc<t>&v,const t&a){
	return lower_bound(all(v),a)-v.bg;
}


chrono::system_clock::time_point startTime;
int Elapsed(){
	auto cur=chrono::system_clock::now();
	auto msecs=chrono::duration_cast<chrono::milliseconds>(cur-startTime);
	return msecs.count();
}


bool has_in_array(const vi&x,int v){
	auto itr=lower_bound(all(x),v);
	return itr!=x.ed && *itr==v;
}

struct Library{
	int n,t,m;
	vi id;
	template<class f>
	void readinit(f&ins){
		ins>>n>>t>>m;
		id.resize(n);
		rep(i,n)ins>>id[i];
		sort(all(id));
	}
};

struct Instruction{
	int l;
	vi id;
};

int calcscore(const int bs,const int ls,const int ds,const vi&sc,const vc<Library>&lib,vc<Instruction> ins){
	map<int,int> book;
	vi used(ls);
	for(auto z:ins){
		if(!inc(0,z.l,ls-1)){
			cerr<<"Invalid ID of Library: "<<z.l<<endl;
			exit(1);
		}
		if(used[z.l]){
			cerr<<"Duplcate Library: "<<z.l<<endl;
			exit(1);
		}
		if(z.id.empty()){
			cerr<<"Empty Library: "<<z.l<<endl;
			exit(1);
		}
		used[z.l]=1;
		for(auto i:z.id){
			if(!inc(0,i,bs-1)){
				cerr<<"Invalid ID of Book: "<<i<<endl;
				exit(1);
			}
			if(book.count(i)){
				cerr<<"Duplicate Book from Libraries "<<book[i]<<" and "<<z.l<<endl;
				exit(1);
			}
			book[i]=z.l;
			if(!has_in_array(lib[z.l].id,i)){
				cerr<<"Library "<<z.l<<" doesn't have Book "<<i<<endl;
				exit(1);
			}
		}
	}
	
	int cur=0,s=0;
	for(auto z:ins){
		int i=z.l;
		cur+=lib[i].t;
		if(cur>=ds)break;
		int u=min(ll(si(z.id)+1),ll(ds-cur)*lib[i].m);
		if(si(z.id)>u){
			cerr<<"Library "<<i<<" tries to ship Books after the period"<<endl; 
		}
		chmin(u,si(z.id));
		rep(j,u)
			s+=sc[z.id[j]];
	}
	
	//cout<<s<<endl;
	cerr<<s<<endl;
	return s;
}

void output(const vc<Instruction>&ins){
	print(si(ins));
	for(auto z:ins){
		cout<<z.l<<" "<<si(z.id)<<endl;
		print(z.id);
	}
}

signed main(signed argc,char* argv[]){
	cin.tie(0);
	ios::sync_with_stdio(0);
	cout<<fixed<<setprecision(20);
	
	startTime=chrono::system_clock::now();
	
	int bs,ls,ds;
	
	cin>>bs>>ls>>ds;
	
	vi sc(bs);
	rep(i,bs)
		cin>>sc[i];
	
	vc<Library> lib(ls);
	rep(i,ls)
		lib[i].readinit(cin);
	
	vi idx;
	{
		assert(argc>=2);
		ifstream ysp(argv[1]);
		int n;ysp>>n;
		int cur=0;
		rep(_,n){
			int i;ysp>>i;
			cur+=lib[i].t;
			int k;ysp>>k;
			int tmp;
			rep($,k)ysp>>tmp;
			idx.pb(i);
		}
	}
	
	vi sum(ls);
	rep(i,ls)for(auto j:lib[i].id)sum[i]+=sc[j];
	
	{
		sort(all(idx),[&](int i,int j){
			//return sum[i]*lib[j].t>sum[j]*lib[i].t;
			return lib[i].t<lib[j].t;
			//return sum[i]>sum[j];
		});
	
		//int h=si(idx)/2;
		int h=si(idx);
		
		vi used(ls);
		int num=ds;
		
		vi bu(bs);
		rep(ii,h){
			int i=idx[ii];
			used[i]=1;
			num-=lib[i].t;
			for(auto j:lib[i].id)
				bu[j]=1;
		}
		
		int s=0;
		rep(i,bs)if(bu[i])s+=sc[i];
		
		vi idx(ls);
		iota(all(idx),0);
		sort(all(idx),[&](int i,int j){
			return sum[i]*lib[j].t>sum[j]*lib[i].t;
			//return lib[i].t<lib[j].t;
			//return sum[i]>sum[j];
		});
		
		//vi sum(ls);
		//rep(i,ls)for(auto j:lib[i].id)if(!bu[j])sum[i]+=sc[j];
		
		{
			set<int> b;
			for(auto i:idx){
				if(used[i])continue;
				sum[i]=0;
				for(auto j:lib[i].id)
					if(!bu[j]&&!b.count(j)){
						b.insert(j);
						sum[i]+=sc[j];
					}
			}
		}
		
		vi dp(num);
		rep(i,ls)if(!used[i]){
			gnr(j,lib[i].t,num)
				chmax(dp[j],dp[j-lib[i].t]+sum[i]);
		}
		cerr<<s+dp[num-1]<<endl;
		
		return 0;
	}
	
	set<int> b;
	
	vc<Instruction> ins;
	
	int cur=0;
	for(auto i:idx){
		cur+=lib[i].t;
		if(cur>=ds)break;
		int u=min(ll(si(lib[i].id)),ll(ds-cur)*lib[i].m);
		vi x;
		vi tmp=lib[i].id;
		sort(all(tmp),[&](int k,int j){
			return sc[k]>sc[j];
		});
		for(auto j:tmp)
			if(si(x)<u&&!b.count(j)){
				b.insert(j);
				x.pb(j);
			}
		if(si(x))
			ins.pb({i,x});
	}
	
	calcscore(bs,ls,ds,sc,lib,ins);
	output(ins);
}
