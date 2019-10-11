#include <algorithm>
#include <iostream>
#include <vector>
#include <set>

using std::fill;
using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::set;

void toposort(const vector<vector<int>>& grafo, vector<bool>& visitado,
	vector<int>& result, int v) {

	if (visitado[v])
		return;
	visitado[v] = true;
	for (auto v2 : grafo[v])
		toposort(grafo, visitado, result, v2);
	result.push_back(v);
}

void pintar(const vector<vector<int>>& grafo, vector<bool>& visitado, int v) {
	if (visitado[v])
		return;
	visitado[v] = true;
	for (auto v2 : grafo[v])
		pintar(grafo, visitado, v2);
}

void resolver() {
	int n, m;
	cin >> n >> m;
	
	vector<vector<int>> grafo(n + 1);
	vector<bool> visitado(n + 1);
	vector<int> sorted;
	sorted.reserve(n);

	while (m--) {
		int a, b;
		cin >> a >> b;
		grafo[a].push_back(b);
	}

	for (int i = 1; i <= n; i++)
		toposort(grafo, visitado, sorted, i);


	int rta = 0;

	fill(visitado.begin(), visitado.end(), false);
	for (auto i = sorted.rbegin(); i != sorted.rend(); i++)
	if (not visitado[*i]) {
		rta++;
		pintar(grafo, visitado, *i);
	}

	cout << rta << endl;
}

int main(int argc, char* argv[]) {
	int t;
	cin >> t;

	while (t--)
		resolver();
	return 0;
}
