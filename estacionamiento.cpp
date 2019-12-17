#include <iostream>
using std::cin;
using std::cout;
using std::endl;

#include <iomanip>
using std::fixed;
using std::setprecision;

#include <vector>
using std::vector;

#include <cmath>
using std::min;
using std::isnan;

#include <unordered_map>
using std::unordered_map;

enum extremo { IZQ, DER };
unordered_map<size_t, double> dp[2001][2001][2];

vector<double> nums(2001);
vector<double> sums(2002);

double resolver(extremo dir, size_t from, size_t to, size_t pasos) {
	size_t i = dir == IZQ ? from : to - 1;
	if (dp[from][to][dir].count(pasos))
		return dp[from][to][dir][pasos];

	double& rval = dp[from][to][dir][pasos];

	double proba_aca = nums[dir == IZQ ? from : to - 1];
	double proba_afuera = 1 - sums[to] + sums[from] + proba_aca;
	double avanzar_izq = INFINITY;
	double avanzar_der = INFINITY;
	size_t avanzar_izq_pasos = dir == IZQ ? 1 : (to - from);
	size_t avanzar_der_pasos = dir == DER ? 1 : (to - from);

	if (from == 0 && to == nums.size())
		return rval = pasos * proba_afuera;

	if (from > 0)
		avanzar_izq = resolver(IZQ, from - 1, to, avanzar_izq_pasos);

	if (to < nums.size())
		avanzar_der = resolver(DER, from, to + 1, avanzar_der_pasos);

	return rval = pasos * proba_afuera + min(avanzar_izq, avanzar_der);
}

int main() {
	int m;
	cin >> m;
	nums.resize(2 * m + 1);
	sums.resize(2 * m + 1);

	for (size_t i = 0; i < m; i++)
		cin >> nums[i];	
	for (size_t i = m + 1; i < 2 * m + 1; i++)
		cin >> nums[i];

	for (size_t i = 0; i < 2 * m + 1; i++)
		sums[i + 1] = sums[i] + nums[i];

	cout << fixed << setprecision(6);
	cout << resolver(IZQ, m, m + 1, 0) << endl;
	return 0;
}
