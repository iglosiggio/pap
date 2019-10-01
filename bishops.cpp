#include <iostream>
#include <vector>

using std::cin;
using std::cout;
using std::endl;
using std::vector;

unsigned long long dp[31][31*31];

static unsigned long long int resolver(int n, int k, bool es_centro) {
	unsigned long long r = -1;

	if (k == 0)
		return 1;

	if (k > n)
		return 0;

	if (n == 1)
	switch (k) {
		case 0:  return 1;
		case 1:  return es_centro ? 1 : 2;
	} 

	if (n == 2)
	switch (k) {
		case 0:  return 1;
		case 1:  return es_centro ? 2 : 4;
		case 2:  return es_centro ? 0 : 2;
	}

	unsigned long long& resultado = es_centro ? r : dp[n][k];

	if (resultado == -1)
		resultado = 0;
	else
		return resultado;

	resultado += resolver(n - 2, k, false);

	if (es_centro)
		resultado += resolver(n - 2, k - 1, false) * (n - (k - 1));
	else
		resultado += resolver(n - 2, k - 1, false) * (n - (k - 1)) * 2;

	if (!es_centro && k > 1)
		resultado += resolver(n - 2, k - 2, false) * (n - (k - 2))
		                                           * (n - (k - 2) - 1);

	return resultado;
}

static unsigned long long int resolver(int n, int k) {
	unsigned long long int resultado = 0;

	if (n % 2)
		for (int i = 0; i <= k; i++)
			resultado += resolver(n, k - i, true)
				   * resolver(n - 1, i, false);
	else
		for (int i = 0; i <= k; i++)
			resultado += resolver(n, k - i, true)
				   * resolver(n, i, true);

	return resultado;
}

int main(int argc, char* argv[]) {
	int n, k;

	for (int i = 0; i < 31; i++)
	for (int j = 0; j < 31*31; j++)
		dp[i][j] = -1;

	while(cin >> n >> k, n)
		cout << resolver(n, k) << endl;
	
	return 0;
}
