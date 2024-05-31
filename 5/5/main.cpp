#include <iostream>
#include <vector>
#include <queue>

using namespace std;

struct Point {
    int x, y, turns, time;
    Point(int _x, int _y, int _turns, int _time) : x(_x), y(_y), turns(_turns), time(_time) {}
};

int bfs(vector<vector<int>>& grid, int startX, int startY, int endX, int endY, int maxTurns) {
    int n = grid.size(), m = grid[0].size();
    vector<vector<vector<bool>>> visited(n, vector<vector<bool>>(m, vector<bool>(maxTurns + 1, false)));
    queue<Point> q;
    q.push(Point(startX, startY, 0, 0));
    visited[startX][startY][0] = true;

    int dx[] = { 1, 0, -1, 0 };
    int dy[] = { 0, 1, 0, -1 };

    while (!q.empty()) {
        Point p = q.front();
        q.pop();

        if (p.x == endX && p.y == endY) {
            return p.time;
        }

        for (int i = 0; i < 4; ++i) {
            int newX = p.x + dx[i];
            int newY = p.y + dy[i];
            int newTurns = p.turns;
            int newTime = p.time + 1;

            if (i != 0) { // Check if turning right
                newTurns++;
            }

            if (newX >= 0 && newX < n && newY >= 0 && newY < m && grid[newX][newY] != 1 && newTurns <= maxTurns) {
                if (!visited[newX][newY][newTurns]) {
                    visited[newX][newY][newTurns] = true;
                    q.push(Point(newX, newY, newTurns, newTime));
                }
            }
        }
    }

    return -1; // Couldn't reach the end
}

int main() {
    int n, m;
    cin >> n >> m;
    vector<vector<int>> grid(n, vector<int>(m));
    int startX, startY, endX, endY;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            cin >> grid[i][j];
            if (grid[i][j] == 2) {
                startX = i;
                startY = j;
            }
            else if (grid[i][j] == 3) {
                endX = i;
                endY = j;
            }
        }
    }
    int maxTurns;
    cin >> maxTurns;

    int minTime = bfs(grid, startX, startY, endX, endY, maxTurns);
    cout << minTime << endl;

    return 0;
}