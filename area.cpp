#include <iostream>

using std::cout;
using std::cin;
using std::ios;
using std::endl;
using num = long long;

static
void resolver() {
	static num t[101][101];
	num n, m, k, s, p;
	cin >> n >> m >> k;
	
	
	for (num i = 0; i <= n; i++)
		t[i][0] = 0;
	for (num j = 0; j <= m; j++)
		t[0][j] = 0;

	for (num i = 1; i <= n; i++)
	for (num j = 1; j <= m; j++)
		cin >> t[i][j];

	for (num i = 1; i <= n; i++)
	for (num j = 1; j <= m; j++)
		t[i][j] += t[i][j - 1];

	for (num i = 1; i <= n; i++)
	for (num j = 1; j <= m; j++)
		t[i][j] += t[i - 1][j];

	s = 0;
	p = 0;

	for (num i = 0; i < n; i++)
	for (num j = 0; j < m; j++)
	for (num y = i + 1; y <= n; y++)
	for (num x = j + 1; x <= m; x++) {
		num ancho = y - i;
		num alto = x - j;
		num area = ancho * alto;
		num precio = t[y][x] - t[i][x] - t[y][j] + t[i][j];

		if (precio <= k && area > s) {
			s = area;
			p = precio;
		} else if (precio < p && area == s)
			p = precio;
	}

	cout << s << ' ' << p << endl;
}

int main(int argc, char* argv[]) {
	ios::sync_with_stdio(false);
	cin.tie(0);
	cout.tie(0);

	num t;
	cin >> t;

	for (num i = 1; i <= t; i++) {
		cout << "Case #" << i << ": ";
		resolver();
	}

	return 0;
}
