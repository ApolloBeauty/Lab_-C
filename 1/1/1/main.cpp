#include <iostream>
#include <vector>
#include <list>

using namespace std;

class Queue {
private:
    struct Node {
        int value;
        Node* nextNode;
        Node(int val) : value(val), nextNode(nullptr) {}
    };

    Node* head;
    Node* tail;

public:
    Queue() : head(nullptr), tail(nullptr) {}

    bool isEmpty() const {
        return head == nullptr;
    }

    void push(int value) {
        Node* newNode = new Node(value);
        if (tail != nullptr) {
            tail->nextNode = newNode;
        }
        tail = newNode;
        if (head == nullptr) {
            head = newNode;
        }
    }

    int pop() {
        if (isEmpty()) {
            cerr << "Queue is empty" << endl;
            return -1;  // Возвращаем -1 в случае ошибки
        }
        Node* tempNode = head;
        int result = tempNode->value;
        head = head->nextNode;
        if (head == nullptr) {
            tail = nullptr;
        }
        delete tempNode;
        return result;
    }

    ~Queue() {
        while (!isEmpty()) {
            pop();
        }
    }
};

class Graph {
private:
    int numVertices;
    vector<list<int>> adjList;

public:
    Graph(int vertices) : numVertices(vertices), adjList(vertices) {}

    void addEdge(int from, int to) {
        adjList[from].push_back(to);
    }

    void breadthFirstSearch(int startVertex) const {
        vector<bool> visited(numVertices, false);
        Queue queue;

        visited[startVertex] = true;
        queue.push(startVertex);

        while (!queue.isEmpty()) {
            int currentVertex = queue.pop();
            if (currentVertex == -1) break;  // Прерываем, если очередь пуста
            cout << currentVertex << " ";

            for (int neighbor : adjList[currentVertex]) {
                if (!visited[neighbor]) {
                    visited[neighbor] = true;
                    queue.push(neighbor);
                }
            }
        }
    }
};

int main() {
    Graph graph(6); // Создаем граф с 6 вершинами
    graph.addEdge(0, 1);
    graph.addEdge(0, 3);
    graph.addEdge(1, 2);
    graph.addEdge(1, 4);
    graph.addEdge(2, 4);
    graph.addEdge(3, 4);
    graph.addEdge(4, 5);

    graph.breadthFirstSearch(0);
}