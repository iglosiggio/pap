#include <vector>
#include <functional>
#include <cassert>

#include <iostream>

enum Spell { NONE, INVERT, BUCCANEERIZE, BARBARIZE };

using DATA = int;
using LAZYDATA = Spell;
const DATA NEUT = 0;
const LAZYDATA LAZYNEUT = NONE;
const auto OP = std::plus<DATA>();
const auto APPLY = [](const DATA& a, const LAZYDATA& spell, int len) -> DATA {
	if (spell == NONE) return a;
	if (spell == INVERT) return len - a;
	if (spell == BUCCANEERIZE) return len;
	if (spell == BARBARIZE) return 0;
	assert(not "Unknown spell");
};
const auto COMBINE = [](const LAZYDATA& a, const LAZYDATA& b) -> LAZYDATA {
	if (a == NONE) return b;
	if (b == NONE) return a;
	if (b == BUCCANEERIZE || b == BARBARIZE) return b;
	// b must be INVERT
	assert(b == INVERT);
	if (a == BUCCANEERIZE) return BARBARIZE;
	if (a == BARBARIZE) return BUCCANEERIZE;
	if (a == INVERT) return NONE;
	assert(not "Unknown spell combination");
};

struct LazyST {
	std::vector<DATA> st;
	std::vector<LAZYDATA> lazy;

	// internal functions
	inline int parent(int i) const { return i / 2; }
	inline int lchild(int i) const { return 2 * i; }
	inline int rchild(int i) const { return 2 * i + 1; }
	inline int leaf_offset() const { return st.size() / 2; }
	int next_pow_2(int i) const {
		int r = 1;
		while (r < i)
			r *= 2;
		return r;
	}

	void apply(int idx, int len) {
		if (lazy[idx] == LAZYNEUT) return;
		st[idx] = APPLY(st[idx], lazy[idx], len);

		if (len > 1) {
			lazy[lchild(idx)] =
				COMBINE(lazy[lchild(idx)], lazy[idx]);
			lazy[rchild(idx)] =
				COMBINE(lazy[rchild(idx)], lazy[idx]);
		}

		lazy[idx] = LAZYNEUT;
	}

	DATA query(int l, int r, int idx, int i, int j) {
		// return neutral value if disjoint
		if (r <= i || l >= j) return NEUT;
		// lower the updates one level
		apply(idx, j - i);
		// return node if fully contained in range
		if (l <= i && j <= r) return st[idx];
		// combine children if partially contained
		int m = (i + j) / 2;
		return OP(query(l, r, lchild(idx), i, m),
		          query(l, r, rchild(idx), m, j));
	}

	// API functions
	// query operation - apply query from top node
	DATA query(int l, int r) {
		return query(l, r, 1, 0, st.size() / 2);
	}

	void set(int l, int r, int idx, int i, int j, const LAZYDATA& v) {
		apply(idx, j - i);
		if (r <= i || l >= j) return;
		if (l <= i && j <= r) {
			lazy[idx] = COMBINE(lazy[idx], v);
			apply(idx, j - i);
			return;
		}
		int m = (i + j) / 2;
		set(l, r, lchild(idx), i, m, v);
		set(l, r, rchild(idx), m, j, v);
		st[idx] = OP(st[lchild(idx)], st[rchild(idx)]);
	}

	void set(int l, int r, const LAZYDATA& v) {
		set(l, r, 1, 0, st.size() / 2, v);
	}

	// segment tree factory
	template<class Iter>
	LazyST& init(Iter begin, Iter end) {
		size_t size = next_pow_2(std::distance(begin, end));
		// allocate space for tree
		st.assign(2 * size, NEUT);
		lazy.assign(2 * size, LAZYNEUT);
		// load values into tree
		std::copy(begin, end, st.begin() + size);
		// compute upper levels
		while (size = parent(size))
			for (size_t i = size; i < 2 * size; i++)
				st[i] = OP(st[lchild(i)], st[rchild(i)]);

		return *this;
	}
};

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;

void resolver() {
	vector<int> s;

	int m;
	cin >> m;
	while (m--) {
		int t;
		string tmp;
		cin >> t;
		cin >> tmp;

		while (t--)
		for (auto c : tmp)
			s.push_back(c == '1');
	}

	LazyST st;
	st.init(s.begin(), s.end());

	int q = 1;
	int qs;
	cin >> qs;
	for (int i = 1; i <= qs; i++) {
		char c;
		int a, b;
		cin >> c >> a >> b;
		if (c == 'F') st.set(a, b + 1, BUCCANEERIZE);
		if (c == 'E') st.set(a, b + 1, BARBARIZE);
		if (c == 'I') st.set(a, b + 1, INVERT);
		if (c == 'S') {
			cout << "Q" << q << ": " << st.query(a, b + 1) << endl;
			q++;
		}
	}
}

int main() {
	int t;
	cin >> t;
	for (int i = 1; i <= t; i++) {
		cout << "Case " << i << ":" << endl;
		resolver();
	}
	return 0;
}
