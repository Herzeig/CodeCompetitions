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

pub struct XorShift {
	pub x: [u32; 4]
}

impl XorShift {
	pub fn new(mut seed: u32) -> XorShift {
		let mut x = [0; 4];
		for i in 0..4 {
			seed = 1812433253u32.wrapping_mul(seed ^ (seed >> 30)).wrapping_add(i as u32);
			x[i] = seed;
		}
		XorShift { x: x }
	}
	pub fn next32(&mut self) -> usize {
		let t = self.x[0] ^ (self.x[0] << 11);
		for i in 0..3 { self.x[i] = self.x[i + 1] }
		self.x[3] = self.x[3] ^ (self.x[3] >> 19) ^ t ^ (t >> 8);
		self.x[3] as usize
	}
	/// [0, n)
	pub fn next(&mut self, n: usize) -> usize {
		loop {
			let t = self.next32();
			let r = t % n;
			if (t - r).checked_add(n).is_some() {
				return r;
			}
		}
	}
}

pub fn main() {
	let _ = ::std::thread::Builder::new().name("run".to_string()).stack_size(32 * 1024 * 1024).spawn(run).unwrap().join();
}

/////////////////////////////////////////////////

#[derive(Clone)]
pub struct Input {
	id: &'static str,
	devs: Vec<(usize, i64, Vec<usize>)>,
	mans: Vec<(usize, i64)>,
	cs: Vec<Vec<char>>,
	C: usize,
	S: usize,
}

fn read_input() -> Input {
	let (_W, H) = read!(usize, usize);
	let cs = read!([char]; H);
	let mut devs = vec![];
	let mut mans = vec![];
	let n = read!(usize);
	let mut com = BTreeMap::new();
	let mut skill = BTreeMap::new();
	for _ in 0..n {
		let (c, b, _, ss) = read!(String, i64, usize, [String]);
		let cn = com.len();
		let c = *com.entry(c).or_insert(cn);
		let mut ss = ss.into_iter().map(|s| {
			let sn = skill.len();
			*skill.entry(s).or_insert(sn)
		}).collect::<Vec<_>>();
		ss.sort();
		devs.push((c, b, ss));
	}
	let m = read!(usize);
	for _ in 0..m {
		let (c, b) = read!(String, i64);
		let cn = com.len();
		let c = *com.entry(c).or_insert(cn);
		mans.push((c, b));
	}
	dbg!(com.len());
	dbg!(skill.len());
	let id = match com.len() {
		5 => "a",
		100 => "b",
		120 => "c",
		350 => "d",
		600 => "e",
		150 => "f",
		_ => unimplemented!()
	};
	Input { id, devs, mans, cs, C: com.len(), S: skill.len() }
}

#[derive(Clone, Debug)]
pub struct Output {
	score: i64,
	devs: Vec<(usize, usize)>,
	mans: Vec<(usize, usize)>,
}

impl Output {
	fn new(input: &Input, devs: Vec<(usize, usize)>, mans: Vec<(usize, usize)>) -> Output {
		let mut score = 0;
		Output { score, devs, mans }
	}
}

fn read_output(input: &Input, f: &str) -> Output {
	unimplemented!()
	// let mut r = std::io::BufReader::new(std::fs::File::open(f).unwrap());
	// let n = read_from!(r; usize);
	// let mut out = vec![];
	// for _ in 0..n {
	// 	let (i, _) = read_from!(r; usize, usize);
	// 	out.push((i, read_from!(r; [usize])));
	// }
	// Output { score: compute_score(input, &out), out }
}

fn write_output<W: Write>(out: &Output, mut w: W) {
	for i in 0..out.devs.len() {
		if out.devs[i].0 == !0 {
			writeln!(w, "X").unwrap();
		} else {
			writeln!(w, "{} {}", out.devs[i].1, out.devs[i].0).unwrap();
		}
	}
	for i in 0..out.mans.len() {
		if out.mans[i].0 == !0 {
			writeln!(w, "X").unwrap();
		} else {
			writeln!(w, "{} {}", out.mans[i].1, out.mans[i].0).unwrap();
		}
	}
}

fn score_skill(s1: &Vec<usize>, s2: &Vec<usize>) -> i64 {
	let mut a = 0;
	let mut b = 0;
	let mut common = 0;
	while a < s1.len() && b < s2.len() {
		if s1[a] == s2[b] {
			common += 1;
			a += 1;
			b += 1;
		} else if s1[a] < s2[b] {
			a += 1;
		} else {
			b += 1;
		}
	}
	let mut tot = s1.len() + s2.len() - common * 2;
	(common * tot) as i64
}

const DXY: [(usize, usize); 4] = [(!0, 0), (1, 0), (0, !0), (0, 1)];

fn mv((x, y): (usize, usize), dir: usize) -> (usize, usize) {
	(x + DXY[dir].0, y + DXY[dir].1)
}

