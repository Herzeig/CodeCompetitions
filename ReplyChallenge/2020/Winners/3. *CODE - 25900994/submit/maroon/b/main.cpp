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

void addpi(pi&a,pi b){
	a.a+=b.a;
	a.b+=b.b;
}

pi adjacentScore(const Person&a,const Person&b){
	return pi(a.c==b.c?a.b*b.b:0,(a.ss&b.ss).count()*(a.ss^b.ss).count());
}

int pitoi(pi a){
	return a.a+a.b;
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
	
	pi res(0,0);
	rep(i,h)rep(j,w){
		int p=idx[i][j];
		if(i+1<h){
			int q=idx[i+1][j];
			if(p!=-1&&q!=-1)addpi(res,adjacentScore(getPerson(p),getPerson(q)));
		}
		if(j+1<w){
			int q=idx[i][j+1];
			if(p!=-1&&q!=-1)addpi(res,adjacentScore(getPerson(p),getPerson(q)));
		}
	}
	
	cerr<<"Success!"<<endl;
	cout<<"Bonus Score: "<<res.a<<endl;
	cout<<"Skill Score: "<<res.b<<endl;
	cout<<"Total Score: "<<res.a+res.b<<endl;
	
	return res.a+res.b;
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

const int dyx[]{-1,0,1,0,-1};

signed main(signed argc,char*argv[]){
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
	
	int cs=companyId.memo.size();
	
	vc<pi> x(D),y(M);
	rep(i,D)x[i]=readPos(out);
	rep(i,M)y[i]=readPos(out);
	
	vvc<int> idx(h,vi(w,-1));
	vvc<int> di(cs),mi(cs);
	rep(i,D){
		if(x[i]!=pi(-1,-1)){
			idx[x[i].a][x[i].b]=i;
			di[ds[i].c].pb(i);
		}
	}
	rep(i,M){
		if(y[i]!=pi(-1,-1)){
			idx[y[i].a][y[i].b]=D+i;
			mi[ms[i].c].pb(D+i);
		}
	}
	
	int score=0;
	
	const int ITR=1000000;
	
	vc<Person> ps=ds;
	ps.insert(ps.ed,all(ms));
	
	x.insert(x.ed,all(y));
	
	auto getid=[&](int r,int c){
		if(inc(0,r,h-1)&&inc(0,c,w-1))return idx[r][c];
		else return -1;
	};
	
	auto calcadj=[&](int i){
		int r,c;tie(r,c)=x[i];
		int res=0;
		rep(dir,4){
			int j=getid(r+dyx[dir],c+dyx[dir+1]);
			if(j!=-1)res+=pitoi(adjacentScore(ps[i],ps[j]));
		}
		return res;
	};
	
	auto doswap=[&](int i,int j){
		assert((i<D)==(j<D));
		swap(x[i],x[j]);
		swap(idx[x[i].a][x[i].b],idx[x[j].a][x[j].b]);
	};
	
	auto tryswap=[&](int i,int j){
		int res=-calcadj(i)-calcadj(j);
		doswap(i,j);
		res+=calcadj(i)+calcadj(j);
		if(res>=0){
			if(res>0)cerr<<res<<endl;
			score+=res;
		}else{
			doswap(i,j);
		}
	};
	
	rep(_,ITR){
		if(_%100000==0)cerr<<"Itr: "<<_<<endl;
		int c=rand_int(0,cs-1);
		int s=si(di[c])+si(mi[c]);
		if(s==0)continue;
		int i=rand_int(0,s-1);
		if(i<si(di[c])){
			i=di[c][i];
		}else{
			i=mi[c][i-si(di[c])];
		}
		int rr,cc;tie(rr,cc)=x[i];
		rep(dir,4){
			i=idx[rr][cc];
			int j=getid(rr+dyx[dir],cc+dyx[dir+1]);
			if(j!=-1){
				int d=ps[j].c;
				if(i<D){
					if(si(di[d]))
						j=di[d][rand_int(0,si(di[d])-1)];
					else j=-1;
				}else{
					if(si(mi[d]))
						j=mi[d][rand_int(0,si(mi[d])-1)];
					else j=-1;
				}
				if(j!=-1){
					tryswap(i,j);
				}
			}
		}
	}
	
	for(auto rc:x){
		if(rc.a==-1)
			cout<<"X"<<endl;
		else
			cout<<rc.b<<" "<<rc.a<<endl;
	}
}
