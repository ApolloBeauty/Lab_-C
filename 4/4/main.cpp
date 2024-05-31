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
        adjMatrix[v][u] = Edge(distance, time, cost); // ≈сли граф неориентированный
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

    vector<Edge> primMST(int criterion) const {
        vector<int> key(V, numeric_limits<int>::max());
        vector<int> parent(V, -1);
        vector<bool> inMST(V, false);

        key[0] = 0;
        for (int count = 0; count < V - 1; ++count) {
            int u = minKey(key, inMST);
            inMST[u] = true;

            for (int v = 0; v < V; ++v) {
                if (adjMatrix[u][v].distance != -1 && !inMST[v]) {
                    int weight = 0;
                    if (criterion == 0) weight = adjMatrix[u][v].distance;
                    else if (criterion == 1) weight = adjMatrix[u][v].time;
                    else if (criterion == 2) weight = adjMatrix[u][v].cost;

                    if (weight < key[v]) {
                        key[v] = weight;
                        parent[v] = u;
                    }
                }
            }
        }

        vector<Edge> mst;
        for (int i = 1; i < V; ++i) {
            int u = parent[i];
            if (u != -1) {
                int v = i;
                mst.push_back(adjMatrix[u][v]);
            }
        }

        return mst;
    }

private:
    vector<vector<Edge>> adjMatrix;
    int V;

    int minKey(const vector<int>& key, const vector<bool>& inMST) const {
        int min = numeric_limits<int>::max(), min_index;

        for (int v = 0; v < V; ++v)
            if (!inMST[v] && key[v] < min)
                min = key[v], min_index = v;

        return min_index;
    }
};

void printMST(const vector<Edge>& mst) {
    int totalDistance = 0;
    int totalTime = 0;
    int totalCost = 0;
    for (const Edge& e : mst) {
        totalDistance += e.distance;
        totalTime += e.time;
        totalCost += e.cost;
        cout << "(" << e.distance << ", " << e.time << ", " << e.cost << ") ";
    }
    cout << endl;
    cout << "Total Distance: " << totalDistance << endl;
    cout << "Total Time: " << totalTime << endl;
    cout << "Total Cost: " << totalCost << endl;
}

int main() {
    int vertices = 5;
    Graph g(vertices);

    // ƒобавим ребра: addEdge(начало, конец, рассто€ние, врем€, стоимость)
    g.addEdge(0, 1, 10, 50, 100);
    g.addEdge(0, 2, 15, 30, 120);
    g.addEdge(1, 3, 20, 90, 200);
    g.addEdge(2, 3, 25, 60, 150);
    g.addEdge(3, 4, 30, 120, 250);

    g.printGraph();

    cout << "Prim MST by Distance:" << endl;
    vector<Edge> mstDistance = g.primMST(0); // дл€ дист
    printMST(mstDistance);

    cout << "Prim MST by Time:" << endl;
    vector<Edge> mstTime = g.primMST(1); // дл€ времени
    printMST(mstTime);

    cout << "Prim MST by Cost:" << endl;
    vector<Edge> mstCost = g.primMST(2); // дл€ стоимости
    printMST(mstCost);

    return 0;
}
