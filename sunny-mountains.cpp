#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <vector>

using namespace std;

struct point {
        double x;
        double y;

        double len() const {
                return sqrt(x * x + y * y);
        }

        point operator-(const point& b) const {
                return point{x - b.x, y - b.y};
        }

        bool operator<(const point& b) const {
                return x < b.x || x == b.x && y < b.y;
        }
};

struct line {
        point hasta;
        point desde;

        double len() const {
                return (hasta - desde).len();
        }
};

void resolver() {
        int n;
        cin >> n;

        vector<point> points(n);
        for (auto& p : points)
                cin >> p.x >> p.y;
        sort(points.begin(), points.end());

        /* {max_y, total_sum} */
        using acc = pair<double, double>;

        /* Toma un resultado parcial y la siguiente línea a considerar,
         * devuelve el resultado parcial de haber considerado esa línea */
        auto suma = [](const acc& a, const line& l) {
                return acc{
                        max(a.first, l.hasta.y),
                        a.second
                        + (a.first < l.hasta.y
                                ? l.len() * (l.hasta.y - a.first) / (l.hasta.y - l.desde.y)
                                : 0)
                };
        };

        /* Toma dos puntos y devuelve una recta */
        auto producto = [](const point& a, const point& b) {
                return line{a, b};
        };

        /* El resultado parcial de no considerar ninguna recta */
        acc inicial{0, 0};

        cout << fixed << setprecision(2)
             << inner_product(points.rbegin() + 1, points.rend(),
                              points.rbegin(), inicial, suma, producto).second
             << endl;
}

int main() {
        int t;
        cin >> t;
        while (t--)
                resolver();
        return 0;
}
