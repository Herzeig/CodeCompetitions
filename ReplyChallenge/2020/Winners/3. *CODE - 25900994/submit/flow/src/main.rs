#![allow(non_snake_case)]
#[allow(unused)]
use std::collections::*;
#[allow(unused)]
use std::io::prelude::*;
#[allow(unused)]
use rayon::prelude::*;
#[allow(unused)]
use rand::prelude::*;

pub trait SetMinMax {
	fn setmin(&mut self, v: Self) -> bool;
	fn setmax(&mut self, v: Self) -> bool;
}
impl<T> SetMinMax for T where T: PartialOrd {
	fn setmin(&mut self, v: T) -> bool {
		*self > v && { *self = v; true }
	}
	fn setmax(&mut self, v: T) -> bool {
		*self < v && { *self = v; true }
	}
}

#[allow(unused)]
macro_rules! mat {
	($($e:expr),*) => { Vec::from(vec![$($e),*]) };
	($($e:expr,)*) => { Vec::from(vec![$($e),*]) };
	($e:expr; $d:expr) => { Vec::from(vec![$e; $d]) };
	($e:expr; $d:expr $(; $ds:expr)+) => { Vec::from(vec![mat![$e $(; $ds)*]; $d]) };
}

#[allow(unused)]
macro_rules! readln {
	() => {{
		readln!(::std::io::stdin())
	}};
	($i:expr) => {{
		let mut line = String::new();
		$i.read_line(&mut line).unwrap_or_else(|e| panic!("{}", e));
		line
	}}
}

#[allow(unused)]
macro_rules! read {
	($($t:tt),*; $n:expr) => {{
		let stdin = ::std::io::stdin();
		read_from!(stdin.lock(); $($t),*; $n)
	}};
	($($t:tt),*) => {{
		read_from!(::std::io::stdin(); $($t),*)
	}};
}

#[allow(unused)]
macro_rules! read_from {
	($i:expr; $($t:tt),*; $n:expr) => {{
		let ret = ::std::io::BufRead::lines($i).take($n).map(|line| {
			let line = line.unwrap();
			let mut it = line.split_whitespace();
			_read!(it; $($t),*)
		}).collect::<Vec<_>>();
		ret
	}};
	($i:expr; $($t:tt),*) => {{
		let line = readln!($i);
		#[allow(unused_mut)]
		let mut it = line.split_whitespace();
		_read!(it; $($t),*)
	}};
}

#[allow(unused)]
macro_rules! _read {
	($it:ident; [char]) => {
		_read!($it; String).chars().collect::<Vec<_>>()
	};
	($it:ident; [u8]) => {
		Vec::from(_read!($it; String).into_bytes())
	};
	($it:ident; usize1) => {
		$it.next().unwrap_or_else(|| panic!("input mismatch")).parse::<usize>().unwrap_or_else(|e| panic!("{}", e)) - 1
	};
	($it:ident; [usize1]) => {
		$it.map(|s| s.parse::<usize>().unwrap_or_else(|e| panic!("{}", e)) - 1).collect::<Vec<_>>()
	};
	($it:ident; [$t:ty]) => {
		$it.map(|s| s.parse::<$t>().unwrap_or_else(|e| panic!("{}", e))).collect::<Vec<_>>()
	};
	($it:ident; $t:ty) => {
		$it.next().unwrap_or_else(|| panic!("input mismatch")).parse::<$t>().unwrap_or_else(|e| panic!("{}", e))
	};
	($it:ident; $($t:tt),+) => {
		($(_read!($it; $t)),*)
	};
}

