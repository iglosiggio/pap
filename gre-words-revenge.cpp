#include <array>
using std::array;

#include <cassert>

#include <cmath>
using std::ceil;
using std::max;
using std::sqrt;

#include <iostream>
using std::cin;
using std::cout;
using std::endl;

#include <queue>
using std::queue;

#include <set>
using std::set;

#include <string>
using std::string;

#include <vector>
using std::vector;

#define ALPHABET_SIZE 2
#define TONUM(c) ((c) == '1')

/* TODO: Define a more generic output function */
struct node {
	size_t matches;
	size_t fail;
	array<ssize_t, ALPHABET_SIZE> gotoidx;
};

using aho_corasick = vector<node>;

node get_node() {
	node n{0};
	for (auto& idx : n.gotoidx)
		idx = -1;
	return n;
}

size_t match(const aho_corasick& aho, const string& s) {
	size_t state = 0;
	size_t matches = aho[state].matches;

	for (auto c : s) {
		while (aho[state].gotoidx[TONUM(c)] < 0)
			state = aho[state].fail;
		state = aho[state].gotoidx[TONUM(c)];
		matches += aho[state].matches;
	}

	return matches;
}

void add_to_trie(aho_corasick& trie, const string& s) {
	size_t nodeidx = 0;

	for (auto c : s) {
		ssize_t& new_node = trie[nodeidx].gotoidx[TONUM(c)];
		if (new_node == -1) {
			new_node = trie.size();
			nodeidx = new_node;
			trie.push_back(get_node());
		} else
			nodeidx = new_node;
	}

	trie[nodeidx].matches = 1;
}

template<class T>
aho_corasick build_trie(const T& ss) {
	aho_corasick trie;
	trie.push_back(get_node());

	for (const auto& s : ss)
		add_to_trie(trie, s);

	for (auto& state : trie[0].gotoidx)
	if (state == -1)
		state = 0;

	return trie;
}

void compute_fail(aho_corasick& aho) {
	queue<size_t> queue;

	for (auto state : aho[0].gotoidx)
	if (state) {
		queue.push(state);
		aho[state].fail = 0;
	}

	while (not queue.empty()) {
		size_t state = queue.front();
		queue.pop();

		for (size_t i = 0; i < aho[state].gotoidx.size(); i++)
		if (aho[state].gotoidx[i] >= 0) {
			size_t next_state = aho[state].gotoidx[i];
			queue.push(next_state);

			size_t fail_state = aho[state].fail;
			while (aho[fail_state].gotoidx[i] == -1)
				fail_state = aho[fail_state].fail;
			fail_state = aho[fail_state].gotoidx[i];

			aho[next_state].fail = fail_state;
			aho[next_state].matches += aho[fail_state].matches;
		}
	}
}

template<class T>
aho_corasick build_aho(const T& ss) {
	aho_corasick trie = build_trie(ss);
	compute_fail(trie);
	return trie;
}

string shift(const string& s, size_t num_chars) {
	num_chars %= s.size();
	return s.substr(num_chars) + s.substr(0, num_chars); 
}

void resolver() {
	int n;
	cin >> n;

	string query;
	set<string> all_strings;
	set<string> some_strings;
	aho_corasick big_aho = build_aho(all_strings);
	aho_corasick small_aho = build_aho(some_strings);
	const size_t SMALL_AHO_TRESHOLD = max(100, (int)ceil(sqrt(n)));
	bool big_dirty = false;
	bool small_dirty = false;
	size_t last_result = 0;

	while (n--) {
		cin >> query;
		string to_query = shift(query.substr(1), last_result);

		if (query[0] == '+') {
			bool es_nuevo = all_strings.insert(to_query).second;
			if (not es_nuevo || big_dirty)
				continue;
			if (some_strings.size() < SMALL_AHO_TRESHOLD) {
				some_strings.insert(to_query);
				small_dirty = true;
			} else {
				some_strings.clear();
				big_dirty = true;
				small_dirty = true;
			}
		} else if (query[0] == '?') {
			if (big_dirty) {
				big_aho = build_aho(all_strings);
				big_dirty = false;
			}
			if (small_dirty) {
				small_aho = build_aho(some_strings);
				small_dirty = false;
			}

			last_result = match(small_aho, to_query)
			            + match(big_aho, to_query);
			cout << last_result << endl;
		} else
			assert(not "Query no reconocida");
	}
}

int main() {
	int t;
	cin >> t;

	for (int i = 1; i <= t; i++) {
		cout << "Case #" << i << ":" << endl;
		resolver();
	}
}
