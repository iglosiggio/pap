#include <iostream>
#include <map>
#include <vector>
#include <stack>

using std::cin;
using std::cout;
using std::endl;
using std::map;
using std::vector;
using std::stack;
using std::pair;
using std::min;
using G = map<int, vector<int>>;

struct Data {
	stack<pair<int, int>> s;
	vector<int> depth;
	vector<int> low;
	vector<int> articulaciones;
	vector<pair<int, int>> puentes;
	vector<vector<pair<int, int>>> componentes;

	Data(int n) : depth(n + 1, -1), low(n + 1) {}
};

void reportarComponente(int v, int w, Data& data) {
	stack<pair<int, int>>& stack = data.s;
		
	data.componentes.emplace_back();
	vector<pair<int, int>>& componente = data.componentes.back();

	componente.emplace_back(stack.top());

	while (stack.top() != pair<int, int>{v, w}) {
		stack.pop();
		componente.emplace_back(stack.top());
	}
	stack.pop();
}

int dfs(const G& g, int v, int d, int padre, Data& data) {
	int& depth = data.depth[v];
	int& low = data.low[v];
	depth = d;
	low = d;

	if (v != padre)
		data.s.emplace(padre, v);

	for (auto w : g.at(v))
	if (w != padre) {
		int& w_depth = data.depth[w];
		int& w_low = data.low[w];

		if (w_depth == -1) {
			low = min(low, dfs(g, w, d + 1, v, data));
			if (w_low >= depth) {
				data.articulaciones.push_back(v);
				reportarComponente(v, w, data);
			}
			if (w_low >= w_depth)
				data.puentes.emplace_back(v, w);
		} else
			low = min(low, w_depth);
	}

	return low;
}

Data biconexas(const G& g) {
	Data data(g.size());

	dfs(g, 1, 0, 1, data);

	return data;
}

void resolver() {
	int n, m;
	G g;
	vector<pair<int, int>> e;

	cin >> n >> m;

	for (int i = 0; i < m; i++) {
		pair<int, int> eje;
		cin >> eje.first >> eje.second;

		e.push_back(eje);
		g[eje.first].push_back(eje.second);
		g[eje.second].push_back(eje.first);
	}

	/* La entrada termina con un 0 0 */
	cin >> n >> m;

	Data d = biconexas(g);

	cout << "Componentes!" << endl;
	cout << "==================" << endl;
	int cid = 0;
	for (const auto& componente : d.componentes) {
		cout << "Componente " << cid << endl;
		cout << "==================" << endl;
		for (const auto& eje : componente)
			cout << eje.first << ' ' << eje.second << endl;
		cid++;
	}
	cout << "Puentes!" << endl;
	cout << "==================" << endl;
	for (const auto& puente : d.puentes) {
		cout << puente.first << ' ' << puente.second << endl;
	}
	cout << "Articulaciones!" << endl;
	cout << "==================" << endl;
	for (const auto& art : d.articulaciones) {
		cout << art << endl;
	}
}

int main(int argc, char* argv[]) {
	int t;
	cin >> t;
	for (int i = 1; i <= t; i++) {
		cout << i << endl;
		resolver();
		cout << '#' << endl;
	}
	return 0;
}