#[allow(unused)]
macro_rules! impl_cmp {
	($name:ident $(<$($t:ident),*>)*; |$x:ident, $y:ident| $e:expr; $($w:tt)*) => {
		impl $(<$($t),*>)* Ord for $name $(<$($t),*>)* $($w)* {
			#[inline]
			fn cmp(&self, $y: &Self) -> ::std::cmp::Ordering {
				let $x = &self;
				$e
			}
		}
		impl $(<$($t),*>)* PartialOrd for $name $(<$($t),*>)* $($w)* {
			#[inline]
			fn partial_cmp(&self, a: &Self) -> Option<::std::cmp::Ordering> {
				Some(self.cmp(a))
			}
		}
		impl $(<$($t),*>)* PartialEq for $name $(<$($t),*>)* $($w)* {
			#[inline]
			fn eq(&self, a: &Self) -> bool {
				self.cmp(a) == ::std::cmp::Ordering::Equal
			}
		}
		impl $(<$($t),*>)* Eq for $name $(<$($t),*>)* $($w)* {}
	}
}

#[derive(Clone, Copy, Debug, Default, Hash)]
pub struct Tot<T: PartialOrd>(pub T);
impl_cmp!(Tot<T>; |a, b| a.0.partial_cmp(&b.0).unwrap(); where T: PartialOrd);

#[derive(Clone, Copy, Debug, Default, Hash)]
pub struct Rev<T: PartialOrd>(pub T);
impl_cmp!(Rev<T>; |a, b| b.0.partial_cmp(&a.0).unwrap(); where T: PartialOrd);

pub fn get_time() -> f64 {
	static mut STIME: f64 = -1.0;
	let t = std::time::SystemTime::now().duration_since(std::time::UNIX_EPOCH).unwrap();
	let ms = t.as_secs() as f64 + t.subsec_nanos() as f64 * 1e-9;
	unsafe {
		if STIME < 0.0 {
			STIME = ms;
		}
		ms - STIME
	}
}

pub fn main() {
	let _ = ::std::thread::Builder::new().name("run".to_string()).stack_size(32 * 1024 * 1024).spawn(run).unwrap().join();
}

/////////////////////////////////////////////////

#[derive(Clone)]
pub struct Input {
	id: &'static str,
	B: usize,
	L: usize,
	D: usize,
	S: Vec<i64>,
	books: Vec<Vec<usize>>,
	Ts: Vec<usize>,
	Ms: Vec<usize>,
}

fn read_input() -> Input {
	let (B, L, D) = read!(usize, usize, usize);
	let S = read!([i64]);
	let mut books = vec![];
	let mut Ts = vec![];
	let mut Ms = vec![];
	for _ in 0..L {
		let (_N, T, M) = read!(usize, usize, usize);
		Ts.push(T);
		Ms.push(M);
		books.push(read!([usize]));
	}
	let id = match D {
		7 => "a",
		1000 => "b",
		100000 => "c",
		30001 => "d",
		200 => "e",
		700 => "f",
		_ => "x"
	};
	Input { id, B, L, D, S, books, Ts, Ms }
}

#[derive(Clone)]
pub struct Output {
	score: i64,
	out: Vec<(usize, Vec<usize>)>,
}

fn compute_score(input: &Input, out: &Vec<(usize, Vec<usize>)>) -> i64 {
	let mut score = 0;
	for i in 0..out.len() {
		for &b in &out[i].1 {
			score += input.S[b];
		}
	}
	score
}

fn read_output(input: &Input, f: &str) -> Output {
	let mut r = std::io::BufReader::new(std::fs::File::open(f).unwrap());
	let n = read_from!(r; usize);
	let mut out = vec![];
	for _ in 0..n {
		let (i, _) = read_from!(r; usize, usize);
		out.push((i, read_from!(r; [usize])));
	}
	Output { score: compute_score(input, &out), out }
}

fn write_output<W: Write>(out: &Output, mut w: W) {
	writeln!(w, "{}", out.out.len()).unwrap();
	for (i, bs) in &out.out {
		writeln!(w, "{} {}", i, bs.len()).unwrap();
		let mut first = true;
		for j in bs {
			if !first {
				write!(w, " ").unwrap();
			}
			first = false;
			write!(w, "{}", j).unwrap();
		}
		writeln!(w).unwrap();
	}
}

