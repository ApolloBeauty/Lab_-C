#include <iostream>
#include <vector>

using namespace std;

struct Route {
    vector<int> stops;
};

// Задание 1: Проверка возможности пути
bool canReachInOneTransport(const vector<Route>& routes, int start, int end) {
    for (const Route& route : routes) {
        bool foundStart = false;
        for (int stop : route.stops) {
            if (stop == start) foundStart = true;
            if (foundStart && stop == end) return true;
        }
    }
    return false;
}

vector<vector<int>> buildGraph(const vector<Route>& routes, int N) {
    vector<vector<int>> graph(N);
    for (const Route& route : routes) {
        for (int i = 0; i < route.stops.size(); ++i) {
            for (int j = i + 1; j < route.stops.size(); ++j) {
                graph[route.stops[i] - 1].push_back(route.stops[j] - 1);
                graph[route.stops[j] - 1].push_back(route.stops[i] - 1);
            }
        }
    }
    return graph;
}

bool dfs(const vector<vector<int>>& graph, int current, int end, int transfers, vector<bool>& visited) {
    if (current == end) return true;
    if (transfers < 0) return false;

    visited[current] = true;

    for (int neighbor : graph[current]) {
        if (!visited[neighbor] && dfs(graph, neighbor, end, transfers - 1, visited)) {
            return true;
        }
    }

    visited[current] = false;
    return false;
}

bool canReachWithTransfers(const vector<Route>& routes, int start, int end, int maxTransfers) {
    int N = routes.size();
    auto graph = buildGraph(routes, N);
    vector<bool> visited(N, false);
    return dfs(graph, start - 1, end - 1, maxTransfers, visited);
}

// Задание 2: Проверка достижения всех остановок
bool isConnected(const vector<Route>& routes, int N) {
    auto graph = buildGraph(routes, N);
    vector<vector<bool>> reachable(N, vector<bool>(N, false));

    for (int i = 0; i < N; ++i) {
        reachable[i][i] = true;
    }

    for (int i = 0; i < N; ++i) {
        for (int j : graph[i]) {
            reachable[i][j] = true;
        }
    }

    for (int k = 0; k < N; ++k) {
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                reachable[i][j] = reachable[i][j] || (reachable[i][k] && reachable[k][j]);
            }
        }
    }

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (!reachable[i][j]) {
                return false;
            }
        }
    }

    return true;
}

// Задание 3: Проверка достижения при удалении любого маршрута
bool checkWithRouteRemoved(const vector<Route>& routes, int N) {
    for (int i = 0; i < routes.size(); ++i) {
        vector<Route> remainingRoutes;
        for (int j = 0; j < routes.size(); ++j) {
            if (i != j) remainingRoutes.push_back(routes[j]);
        }
        if (!isConnected(remainingRoutes, N)) {
            return false;
        }
    }
    return true;
}

int main() {

    int N, M;
    cout << "Enter the number of stops and routes: ";
    cin >> N >> M;

    vector<Route> routes(M);
    for (int i = 0; i < M; ++i) {
        int numStops;
        cout << "Enter the number of stops in route " << i + 1 << ": ";
        cin >> numStops;
        routes[i].stops.resize(numStops);
        for (int j = 0; j < numStops; ++j) {
            cin >> routes[i].stops[j];
        }
    }

    int start, end, maxTransfers;
    cout << "Enter start and end stops to check: ";
    cin >> start >> end;
    if (canReachInOneTransport(routes, start, end)) {
        cout << "You can reach from " << start << " to " << end << " on one transport.\n";
    }
    else {
        cout << "You cannot reach from " << start << " to " << end << " on one transport.\n";
    }

    cout << "Enter start, end stops and max transfers to check: ";
    cin >> start >> end >> maxTransfers;
    if (canReachWithTransfers(routes, start, end, maxTransfers)) {
        cout << "You can reach from " << start << " to " << end << " with max " << maxTransfers << " transfers.\n";
    }
    else {
        cout << "You cannot reach from " << start << " to " << end << " with max " << maxTransfers << " transfers.\n";
    }

    if (isConnected(routes, N)) {
        cout << "All stops are reachable from each other.\n";
    }
    else {
        cout << "Not all stops are reachable from each other.\n";
    }

    if (checkWithRouteRemoved(routes, N)) {
        cout << "All stops are reachable from each other even if any one route is removed.\n";
    }
    else {
        cout << "There exists a route, the removal of which will make some stops unreachable.\n";
    }

    return 0;
}