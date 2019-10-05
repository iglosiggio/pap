#include <iostream>
#include <map>
#include <vector>
#include <queue>

using std::cin;
using std::cout;
using std::endl;
using std::map;
using std::min;
using std::pair;
using std::priority_queue;
using std::vector;

using Grafo = vector<vector<pair<int, int>>>;

#define INF 9999999

vector<int> dijkstra(const Grafo& g, int desde) {
	vector<int> dist(g.size(), INF);
	priority_queue<pair<int, int>> pq;

	dist[desde] = 0;

	for (const auto& vert : g[desde]) {
		dist[vert.first] = vert.second;
		pq.push(vert);
	}

	while (not pq.empty()) {
		auto vert = pq.top();
		int dist_aca = dist[vert.first];
		pq.pop();

		for (const auto& v : g[vert.first]) 
		if (dist_aca + v.second < dist[v.first]) {
			dist[v.first] = dist_aca + v.second;
			pq.emplace(v.first, dist[v.first]);
		}
	}

	return dist;
}

void resolver() {
	int b, p, q;

	cin >> b >> p;
	Grafo g(p + 1);

	cin >> q;
	while (q--) {
		int o;
		cin >> o;
		g[0].emplace_back(o, 0);
	}

	for (int i = 1; i <= p; i++) {
		int q;
		cin >> q;
		while (q--) {
			int c, o;
			cin >> c >> o;
			g[i].emplace_back(o, c);
		}
	}

	int mindist = INF;

	for (int i = 1; i <= p; i++)
		mindist = min(dijkstra(g, i)[0], mindist);

	cout << b / (mindist - 1) << endl;
}

int main(int argc, char* argv[]) {
	int t;
	cin >> t;
	while (t--)
		resolver();
	return 0;
}
