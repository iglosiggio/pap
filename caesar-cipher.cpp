#include <algorithm>
using std::sort;

#include <iostream>
using std::cin;
using std::cout;
using std::endl;
using std::ostream;

#include <vector>
using std::vector;

#include <map>
using std::map;

using std::string;

vector<ssize_t> kmppre(const string& s) {
	vector<ssize_t> rval(s.size() + 1);
	rval[0] = -1;

	ssize_t i = 1;
	ssize_t len = 0;

	while (i < s.size()) {
		if (s[i] == s[len])
			rval[i] = rval[len];
		else {
			rval[i] = len;
			while (len >= 0 && s[i] != s[len])
				len = rval[len];
		}
		i++;
		len++;
	}

	rval[i] = len;

	return rval;
}

size_t kmpsearch(string p, string s) {
	vector<ssize_t> t = kmppre(p);
	size_t count = 0;

	ssize_t p_i = 0;
	size_t s_i = 0;

	while (s_i < s.size()) {
		if (p[p_i] == s[s_i]) {
			p_i++, s_i++;

			if (p_i == p.size()) {
				count++;
				p_i = t[p_i];
			}
		} else {
			p_i = t[p_i];

			if (p_i < 0)
				p_i++, s_i++;
		}
	}

	return count;
}

void rot(string& s, const map<char, char> subst) {
	for (auto& c : s)
		c = subst.at(c);
}

template<class T>
ostream& operator<<(ostream& o, vector<T> v) {
	if (v.size())
		o << v[0];
	for (size_t i = 1; i < v.size(); i++)
		o << ' ' << v[i];
	return o;
}

void resolver() {
	string a;
	string w;
	string s;

	cin >> a >> w >> s;

	map<char, char> subst;
	for (int i = 0; i < a.size(); i++)
		subst[a[i]] = a[(i + 1) % a.size()];

	vector<int> solutions;
	for (int i = 0; i < a.size(); i++, rot(w, subst)) {
		if (kmpsearch(w, s) == 1)
			solutions.push_back(i);
	}

	std::sort(solutions.begin(), solutions.end());

	if (solutions.size() == 0)
		cout << "no solution" << endl;
	else if (solutions.size() == 1)
		cout << "unique: " << solutions[0] << endl;
	else cout << "ambiguous: " << solutions << endl;
}

int main(int argc, char* argv[]) {
	int t;
	cin >> t;
	while (t--)
		resolver();
	return 0;
}
