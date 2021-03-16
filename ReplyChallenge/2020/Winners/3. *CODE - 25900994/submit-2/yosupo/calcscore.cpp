#include <bits/stdc++.h>
using namespace std;

using ll=long long;
#define int ll

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
	#ifdef LOCAL
	static mt19937_64 gen;
	#else
	static mt19937_64 gen(chrono::steady_clock::now().time_since_epoch().count());
	#endif
	return uniform_int_distribution<ll>(l, r)(gen);
}

template<class t>
void myshuffle(vc<t>&a){
	rep(i,si(a))swap(a[i],a[rand_int(0,i)]);
}

template<class t>
int lwb(const vc<t>&v,const t&a){
	return lower_bound(all(v),a)-v.bg;
}

struct str_to_idx{
	map<string,int> memo;
	int i;
	str_to_idx():i(0){}
	int getid(const string&s){
		if(memo.count(s))return memo[s];
		return memo[s]=i++;
	}
} companyId,skillId;

const int numSkillMax=405;
using B=bitset<numSkillMax>;

struct Person{
	int c,b;
	B ss;
	void readDeveloper(){
		c=companyId.getid(readString());
		cin>>b;
		int s;cin>>s;
		rep(_,s)ss[skillId.getid(readString())]=1;
	}
	void readManager(){
		c=companyId.getid(readString());
		cin>>b;
	}
};

int adjacentScore(const Person&a,const Person&b){
	int res=0;
	if(a.c==b.c)res+=a.b*b.b;
	int x=(a.ss&b.ss).count(),y=(a.ss^b.ss).count();
	return res+x*y;
}

int calcScore(const vc<string>&board,const vc<Person>&ds,const vc<Person>&ms,const vc<pi>&x,const vc<pi>&y){
	int h=si(board);
	int w=si(board[0]);
	int D=si(ds);
	int M=si(ms);
	assert(D==si(x));
	assert(M==si(y));
	
	vvc<int> idx(h,vi(w,-1));
	rep(i,D){
		int r,c;tie(r,c)=x[i];
		if(r==-1&&c==-1)continue;
		if(!inc(0,r,h-1)||!inc(0,c,w-1)){
			cerr<<"Developer "<<i<<" has invalid position "<<x[i]<<endl;
			exit(1);
		}
		if(board[r][c]!='_'){
			cerr<<"Developer "<<i<<" tried to occupy invalid cell "<<x[i]<<" ("<<board[r][c]<<")"<<endl;
			exit(1);
		}
		if(idx[r][c]!=-1){
			cerr<<"Developer "<<i<<" tried to occupy alraedy occupied cell "<<x[i]<<endl;
			exit(1);
		}
		idx[r][c]=i;
	}
	
	rep(i,M){
		int r,c;tie(r,c)=y[i];
		if(r==-1&&c==-1)continue;
		if(!inc(0,r,h-1)||!inc(0,c,w-1)){
			cerr<<"Manager "<<i<<" has invalid position "<<x[i]<<endl;
			exit(1);
		}
		if(board[r][c]!='M'){
			cerr<<"Manager "<<i<<" tried to occupy invalid cell "<<x[i]<<" ("<<board[r][c]<<")"<<endl;
			exit(1);
		}
		if(idx[r][c]!=-1){
			cerr<<"Manager "<<i<<" tried to occupy alraedy occupied cell "<<x[i]<<endl;
			exit(1);
		}
		idx[r][c]=D+i;
	}
	
	auto getPerson=[&](int i)->const Person&{
		return i<D?ds[i]:ms[i-D];
	};
	
	
	int res=0;
	rep(i,h)rep(j,w){
		int p=idx[i][j];
		if(i+1<h){
			int q=idx[i+1][j];
			if(p!=-1&&q!=-1)res+=adjacentScore(getPerson(p),getPerson(q));
		}
		if(j+1<w){
			int q=idx[i][j+1];
			if(p!=-1&&q!=-1)res+=adjacentScore(getPerson(p),getPerson(q));
		}
	}
	
	cerr<<"Success!"<<endl;
	
	return res;
}

pi readPos(ifstream&f){
	//int r,c;cin>>c>>r;
	string s;f>>s;
	if(s=="X"){
		return pi(-1,-1);
	}
	else{
		int y;f>>y;
		return pi(y,stoi(s));
	}
}

signed main(signed argc,char* argv[]){
	cin.tie(0);
	ios::sync_with_stdio(0);
	cout<<fixed<<setprecision(20);
	
	if(argc<=2){
		cerr<<"Not enough arguments"<<endl;
		exit(1);
	}
	
	//ifstream in(argv[1]);
	freopen(argv[1],"r",stdin);
	ifstream out(argv[2]);
	
	int w,h;cin>>w>>h;
	vc<string> board(h);
	rep(i,h)cin>>board[i];
	
	int D;cin>>D;
	vc<Person> ds(D);
	rep(i,D)ds[i].readDeveloper();
	
	int M;cin>>M;
	vc<Person> ms(M);
	rep(i,M)ms[i].readManager();
	
	//cerr<<companyId.memo.size()<<endl;
	//cerr<<skillId.memo.size()<<endl;
	assert(si(skillId.memo)<=numSkillMax);
	
	vc<pi> x(D),y(M);
	rep(i,D)x[i]=readPos(out);
	rep(i,M)y[i]=readPos(out);
	
	cout<<calcScore(board,ds,ms,x,y)<<endl;
}
