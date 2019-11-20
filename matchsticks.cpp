#include <iostream>
using std::cin;
using std::cout;
using std::endl;
using std::fixed;

using std::max;
using std::min;

#define K 20
#define MAXN 100000
int n;
int q;
int nums[MAXN];
int min_st[MAXN][K + 1];
int max_st[MAXN][K + 1];

int log2(int v) {
	int r = 0;
	while (v /= 2)
		r++;
	return r;
}

int rminq(int l, int r) {
	int j = log2(r - l + 1);
	return min(min_st[l][j], min_st[r - (1 << j) + 1][j]);
}

int rmaxq(int l, int r) {
	int j = log2(r - l + 1);
	return max(max_st[l][j], max_st[r - (1 << j) + 1][j]);
}

int main() {
	cout.precision(1);
	cout << fixed;

	cin >> n;
	for (int i = 0; i < n; i++) {
		cin >> nums[i];
		min_st[i][0] = nums[i];
		max_st[i][0] = nums[i];
	}

	for (int j = 1; j <= K; j++)
	for (int i = 0; i + (1 << j) <= n; i++) {
		min_st[i][j] = min(min_st[i][j - 1], min_st[i + (1 << (j - 1))][j - 1]);
		max_st[i][j] = max(max_st[i][j - 1], max_st[i + (1 << (j - 1))][j - 1]);
	}

	cin >> q;
	while (q--) {
		int l, r;
		cin >> l >> r;

		/* El que menos tarda en quemarse de los que enciendo */
		double min_match = rminq(l, r);
		/* El que más tarda en quemarse de los que enciendo */
		double max_match = rmaxq(l, r);

		/* Cuando se consume min_match quema a todos... */
		if (min_match != max_match)
			max_match = min_match + (max_match - min_match) / 2.0;

		/* ... quema a los que están a la izquierda de lo que prendí */
		if (l)
			max_match = max(max_match, min_match + rmaxq(0, l-1));

		/* ... y a los que están a la derecha */
		if (r < n - 1)
			max_match = max(max_match, min_match + rmaxq(r+1, n-1));

		cout << max_match << endl;
	}
}
