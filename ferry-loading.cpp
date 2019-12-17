#include <vector>
#include <iostream>

using std::vector;
using std::cout;
using std::cin;
using std::endl;

inline int max(int a, int b) {
	return a > b ? a : b;
}

inline int min(int a, int b) {
	return a < b ? a : b;
}

/* La dp es (i, max(izq, der)) no es necesario saber la longitud del menor dado
 * que es sum(cola[0..i]) - max(izq, der). */
int dp[222][22222];

int resolver(size_t i, int izq, int der, const vector<int>& cola, int l) {
	if (cola.size() == i)
		return 0;

	int& res = dp[i][max(izq, der)];

	if (res >= 0)
		return res;
	else
		res = 0;

	if (cola[i] + izq <= l)
		res = max(res, 1 + resolver(i + 1, cola[i] + izq, der, cola, l));

	if (cola[i] + der <= l)
		res = max(res, 1 + resolver(i + 1, izq, cola[i] + der, cola, l));

	return res;
}

void resolver() {
	int l, n;
	vector<int> cola;

	cin >> l;
	l *= 100;

	while (cin >> n, n)
		cola.push_back(n);

	for (size_t i = 0; i < min(cola.size(), 222); i++)
	for (int j = 0; j <= l; j++)
		dp[i][j] = -1;

	int respuesta = resolver(0, 0, 0, cola, l);

	cout << respuesta << endl;

	/* Reconstruyo la respuesta desde la dp */
	int izq = 0;
	int der = 0;
	bool es_port = false;
	for (int i = 0; i < respuesta; i++) {
		if (cola[i] + izq > l)
			es_port = false;
		else if (cola[i] + der > l)
			es_port = true;
		else
			es_port = dp[i + 1][max(izq + cola[i], der)]
				>= dp[i + 1][max(izq, der + cola[i])];

		if (es_port) {
			izq += cola[i];
			cout << "port" << endl;
		} else {
			der += cola[i];
			cout << "starboard" << endl;
		}
	}
}

int main(int argc, char* argv[]) {
	int n;

	cin >> n;

	while (n--) {
		resolver();
		if (n)
			cout << endl;
	}

	return 0;
}