fn compute_gain(input: &Input, who: &Vec<Vec<usize>>, i: usize, j: usize, p: usize) -> i64 {
	let mut gain = 0;
	let (my_c, my_b) = if input.cs[i][j] == '_' {
		(input.devs[p].0, input.devs[p].1)
	} else {
		(input.mans[p].0, input.mans[p].1)
	};
	for dir in 0..4 {
		let (i2, j2) = mv((i, j), dir);
		if i2 < input.cs.len() && j2 < input.cs[0].len() && who[i2][j2] != !0 {
			let q = who[i2][j2];
			let (op_c, op_b) = if input.cs[i2][j2] == '_' {
				(input.devs[q].0, input.devs[q].1)
			} else {
				(input.mans[q].0, input.mans[q].1)
			};
			if my_c == op_c {
				gain += my_b * op_b;
			}
			if input.cs[i][j] == '_' && input.cs[i2][j2] == '_' {
				gain += score_skill(&input.devs[p].2, &input.devs[q].2);
			}
		}
	}
	gain
}

fn greedy(input: &Input, who: &mut Vec<Vec<usize>>, devs: &Vec<usize>, mans: &Vec<usize>, pos: &Vec<(usize, usize)>) -> i64 {
	let mut used_dev = vec![false; devs.len()];
	let mut used_man = vec![false; mans.len()];
	let mut total_gain = 0;
	for p in 0..pos.len() {
		let (i, j) = pos[p];
		if input.cs[i][j] == '_' {
			let mut max_gain = -1;
			let mut max_a = !0;
			for a in 0..devs.len() {
				if used_dev[a] {
					continue;
				}
				if max_gain.setmax(compute_gain(input, who, i, j, devs[a])) {
					max_a = a;
				}
			}
			who[i][j] = devs[max_a];
			used_dev[max_a] = true;
			total_gain += max_gain;
		} else {
			let mut max_gain = -1;
			let mut max_a = !0;
			for a in 0..mans.len() {
				if used_man[a] {
					continue;
				}
				if max_gain.setmax(compute_gain(input, who, i, j, mans[a])) {
					max_a = a;
				}
			}
			who[i][j] = mans[max_a];
			used_man[max_a] = true;
			total_gain += max_gain;
		}
	}
	total_gain
}

fn optimize(input: &Input, who: &mut Vec<Vec<usize>>, pos: &Vec<(usize, usize)>) -> i64 {
	if pos.len() < 2 {
		return 0;
	}
	let until = get_time() + 0.2;
	let mut gain = 0;
	let mut rng = rand::thread_rng();
	while get_time() < until {
		let a = rng.gen_range(0, pos.len());
		let b = rng.gen_range(0, pos.len());
		if a == b {
			continue;
		}
		let (i1, j1) = pos[a];
		let (i2, j2) = pos[b];
		if input.cs[i1][j1] != input.cs[i2][j2] {
			continue;
		}
		let p1 = who[i1][j1];
		let p2 = who[i2][j2];
		who[i1][j1] = !0;
		who[i2][j2] = !0;
		let tmp = - compute_gain(input, who, i1, j1, p1) - compute_gain(input, who, i2, j2, p2)
				  + compute_gain(input, who, i1, j1, p2) + compute_gain(input, who, i2, j2, p1);
		who[i1][j1] = p1;
		who[i2][j2] = p2;
		if tmp >= 0 {
			gain += tmp;
			let w = who[i1][j1];
			who[i1][j1] = who[i2][j2];
			who[i2][j2] = w;
		}
	}
	eprintln!("gain: {}", gain);
	gain
}

fn optimize2(input: &Input, who: &mut Vec<Vec<usize>>, pos: &Vec<(usize, usize)>) -> i64 {
	if pos.len() < 2 {
		return 0;
	}
	let stime = get_time();
	let until = stime + 0.2;
	let mut gain = 0;
	let mut rng = rand::thread_rng();
	while get_time() < until {
		let a = rng.gen_range(0, pos.len());
		let b = rng.gen_range(0, pos.len());
		if a == b {
			continue;
		}
		let (i1, j1) = pos[a];
		let (i2, j2) = pos[b];
		if input.cs[i1][j1] != input.cs[i2][j2] {
			continue;
		}
		let p1 = who[i1][j1];
		let p2 = who[i2][j2];
		who[i1][j1] = !0;
		who[i2][j2] = !0;
		let tmp = - compute_gain(input, who, i1, j1, p1) - compute_gain(input, who, i2, j2, p2)
				  + compute_gain(input, who, i1, j1, p2) + compute_gain(input, who, i2, j2, p1);
		who[i1][j1] = p1;
		who[i2][j2] = p2;
		if tmp >= 0 || { let T = 0.01 + 10.0 * (until - get_time()) / (until - stime); f64::exp(tmp as f64 / T) > rng.gen::<f64>() } {
			gain += tmp;
			let w = who[i1][j1];
			who[i1][j1] = who[i2][j2];
			who[i2][j2] = w;
		}
	}
	eprintln!("gain: {}", gain);
	gain
}