fn solve(input: &Input) -> Output {
	let mut order = (0..input.L).map(|i| (Tot(input.Ms[i] as f64 / input.Ts[i] as f64), i)).collect::<Vec<_>>();
	order.sort();
	order.reverse();
	let score = 0;
	let mut out = vec![];
	let mut t = 0;
	for (_, i) in order {
		if t + input.Ts[i] <= input.D {
			t += input.Ts[i];
			out.push((i, vec![]));
		}
	}
	Output { score, out }
}

pub fn solve2(input: &Input) -> Output {
	let mut order = (0..input.L).map(|i| (input.Ts[i], i)).collect::<Vec<_>>();
	order.sort();
	let score = 0;
	let mut out = vec![];
	let mut t = 0;
	for (_, i) in order {
		if t + input.Ts[i] <= input.D {
			t += input.Ts[i];
			out.push((i, vec![]));
		}
	}
	Output { score, out }
}

pub mod mcf {
	use crate::*;
	use std::ops::*;
	
	type V = usize;
	
	#[derive(Copy, Clone, Debug)]
	pub struct E<C, W> {
		pub to: V,
		pub cap: C,
		pub init: C,
		pub cost: W,
		pub rev: usize
	}
	
	#[derive(Clone, Debug)]
	pub struct Graph<C, W> {
		pub es: Vec<Vec<E<C, W>>>,
		pub ex: Vec<C>,
		pub p: Vec<W>,
		iter: Vec<usize>
	}
	
