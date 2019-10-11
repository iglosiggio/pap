#include <algorithm>
using std::sort;

#include <iostream>
using std::cin;
using std::cout;
using std::endl;

#include <tuple>
using std::tuple;
using std::get;

#include <vector>
using std::vector;

#include <functional>
using std::greater;

#include <cmath>
using std::max;
using std::sqrt;

using num = long long;

bool leer_punto(tuple<num, num>& punto) {
	num x, y;
	cin >> x;
	if (x == -1)
		return false;
	cin >> y;
	punto = {x, y};
	return true;
}

num dist(tuple<num, num> a, tuple<num, num> b) {
	return (get<0>(a) - get<0>(b)) * (get<0>(a) - get<0>(b))
		+ (get<1>(a) - get<1>(b)) * (get<1>(a) - get<1>(b));
}

int ds_find(vector<int>& ds, int a) {
	if (ds[a] == -1)
		return a;
	return ds[a] = ds_find(ds, ds[a]);
}

bool ds_union(vector<int>& ds, int a, int b) {
	int cA = ds_find(ds, a);
	int cB = ds_find(ds, b);

	if (cA != cB) {
		ds[cA] = cB;
		return true;
	}
	return false;
}

void kruskal(vector<tuple<num, int, int>>& ejes, int n, int target) {
	vector<int> ds(n, -1);
	int componentes = n;
	num max_eje = 0;

	while (componentes > target) {
		tuple<num, int, int> eje = ejes.back();
		ejes.pop_back();

		if (ds_union(ds, get<1>(eje), get<2>(eje))) {
			max_eje = get<0>(eje);
			componentes--;
		}
	}
	cout << ceil(sqrt(max_eje)) << endl;
}

void resolver() {
	vector<tuple<num, num>> puntos;
	vector<tuple<num, int, int>> ejes;
	tuple<num, num> punto;
	int transmisores;

	cin >> transmisores;

	while (leer_punto(punto))
		puntos.push_back(punto);

	for (int i = 0; i < puntos.size(); i++)
	for (int j = i + 1; j < puntos.size(); j++)
		ejes.emplace_back(dist(puntos[i], puntos[j]), i, j);

	sort(ejes.begin(), ejes.end(), greater<tuple<num, int, int>>());

	kruskal(ejes, puntos.size(), transmisores);
}

int main(int argc, char* argv[]) {
	int t;
	cin >> t;
	while (t--)
		resolver();
	return 0;
}