fn solve(input: &Input) -> Output {
	let mut coms = vec![(vec![], vec![]); input.C];
	for i in 0..input.devs.len() {
		coms[input.devs[i].0].0.push(i);
	}
	for i in 0..input.mans.len() {
		coms[input.mans[i].0].1.push(i);
	}
	let (H, W) = (input.cs.len(), input.cs[0].len());
	let mut ps = mat![!0; H; W];
	let mut pos_dev = vec![vec![]; coms.len()];
	let mut pos_man = vec![vec![]; coms.len()];
	let mut pos = vec![vec![]; coms.len()];
	// let mut orders = vec![];
	// let mut checked = mat![false; H; W];
	// for i in 0..H {
	// 	for j in 0..W {
	// 		if checked[i][j] || input.cs[i][j] == '_' {
	// 			continue;
	// 		}
	// 		let mut que = std::collections::VecDeque::new();
	// 		que.push_back((i, j));
			
	// 	}
	// }
	for si in 0..H {
		for sj in 0..W {
			if ps[si][sj] == !0 && input.cs[si][sj] != '#' {
				let mut c = !0;
				if input.cs[si][sj] == '_' {
					for a in 0..coms.len() {
						if pos_dev[a].len() < coms[a].0.len() && (c == !0 || coms[c].0.len() + coms[c].1.len() - pos[c].len() < coms[a].0.len() + coms[a].1.len() - pos[a].len()) {
							c = a;
						}
					}
				} else {
					for a in 0..coms.len() {
						if pos_man[a].len() < coms[a].1.len() && (c == !0 || coms[c].0.len() + coms[c].1.len() - pos[c].len() < coms[a].0.len() + coms[a].1.len() - pos[a].len()) {
							c = a;
						}
					}
				};
				if c == !0 {
					break;
				}
				let mut count = mat![0; H; W];
				let mut hoge = 0i32;
				let mut que = BinaryHeap::new();
				que.push((0, -hoge, si, sj));
				hoge += 1;
				while let Some((_, _, i, j)) = que.pop() {
					if ps[i][j] != !0 {
						continue;
					}
					if input.cs[i][j] == '_' {
						if pos_dev[c].len() >= coms[c].0.len() {
							continue;
						}
						pos_dev[c].push((i, j));
					} else {
						if pos_man[c].len() >= coms[c].1.len() {
							continue;
						}
						pos_man[c].push((i, j));
					}
					pos[c].push((i, j));
					ps[i][j] = c;
					for dir in 0..4 {
						let (i2, j2) = mv((i, j), dir);
						if i2 < H && j2 < W && input.cs[i2][j2] != '#' && ps[i2][j2] == !0 {
							count[i2][j2] += 1;
							hoge += 1;
							que.push((count[i2][j2], -hoge, i2, j2));
						}
					}
				}
			}
		}
	}
	let mut who = mat![!0; H; W];
	let mut score = 0;
	let mut done = vec![false; coms.len()];
	for i in 0..H {
		for j in 0..W {
			if ps[i][j] != !0 {
				let c = ps[i][j];
				if !done[c] {
					done[c] = true;
					score += greedy(input, &mut who, &coms[c].0, &coms[c].1, &pos[c]);
					score += optimize(input, &mut who, &pos[c]);
				}
			}
		}
	}
	let mut piyo = mat![!0; H; W];
	for c in 0..coms.len() {
		let mut tmp = vec![];
		for &(i, j) in &pos[c] {
			if piyo[i][j] != c {
				piyo[i][j] = c;
				tmp.push((i, j));
			}
			for dir in 0..4 {
				let (i, j) = mv((i, j), dir);
				if i < H && j < W && piyo[i][j] != c && input.cs[i][j] != '#' {
					piyo[i][j] = c;
					tmp.push((i, j));
				}
			}
		}
		score += optimize(input, &mut who, &tmp);
	}
	for c in 0..coms.len()-1 {
		let mut tmp = pos[c].clone();
		tmp.extend(pos[c + 1].clone());
		score += optimize(input, &mut who, &tmp);
	}
	let mut devs = vec![(!0, !0); input.devs.len()];
	let mut mans = vec![(!0, !0); input.mans.len()];
	for i in 0..H {
		for j in 0..W {
			if who[i][j] != !0 {
				if input.cs[i][j] == '_' {
					devs[who[i][j]] = (i, j);
				} else {
					mans[who[i][j]] = (i, j);
				}
			}
		}
	}
	Output { score, devs, mans }
}

fn run() {
	let input = read_input();
	let mut best = if let Some(f) = std::env::args().nth(1) {
		read_output(&input, &f)
	} else {
		let out = solve(&input);
		out
	};
	eprintln!("id = {}", input.id);
	eprintln!("Score = {}", best.score);
	// optimize(&input, &mut best);
	// eprintln!("Score = {}", best.score);
	write_output(&best, std::fs::File::create(format!("out/{}.txt", input.id)).unwrap());
}
