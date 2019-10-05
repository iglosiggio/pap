#include <iostream>
#include <vector>

using std::cin;
using std::cout;
using std::endl;
using std::min;
using std::vector;

using Grafo = vector<vector<int>>;

#define INF 9999999

void fw(Grafo& g) {
	int i, j, k, n = g.size();
	for (k = 0; k < n; k++)
	for (i = 0; i < n; i++)
	for (j = 0; j < n; j++)
	if (g[i][k] + g[k][j] < g[i][j])
		g[i][j] = g[i][k] + g[k][j];
}

void resolver() {
	int b, p, q;

	cin >> b >> p;
	Grafo g(p + 1, vector<int>(p + 1, INF));
	vector<int> posibles;

	cin >> q;
	while (q--) {
		int o;
		cin >> o;
		g[0][o] = 0;
		posibles.push_back(o);
	}

	for (int i = 1; i <= p; i++) {
		int q;
		cin >> q;
		while (q--) {
			int c, o;
			cin >> c >> o;
			g[i][o] = min(g[i][o], c);
		}
	}

	int mindist = INF;

	fw(g);
	for (int v : posibles)
		mindist = min(g[v][0], mindist);

	if (mindist >= b)
		cout << 0 << endl;
	else {
		int res = (b - mindist) / (mindist - 1);
		res += (res * (mindist - 1) + mindist) < b;
		cout << res << endl;
	}
}

int main(int argc, char* argv[]) {
	int t;
	cin >> t;
	while (t--)
		resolver();
	return 0;
}
