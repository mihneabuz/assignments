#include <iostream>
#include <fstream>
#include <vector>
#include <queue>

using namespace std;

struct poz {
	int x;
	int y;
};

const vector<poz> directions_map[] =  {
	{},                                    // 0
	{{-1, 0}, {1, 0}},                     // 1
	{{0, -1}, {0, 1}},                     // 2
	{{-1, 0}, {1, 0}, {0, -1}, {0, 1}}     // 3
};

int main() {
	ifstream in("poduri.in");
    ofstream out("poduri.out");

	int n, m;
	in >> n >> m;

	int x, y;
	in >> x >> y;
	poz start{x - 1, y - 1};

	vector<vector<int>> A(n, vector<int>(m, 0));
	vector<vector<int>> costs(n, vector<int>(m, 0));

	char aux;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			in >> aux;

			if (aux == 'V')
				A[i][j] = 1;
			else if (aux == 'O')
				A[i][j] = 2;
			else if (aux == 'D')
				A[i][j] = 3;
		}
	}

	queue<poz> q;
	q.push(start);

	while (!q.empty()) {
		auto p = q.front();
		int bridge = A[p.x][p.y];

		auto directions = directions_map[bridge];

		for (auto d : directions) {
			auto new_x = p.x + d.x;
			auto new_y = p.y + d.y;

			if (new_x < 0 || new_x >= n || new_y < 0 || new_y >= m) {
				out << costs[p.x][p.y] + 1;
				in.close();
				out.close();
				return 0;

			} else if (!costs[new_x][new_y]) {
				costs[new_x][new_y] = costs[p.x][p.y] + 1;
				q.push({new_x, new_y});
			}
		}
		q.pop();
	}

	out << -1;
	in.close();
	out.close();
	return 0;
}
