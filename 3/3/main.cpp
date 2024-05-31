#include <iostream>
#include <vector>
#include <limits>
#include <queue>

using namespace std;

struct Edge {
    int distance;
    int time;
    int cost;
    Edge() : distance(-1), time(-1), cost(-1) {}
    Edge(int d, int t, int c) : distance(d), time(t), cost(c) {}
};

class Graph {
public:
    Graph(int vertices) : adjMatrix(vertices, vector<Edge>(vertices)), V(vertices) {}

    void addEdge(int u, int v, int distance, int time, int cost) {
        adjMatrix[u][v] = Edge(distance, time, cost);
        adjMatrix[v][u] = Edge(distance, time, cost); // Если граф неориентированный
    }

    vector<int> dijkstra(int src, int criterion) const {
        vector<int> dist(V, numeric_limits<int>::max());
        dist[src] = 0;
        vector<bool> visited(V, false);
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;
        pq.push({ 0, src });

        while (!pq.empty()) {
            int u = pq.top().second;
            pq.pop();

            if (visited[u]) continue;
            visited[u] = true;

            for (int v = 0; v < V; ++v) {
                if (adjMatrix[u][v].distance != -1 && !visited[v]) {
                    int weight = 0;
                    if (criterion == 0) weight = adjMatrix[u][v].distance;
                    else if (criterion == 1) weight = adjMatrix[u][v].time;
                    else if (criterion == 2) weight = adjMatrix[u][v].cost;

                    if (dist[u] + weight < dist[v]) {
                        dist[v] = dist[u] + weight;
                        pq.push({ dist[v], v });
                    }
                }
            }
        }

        return dist;
    }

    void printGraph() const {
        for (int i = 0; i < V; ++i) {
            for (int j = 0; j < V; ++j) {
                Edge e = adjMatrix[i][j];
                if (e.distance != -1) {
                    cout << "(" << e.distance << "," << e.time << "," << e.cost << ") ";
                }
                else {
                    cout << "(-1,-1,-1) ";
                }
            }
            cout << endl;
        }
    }

    int getV() const {
        return V;
    }

    const vector<vector<Edge>>& getAdjMatrix() const {
        return adjMatrix;
    }

private:
    vector<vector<Edge>> adjMatrix;
    int V;
};

void findCitiesWithinBudget(const Graph& g, int source, int budget) {
    vector<int> dist = g.dijkstra(source, 2); // для стоимости
    cout << "Cities reachable within budget " << budget << " from source " << source << ": ";
    for (int i = 0; i < dist.size(); ++i) {
        if (dist[i] <= budget) cout << i << " ";
    }
    cout << endl;
}

void findCitiesWithinTime(const Graph& g, int source, int timeLimit) {
    vector<int> dist = g.dijkstra(source, 1); // для стоимости
    cout << "Cities reachable within time " << timeLimit << " from source " << source << ": ";
    for (int i = 0; i < dist.size(); ++i) {
        if (dist[i] <= timeLimit) cout << i << " ";
    }
    cout << endl;
}

bool isReachableWithinHops(const Graph& g, int src, int dest, int maxHops) {
    queue<pair<int, int>> q; 
    q.push({ src, 0 });
    vector<bool> visited(g.getV(), false);

    while (!q.empty()) {
        pair<int, int> current_pair = q.front();
        int current = current_pair.first;
        int hops = current_pair.second;
        q.pop();

        if (current == dest && hops <= maxHops) return true;
        if (hops >= maxHops) continue;

        visited[current] = true;

        for (int i = 0; i < g.getV(); ++i) {
            if (g.getAdjMatrix()[current][i].distance != -1 && !visited[i]) {
                q.push({ i, hops + 1 });
            }
        }
    }

    return false;
}

int maxDistanceWithBudget(const Graph& g, int source, int budget) {
    vector<int> dist = g.dijkstra(source, 2); // для стоимости
    int maxDistance = 0;

    for (int i = 0; i < dist.size(); ++i) {
        if (dist[i] <= budget) {
            int distance = g.dijkstra(source, 0)[i]; // дял дистанции
            if (distance > maxDistance) maxDistance = distance;
        }
    }

    return maxDistance;
}

int main() {
    int vertices = 5;
    Graph g(vertices);

    // Добавим ребра: addEdge(начало, конец, расстояние, время, стоимость)
    g.addEdge(0, 1, 10, 50, 100);
    g.addEdge(0, 2, 15, 30, 120);
    g.addEdge(1, 3, 20, 90, 200);
    g.addEdge(2, 3, 25, 60, 150);
    g.addEdge(3, 4, 30, 120, 250);

    g.printGraph();

    int source = 0;
    vector<int> dist_time = g.dijkstra(source, 1); // для времени
    vector<int> dist_cost = g.dijkstra(source, 2); // для стоимости

    cout << "Shortest times from source " << source << ": ";
    for (int i : dist_time) cout << i << " ";
    cout << endl;

    cout << "Shortest costs from source " << source << ": ";
    for (int i : dist_cost) cout << i << " ";
    cout << endl;

    findCitiesWithinBudget(g, source, 200);
    findCitiesWithinTime(g, source, 100);

    int dest = 4;
    int maxHops = 2;
    bool reachable = isReachableWithinHops(g, source, dest, maxHops);
    cout << "Is city " << dest << " reachable from city " << source << " within " << maxHops << " hopы " << (reachable ? "Yes" : "No") << endl;

    int maxDist = maxDistanceWithBudget(g, source, 250);
    cout << "Max distance with budget 250 from city " << source << ": " << maxDist << endl;

    return 0;
}