	impl<C, W> Graph<C, W> where	C: Copy + Default + Ord + Sub<Output = C> + Neg<Output = C> + AddAssign + SubAssign,
									W: Copy + Default + Ord + Add<Output = W> + Sub<Output = W> + Mul<Output = W> + Div<Output = W> + Neg<Output = W>
															+ AddAssign + SubAssign + MulAssign + DivAssign + From<u32> {
		pub fn new(n: usize) -> Graph<C, W> {
			Graph { es: vec![vec![]; n], ex: vec![C::default(); n], p: vec![W::default(); n], iter: vec![0; n] }
		}
		pub fn add(&mut self, v: V, to: V, cap: C, cost: W) {
			let (fwd, rev) = (self.es[v].len(), self.es[to].len());
			self.es[v].push(E { to: to, cap: cap, init: cap, cost: cost, rev: rev });
			self.es[to].push(E { to: v, cap: C::default(), init: C::default(), cost: -cost, rev: fwd });
		}
		fn is_admissible(&self, v: V, e: &E<C, W>) -> bool {
			e.cap > C::default() && e.cost + self.p[v] - self.p[e.to] < W::default()
		}
		/// Compute minimum cost circulation.
		/// Return whether there is a flow satisfying the demand constraints.
		/// flow(e) = init(e) - cap(e).
		/// For solving min cost s-t flow of value F, set ex(s)=F and ex(t)=-F.
		/// For every vertex, the total capacity of its incident edges must be fit in C.
		/// C and W are expected to be i64 or Float (do not use f64).
		/// Dual: minimize \sum_v ex(v)p(v) + \sum_{uv} cap(e) max(0, -cost(uv) - p(u) + p(v)).
		/// To get the dual optimum, call fitting if W is integer, or use p/(n+1) if W is float.
		/// O(V^2 E log VC), where C=max(cost(e)). When cap=1, O(V E log VC).
		pub fn solve(&mut self) -> bool {
			let n = self.es.len();
			let mut eps = W::from(2);
			for v in &mut self.es {
				for e in v {
					e.cost *= (n as u32 + 1).into();
					eps.setmax(e.cost);
				}
			}
			let mut stack = vec![];
			let mut visit = vec![false; n];
			let mut ok = false;
			'refine: while { eps /= 2.into(); eps > W::default() } {
				for v in 0..n {
					for i in 0..self.es[v].len() {
						let e = self.es[v][i];
						if self.is_admissible(v, &e) {
							self.ex[e.to] += e.cap;
							self.ex[v] -= e.cap;
							self.es[e.to][e.rev].cap += e.cap;
							self.es[v][i].cap = C::default();
						}
					}
				}
				loop {
					for v in 0..n {
						self.iter[v] = 0;
						if self.ex[v] > C::default() {
							visit[v] = true;
							stack.push(v);
						} else {
							visit[v] = false;
						}
					}
					if stack.len() == 0 { break }
					while let Some(v) = stack.pop() {
						for e in &self.es[v] {
							if !visit[e.to] && self.is_admissible(v, e) {
								visit[e.to] = true;
								stack.push(e.to);
							}
						}
					}
					if (0..n).filter(|&v| visit[v]).flat_map(|v| self.es[v].iter()).all(|e| e.cap <= C::default() || visit[e.to]) {
						assert!(!ok);
						break 'refine;
					}
					for v in (0..n).filter(|&v| visit[v]) { self.p[v] -= eps }
					for v in 0..n {
						while self.ex[v] > C::default() {
							let f = self.dfs(v, self.ex[v]);
							if f == C::default() { break }
							else { self.ex[v] -= f }
						}
					}
				}
				ok = true;
			}
			for v in &mut self.es {
				for e in v {
					e.cost /= (n as u32 + 1).into();
				}
			}
			ok
		}
		fn dfs(&mut self, v: V, f: C) -> C {
			if self.ex[v] < C::default() {
				let d = ::std::cmp::min(f, -self.ex[v]);
				self.ex[v] += d;
				return d;
			}
			while self.iter[v] < self.es[v].len() {
				let e = self.es[v][self.iter[v]];
				if self.is_admissible(v, &e) {
					let d = self.dfs(e.to, ::std::cmp::min(f, e.cap));
					if d > C::default() {
						self.es[v][self.iter[v]].cap -= d;
						self.es[e.to][e.rev].cap += d;
						return d;
					}
				}
				self.iter[v] += 1;
			}
			C::default()
		}
		pub fn val<T>(&self) -> T where T: Default + From<C> + From<W> + AddAssign + Mul<Output = T> {
			let mut tot = T::default();
			for v in &self.es {
				for e in v {
					if e.cap < e.init {
						tot += T::from(e.init - e.cap) * e.cost.into();
					}
				}
			}
			tot
		}
		/// Find p s.t. cost(uv) + p(u) - p(v) >= 0 holds for every edge uv with cap(uv) > 0.
		/// W is expected to be i64. When W is float, use p/(n+1).
		/// O(E log V)
		pub fn fitting(&mut self) {
			let n = self.es.len();
			let mut d: Vec<W> = self.p.iter().map(|&a| a / (n as u32 + 1).into()).collect(); // p must be non-positive.
			let mut d2: Vec<W> = (0..n).map(|v| d[v] * (n as u32 + 1).into() - self.p[v] + 1.into()).collect();
			let mut fixed = vec![false; n];
			let mut que = ::std::collections::BinaryHeap::new();
			for v in 0..n {
				que.push((-d2[v], v))
			}
			while let Some((_, v)) = que.pop() {
				if fixed[v] { continue }
				fixed[v] = true;
				for e in &self.es[v] {
					if e.cap > C::default() && !fixed[e.to] && { let tmp = d2[v] + e.cost * (n as u32 + 1).into() + self.p[v] - self.p[e.to] + 1.into(); d2[e.to].setmin(tmp) } {
						d[e.to] = d[v] + e.cost;
						que.push((-d2[e.to], e.to));
					}
				}
			}
			self.p = d
		}
	}
}

