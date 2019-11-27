/* Basado en la implemenfacion de suffix rank y lcp de Francisco Soulignac */

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <set>
#include <map>

using namespace std;

struct entry {
	int rank[2]; // ranking de la primera y segunda mitad del prefijo
	int pos;     // posición del string que representa esta entrada
};

void rsort(vector<entry>& join, int r, int M) {
	vector<vector<entry>> buckets(M);
	for_each(join.crbegin(), join.crend(), [&](const entry& e) {
		buckets[e.rank[r]].push_back(e);
	});

	join.clear();
	for (const auto& bucket : buckets)
		join.insert(join.end(), bucket.crbegin(), bucket.crend());
}

template<class T>
vector<int> suffixrank(const T& s) {
	vector<int> rank(s.begin(), s.end());

	vector<entry> join(s.size());
	int M = 256;
	for (int k = 1; k < rank.size(); k *= 2) {
		for (int i = 0; i < rank.size(); ++i)
			join[i] = entry{
				{ rank[i], i+k < rank.size() ? rank[i+k] : 0 },
				i
			};

		rsort(join, 1, M);
		rsort(join, 0, M);
		// recargamos el rank
		M = rank[join[0].pos] = 0;
		for (int i = 1; i < rank.size(); ++i)
			rank[join[i].pos] =
				join[i].rank[0] == join[i-1].rank[0]
				and join[i].rank[1] == join[i-1].rank[1]
				? M : ++M;

		if (++M == rank.size())
			break;
	}

	return rank;
}


vector<int> suffixarray(const vector<int>& rank) {
	vector<int> sa(rank.size());
	for (int i = 0; i < rank.size(); ++i)
		sa[rank[i]] = i;

	return sa;
}

template<class T>
vector<int> LCP(const T& s, const vector<int>& sa,
		const vector<int>& rank) {
	vector<int> lcp(s.size(), 0);
	int len = 0;

	for (int i = 0; i < s.size()-1; ++i) {
		int k = rank[i], j = sa[k-1];
		// j es el predecesor de i en el orden lexicografico.
		while (s[i+len] == s[j+len])
			len++;
		lcp[k] = len;
		if (len > 0)
			len--;
	}

	return lcp;
}

/* RMQ */
#define K 20
#define MAXN 110000
int min_st[K + 1][MAXN];

template<class T>
T log2(T v) {
	T r = 0;
	while (v /= 2)
		r++;
	return r;
}

template<class It>
void rmq_init(It begin, It end) {
	size_t i = 0;

	while (begin != end) {
		min_st[0][i] = *begin;
		begin++, i++;
	}

	size_t total = i;
	for (size_t i = 1; i <= K; i++)
	for (size_t j = 0; j + (1 << i) <= total; j++) {
		min_st[i][j] = min(min_st[i - 1][j],
		                   min_st[i - 1][j + (1 << (i - 1))]);
	}
}

int rmq(size_t l, size_t r) {
	size_t k = log2(r - l + 1);
	return min(min_st[k][l], min_st[k][r - (1 << k) + 1]);
}

void resolver(int n) {
	set<pair<size_t, string>> strings;
	vector<int> concatenado;
	vector<int> lcp;
	vector<int> suffix_array;
	vector<int> suffix_rank;

	/* Caso trivial */
	if (n == 1) {
		string s;
		cin >> s;
		cout << s << endl;
		return;
	}

	size_t concatenado_size = 0;
	for (int i = 0; i < n; i++) {
		string tmp;
		cin >> tmp;
		size_t tmp_size = tmp.size();
		strings.insert({concatenado_size, move(tmp)});
		concatenado_size += tmp_size + 1;
	}

	/* Construyo concatenado agregando los marcadores de fin de cada
	 * cadena */
	{
		concatenado.resize(concatenado_size);
		auto concat_it = concatenado.begin();
		size_t i = 0;
		for (const auto& s : strings) {
			transform(s.second.begin(), s.second.end(), concat_it,
				  [&](char c) { return c + n; });
			concat_it += s.second.size();
			*(concat_it++) = i++;
		}
	}

	suffix_rank = suffixrank(concatenado);
	suffix_array = suffixarray(suffix_rank);
	lcp = LCP(concatenado, suffix_array, suffix_rank);

	/* Print strings */
	//for (const auto& v : strings)
	//	cout << v.first << '\t' << v.first + v.second.size()
	//	     << '\t' << v.second << endl;
	//cout << endl;

	/* Print suffix array */
	//for (int i : suffix_array) {
	//	const auto& p = *--strings.upper_bound({i, "~"});
	//	if (i - p.first == p.second.size())
	//		cout << "\"\"" << endl;
	//	else
	//		cout << p.second.substr(i - p.first) << endl;
	//}
	//cout << endl;

	/* Print LCP */
	//auto get_suffix = [&](size_t i) -> string {
	//	int sa = suffix_array[i];
	//	auto& p = *--strings.upper_bound({sa, "~"});
	//	return p.second.substr(sa - p.first);
	//};
	//
	//for (size_t i = n + 1; i < concatenado.size(); i++) {
	//	cout << i << '\t'
	//	     << get_suffix(i-1) << '\t'
	//	     << get_suffix(i) << '\t' << '"';
	//	for (size_t j = suffix_array[i]; j < suffix_array[i] + lcp[i]; j++)
	//		cout << (char) (concatenado[j] - n);
	//	cout << '"' << endl;
	//}

	size_t half = n / 2;
	size_t maxlen = 0;
	set<string> output;
	rmq_init(lcp.begin(), lcp.end());

	/* Dado un índice en concatenado da un iterador a la cadena
	 * correspondiente en strings */
	auto find_str = [&](size_t i) {
		return --strings.upper_bound({suffix_array[i], "~"});
	};

	/* Comienzo del string es concatenando -> cantidad de apariciones */
	map<size_t, size_t> strings_in_range;
	size_t i = n;
	size_t j = n;
	strings_in_range[find_str(i)->first] = 1;
	while (i < concatenado.size() - 1) {
		/* Avanzo j hasta que cubro más de la mitad de las cadenas */
		while (strings_in_range.size() <= half
		       && j < concatenado.size() - 1) {
			j++;
			strings_in_range[find_str(j)->first]++;
		}
		/* Si encontré una solución la agrego */
		if (rmq(i+1, j) >= maxlen && strings_in_range.size() > half) {
			if (rmq(i+1, j) > maxlen) {
				maxlen = rmq(i+1, j);
				output.clear();
			}
			auto prefix_start = suffix_array[i];
			auto s = find_str(i);
			size_t substr_start = prefix_start - s->first;
			string substr = s->second.substr(substr_start, maxlen);
			output.insert(substr);
		}
		/* Avanzo i un paso y elimino una cadena de ser necesario */
		auto it = strings_in_range.find(find_str(i)->first);
		it->second--;
		if (it->second == 0)
			strings_in_range.erase(it);
		i++;
	}

	if (maxlen == 0)
		cout << '?' << endl;
	else
		for (const auto& s : output)
			cout << s << endl;
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(0);

	int n;

	cin >> n;
	if (n) {
		resolver(n);

		while (cin >> n, n) {
			cout << endl;
			resolver(n);
		}
	}

	return 0;
}
