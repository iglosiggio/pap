#include <iostream>
#include <algorithm>
#include <vector>

using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::pair;
using std::move;
using std::min;
using std::max;
using std::sort;
using punto = pair<int, int>;

enum tile { VACIO, LLENO, VISITADO };

struct componente {
	vector<punto> puntos;
	int minx = 101;
	int maxx = -1;
	int miny = 101;
	int maxy = -1;

	bool operator<(const componente& b) {
		if (puntos.size() < b.puntos.size())
			return true;
		if (puntos.size() > b.puntos.size())
			return false;
		for (int i = 0; i < puntos.size(); i++) {
			if (puntos[i] < b.puntos[i])
				return true;
			if (puntos[i] > b.puntos[i])
				return false;
		}
		return false;
	}

	bool operator!=(const componente& b) {
		if (puntos.size() != b.puntos.size())
			return true;
		for (int i = 0; i < puntos.size(); i++) {
			if (puntos[i] != b.puntos[i])
				return true;
		}
		return false;
	}
};

void normalizar(componente& c) {
	for (auto& p : c.puntos) {
		p.first -= c.minx;
		p.second -= c.miny;
	}

	c.maxx -= c.minx;
	c.maxy -= c.miny;
	c.minx = 0;
	c.miny = 0;

	sort(c.puntos.begin(), c.puntos.end());
}

componente rot90(componente c) {
	for (auto& p : c.puntos) {
		int x = p.first;
		int y = p.second;
		p.first = -y;
		p.second = x;
	}

	int minx = -c.maxy;
	int maxx = -c.miny;
	c.miny = c.minx;
	c.maxy = c.maxx;
	c.minx = minx;
	c.maxx = maxx;

	normalizar(c);
	return c;
}

componente flipX(componente c) {
	for (auto& p : c.puntos)
		p.first *= -1;

	int minx = -c.maxx;
	c.maxx = -c.minx;
	c.minx = minx;

	normalizar(c);
	return c;
}

void normalizar(vector<componente>& componentes) {
	for (auto& c : componentes)
		normalizar(c);
}

void dfs(vector<vector<tile>>& mapa, int x, int y,
	componente& componente) {

	if (mapa[x][y] == VACIO)
		return;
	if (mapa[x][y] == VISITADO)
		return;

	mapa[x][y] = VISITADO;

	componente.minx = min(componente.minx, x);
	componente.maxx = max(componente.maxx, x);

	componente.miny = min(componente.miny, y);
	componente.maxy = max(componente.maxy, y);

#define IN_BOUNDS(x, y) \
	((x) >= 0 && (x) < mapa.size() && (y) >= 0 && (y) < mapa[0].size())

	if (IN_BOUNDS(x - 1, y))
		dfs(mapa, x - 1, y, componente);

	if (IN_BOUNDS(x + 1, y))
		dfs(mapa, x + 1, y, componente);

	if (IN_BOUNDS(x, y - 1))
		dfs(mapa, x, y - 1, componente);

	if (IN_BOUNDS(x, y + 1))
		dfs(mapa, x, y + 1, componente);

#undef IN_BOUNDS

	componente.puntos.push_back({x, y});
}

void dibujar(const vector<vector<tile>>& mapa, int w, int h) {
	for (int j = 0; j < h; j++, cout << endl)
	for (int i = 0; i < w; i++)
		cout << (mapa[i][j] == VACIO ? ' ' : '*');
}

void dibujar(const componente& c) {
	vector<vector<char>> mapa(c.maxy + 1, vector<char>(c.maxx + 1, ' '));

	for (auto p : c.puntos)
		mapa[p.second][p.first] = '*';

	for (const auto& linea : mapa) {
		for (char c : linea)
			cout << c;
		cout << endl;
	}
}

void imprimirComponentes(const vector<componente>& componentes) {
	for (const auto& componente : componentes) {
		cout << "--- COMPONENTE ---" << endl;
		cout << "min x = " << componente.minx << endl;
		cout << "max x = " << componente.maxx << endl;
		cout << "min y = " << componente.miny << endl;
		cout << "max y = " << componente.maxy << endl;
		for (auto p : componente.puntos)
			cout << p.first << ' ' << p.second << endl;
		dibujar(componente);
	}
}

void resolver() {
	int w, h, n;
	cin >> w >> h >> n;

	vector<vector<tile>> mapa1(w, vector<tile>(h, VACIO));
	vector<vector<tile>> mapa2(w, vector<tile>(h, VACIO));
	vector<punto> puntos1;
	vector<punto> puntos2;
	vector<componente> componentes1;
	vector<componente> componentes2;

	int i = n;
	while (i--) {
		int x, y;
		cin >> x >> y;
		mapa1[x][y] = LLENO;
		puntos1.push_back({x, y});
	}

	i = n;
	while (i--) {
		int x, y;
		cin >> x >> y;
		mapa2[x][y] = LLENO;
		puntos2.push_back({x, y});
	}

	for (auto p : puntos1)
	if (mapa1[p.first][p.second] == LLENO) {
		componente c;
		dfs(mapa1, p.first, p.second, c);
		normalizar(c);
		componentes1.emplace_back(move(rot90(c)));
		componentes1.emplace_back(move(rot90(rot90(c))));
		componentes1.emplace_back(move(rot90(rot90(rot90(c)))));
		componentes1.emplace_back(move(flipX(c)));
		componentes1.emplace_back(move(flipX(rot90(c))));
		componentes1.emplace_back(move(flipX(rot90(rot90(c)))));
		componentes1.emplace_back(move(flipX(rot90(rot90(rot90(c))))));
		componentes1.emplace_back(move(c));
	}

	for (auto p : puntos2)
	if (mapa2[p.first][p.second] == LLENO) {
		componente c;
		dfs(mapa2, p.first, p.second, c);
		normalizar(c);
		componentes2.emplace_back(move(rot90(c)));
		componentes2.emplace_back(move(rot90(rot90(c))));
		componentes2.emplace_back(move(rot90(rot90(rot90(c)))));
		componentes2.emplace_back(move(flipX(c)));
		componentes2.emplace_back(move(flipX(rot90(c))));
		componentes2.emplace_back(move(flipX(rot90(rot90(c)))));
		componentes2.emplace_back(move(flipX(rot90(rot90(rot90(c))))));
		componentes2.emplace_back(move(c));
	}

	sort(componentes1.begin(), componentes1.end());
	sort(componentes2.begin(), componentes2.end());

	if (componentes1.size() != componentes2.size()) {
		cout << "NO" << endl;
		return;
	}

	for (int i = 0; i < componentes1.size(); i++)
		if (componentes1[i] != componentes2[i]) {
			cout << "NO" << endl;
			return;
		}

	cout << "YES" << endl;
}

int main(int argc, char* argv[]) {
	int t;
	cin >> t;

	while (t--)
		resolver();

	return 0;
}