fn optimize(input: &Input, best: &mut Output) {
	let order = best.out.iter().map(|&(i, _)| i).collect::<Vec<_>>();
	let mut g = mcf::Graph::<i64, i64>::new(order.len() + input.B + 1);
	let r = order.len() + input.B;
	let mut t = 0;
	for i in 0..order.len() {
		let j = order[i];
		t += input.Ts[j];
		if t <= input.D {
			let k = ((input.D - t) * input.Ms[j]).min(input.books[j].len()) as i64;
			g.add(r, i, k, 0);
			for &b in &input.books[j] {
				g.add(i, order.len() + b, 1, 0);
			}
		}
	}
	for b in 0..input.B {
		g.add(order.len() + b, r, 1, -input.S[b]);
	}
	g.solve();
	let mut out = vec![];
	let mut score = 0;
	for i in 0..order.len() {
		out.push((order[i], vec![]));
		for e in g.es[i].iter() {
			if e.cap < e.init && order.len() <= e.to && e.to < r {
				let b = e.to - order.len();
				score += input.S[b];
				out[i].1.push(b);
			}
		}
	}
	eprintln!("{}", score);
	out = out.iter().cloned().filter(|(_, a)| a.len() > 0).collect();
	if best.score < score {
		best.out = out;
		best.score = score;
	}
}

pub fn optimize2(input: &Input, best: &mut Output) {
	for rep in 0..10 {
		let mut out = best.clone();
		if rep > 0 {
			let mut used = vec![false; input.L];
			for i in 0..out.out.len() {
				assert!(!used[out.out[i].0]);
				used[out.out[i].0] = true;
			}
			let mut ps = vec![];
			let mut t = 0;
			for i in 0..best.out.len() {
				let j = best.out[i].0;
				t += input.Ts[j];
				let k = ((input.D - t) * input.Ms[j]).min(input.books[j].len());
				if best.out[i].1.len() < k {
					ps.push((i + 100, i));
				} else {
					ps.push((i, i));
				}
			}
			ps.sort();
			out.out = vec![];
			for i in 0..best.out.len() {
				out.out.push(best.out[ps[i].1].clone());
			}
			let mut used = vec![false; input.L];
			for i in 0..out.out.len() {
				assert!(!used[out.out[i].0]);
				used[out.out[i].0] = true;
			}
		}
		let mut g = mcf::Graph::<i64, i64>::new(out.out.len() + input.B + 1);
		let r = out.out.len() + input.B;
		let mut t = 0;
		for i in 0..out.out.len() {
			let j = out.out[i].0;
			t += input.Ts[j];
			let k = ((input.D - t) * input.Ms[j]).min(input.books[j].len()) as i64;
			g.add(r, i, k, 0);
			for &b in &input.books[j] {
				g.add(i, out.out.len() + b, 1, 0);
			}
		}
		for b in 0..input.B {
			g.add(out.out.len() + b, r, 1, -input.S[b]);
		}
		eprintln!("start flow");
		g.solve();
		eprintln!("end flow");
		out.score = 0;
		for i in 0..out.out.len() {
			out.out[i].1.clear();
			for e in g.es[i].iter() {
				if e.cap < e.init && out.out.len() <= e.to && e.to < r {
					let b = e.to - out.out.len();
					out.score += input.S[b];
					out.out[i].1.push(b);
				}
			}
		}
		out.out = out.out.iter().cloned().filter(|(_, a)| a.len() > 0).collect();
		if rep == 0 || best.score < out.score {
			eprintln!("Score = {}", out.score);
			*best = out;
		} else {
			break;
		}
	}
}

fn run() {
	let input = read_input();
	let mut best = if let Some(f) = std::env::args().nth(1) {
		read_output(&input, &f)
	} else {
		let out = solve(&input);
		write_output(&out, std::io::BufWriter::new(std::fs::File::create(format!("out/{}.{}.out", input.id, out.score)).unwrap()));
		out
	};
	eprintln!("Score = {}", best.score);
	optimize(&input, &mut best);
	eprintln!("Score = {}", best.score);
	write_output(&best, std::io::stdout().lock());
}
