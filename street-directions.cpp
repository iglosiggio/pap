#include <iostream>
#include <map>
#include <vector>
#include <set>

using std::cin;
using std::cout;
using std::endl;
using std::map;
using std::vector;
using std::set;
using std::pair;
using std::min;
using G = map<int, vector<int>>;

struct Data {
	vector<int> depth;
	vector<int> low;
	set<pair<int, int>> ejes;

	Data(int n) : depth(n + 1, -1), low(n + 1) {}
};

int dfs(const G& g, int v, int d, int padre, Data& data) {
	set<pair<int, int>>& ejes = data.ejes;
	int& depth = data.depth[v];
	int& low = data.low[v];
	depth = d;
	low = d;

	for (auto w : g.at(v))
	if (w != padre) {
		int& w_depth = data.depth[w];
		int& w_low = data.low[w];

		if (w_depth == -1) {
			low = min(low, dfs(g, w, d + 1, v, data));
			ejes.emplace(v, w);
			if (w_low >= w_depth)
				ejes.emplace(w, v);
		} else {
			low = min(low, w_depth);
			if (ejes.find(pair<int, int>{w, v}) == ejes.end())
				ejes.emplace(v, w);
		}

	}

	return low;
}

void resolver(int n, int m) {
	G g;
	vector<pair<int, int>> e;

	for (int i = 0; i < m; i++) {
		pair<int, int> eje;
		cin >> eje.first >> eje.second;

		e.push_back(eje);
		g[eje.first].push_back(eje.second);
		g[eje.second].push_back(eje.first);
	}

	Data data(n);
	dfs(g, 1, 0, 1, data);

	for (const auto& eje : data.ejes)
		cout << eje.first << ' ' << eje.second << endl;
}

int main(int argc, char* argv[]) {
	int i = 0, n, m;

	while (i++, cin >> n >> m, n || m) {
		cout << i << endl << endl;
		resolver(n, m);
		cout << '#' << endl;
	}

	return 0;
}
