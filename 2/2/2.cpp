#include <iostream>
#include <vector>

using namespace std;

class Stac {
private:
    struct Node {
        int value;
        Node* next;
        Node(int val) : value(val), next(nullptr) {}
    };

    Node* head;

public:
    Stac() : head(nullptr) {}

    bool isEmpty() const {
        return head == nullptr;
    }

    void push(int value) {
        Node* newNode = new Node(value);
        newNode->next = head;
        head = newNode;
    }

    int pop() {
        if (isEmpty()) {
            cerr << "Стек пуст" << endl;
            return -1;  
        }
        Node* tempNode = head;
        int result = tempNode->value;
        head = head->next;
        delete tempNode;
        return result;
    }

    int top() const {
        if (isEmpty()) {
            cerr << "Стек пуст" << endl;
            return -1; 
        }
        return head->value;
    }

    ~Stac() {
        while (!isEmpty()) {
            pop();
        }
    }
};

class Graph {
private:
    int numVertices;
    vector<vector<int>> adjMatrix;

    void dfsRecursive(int vertex, vector<bool>& visited) const {
        visited[vertex] = true;
        cout << vertex << " ";
        for (int i = 0; i < numVertices; ++i)
            if (adjMatrix[vertex][i] && !visited[i])
                dfsRecursive(i, visited);
    }

    bool dfsBipartiteCheck(int vertex, vector<int>& color, int currentColor) const {
        color[vertex] = currentColor;
        for (int i = 0; i < numVertices; ++i) {
            if (adjMatrix[vertex][i]) {
                if (color[i] == 0 && !dfsBipartiteCheck(i, color, 3 - currentColor))
                    return false;
                else if (color[i] == currentColor)
                    return false;
            }
        }
        return true;
    }

    bool dfsCycleCheck(int vertex, vector<int>& color) const {
        color[vertex] = 1;
        for (int i = 0; i < numVertices; ++i) {
            if (adjMatrix[vertex][i]) {
                if (color[i] == 1 || (color[i] == 0 && dfsCycleCheck(i, color)))
                    return true;
            }
        }
        color[vertex] = 2;
        return false;
    }

public:
    Graph(int vertices) : numVertices(vertices), adjMatrix(vertices, vector<int>(vertices, 0)) {}

    void addEdge(int from, int to) {
        adjMatrix[from][to] = 1;
    }

    void dfsIterative(int startVertex) const {
        vector<bool> visited(numVertices, false);
        Stac stack;
        stack.push(startVertex);

        while (!stack.isEmpty()) {
            int vertex = stack.pop();

            if (!visited[vertex]) {
                visited[vertex] = true;
                cout << vertex << " ";

                for (int i = numVertices - 1; i >= 0; --i)
                    if (adjMatrix[vertex][i] && !visited[i])
                        stack.push(i);
            }
        }
    }

    void dfs(int startVertex) const {
        vector<bool> visited(numVertices, false);
        dfsRecursive(startVertex, visited);
        cout << endl;
    }

    bool isBipartite() const {
        vector<int> color(numVertices, 0);
        for (int i = 0; i < numVertices; ++i)
            if (color[i] == 0 && !dfsBipartiteCheck(i, color, 1))
                return false;
        return true;
    }

    // Поиск циклов
    bool hasCycle() const {
        vector<int> color(numVertices, 0);
        for (int i = 0; i < numVertices; ++i)
            if (color[i] == 0 && dfsCycleCheck(i, color))
                return true;
        return false;
    }
};

int main() {
    Graph graph(6);
    graph.addEdge(0, 1);
    graph.addEdge(0, 2);
    graph.addEdge(1, 3);
    graph.addEdge(2, 3);
    graph.addEdge(3, 4);
    graph.addEdge(4, 5);

    graph.dfsIterative(0);
    graph.dfs(0);

    cout << "Граф " << (graph.isBipartite() ? "двудольный" : "не двудольный") << endl;
    cout << "Граф " << (graph.hasCycle() ? "содержит цикл" : "не содержит цикл") << endl;

    return 0;
}
