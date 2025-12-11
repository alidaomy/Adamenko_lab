#include "Network.h"
#include "utils.h"
#include <algorithm>
#include <climits>
#include <iostream>

void Network::addConnection(int pipeId, int startCS, int endCS, int diameter, Pipe& pipe, CompressorStation& startStation, CompressorStation& endStation) {
    Edge edge{ pipeId, startCS, endCS, diameter, (double)diameter };

    // Проверяем, существует ли уже такое соединение
    for (const auto& existingEdge : adjacencyList[startCS]) {
        if (existingEdge.endCS == endCS && existingEdge.pipeId == pipeId) {
            cout << "Соединение уже существует!\n";
            return;
        }
    }

    adjacencyList[startCS].push_back(edge);
    pipes[pipeId] = pipe;
    stations[startCS] = startStation;
    stations[endCS] = endStation;
    usedPipes[pipeId] = true;

    cout << "Соединение добавлено: КС " << startCS << " -> КС " << endCS
        << " (труба ID: " << pipeId << ", диаметр: " << diameter << " мм)\n";
    log("Добавлено соединение: КС " + to_string(startCS) + " -> КС " + to_string(endCS) +
        " (труба ID: " + to_string(pipeId) + ")");
}

void Network::displayNetwork() const {
    if (adjacencyList.empty()) {
        cout << "Газотранспортная сеть пуста.\n";
        return;
    }

    cout << "=== ГАЗОТРАНСПОРТНАЯ СЕТЬ ===\n";
    for (const auto& pair : adjacencyList) {
        int stationId = pair.first;
        cout << "КС " << stationId << " соединена с:\n";
        for (const Edge& edge : pair.second) {
            cout << "  -> КС " << edge.endCS << " (труба ID: " << edge.pipeId
                << ", диаметр: " << edge.diameter << " мм)\n";
        }
    }
    log("Отображение газотранспортной сети");
}

vector<int> Network::topologicalSort() const {
    vector<int> result;
    map<int, int> inDegree;
    queue<int> q;

    // Инициализация степеней входа
    for (const auto& pair : adjacencyList) {
        inDegree[pair.first] = 0;
        for (const Edge& edge : pair.second) {
            inDegree[edge.endCS] = 0;
        }
    }

    // Вычисление степеней входа
    for (const auto& pair : adjacencyList) {
        for (const Edge& edge : pair.second) {
            inDegree[edge.endCS]++;
        }
    }

    // Добавление вершин с нулевой степенью входа в очередь
    for (const auto& pair : inDegree) {
        if (pair.second == 0) {
            q.push(pair.first);
        }
    }

    // Топологическая сортировка
    while (!q.empty()) {
        int current = q.front();
        q.pop();
        result.push_back(current);

        if (adjacencyList.find(current) != adjacencyList.end()) {
            for (const Edge& edge : adjacencyList.at(current)) {
                inDegree[edge.endCS]--;
                if (inDegree[edge.endCS] == 0) {
                    q.push(edge.endCS);
                }
            }
        }
    }

    // Проверка на цикл
    if (result.size() != inDegree.size()) {
        cout << "В сети обнаружен цикл! Топологическая сортировка невозможна.\n";
        return vector<int>();
    }

    return result;
}

bool Network::hasCycle() const {
    map<int, bool> visited;
    map<int, bool> recStack;

    for (const auto& pair : adjacencyList) {
        if (!visited[pair.first]) {
            if (hasCycleDFS(pair.first, visited, recStack)) {
                return true;
            }
        }
    }
    return false;
}

bool Network::hasCycleDFS(int v, map<int, bool>& visited, map<int, bool>& recStack) const {
    if (!visited[v]) {
        visited[v] = true;
        recStack[v] = true;

        if (adjacencyList.find(v) != adjacencyList.end()) {
            for (const Edge& edge : adjacencyList.at(v)) {
                int neighbor = edge.endCS;
                if (!visited[neighbor] && hasCycleDFS(neighbor, visited, recStack)) {
                    return true;
                }
                else if (recStack[neighbor]) {
                    return true;
                }
            }
        }
    }
    recStack[v] = false;
    return false;
}

bool Network::isPipeUsed(int pipeId) const {
    auto it = usedPipes.find(pipeId);
    return it != usedPipes.end() && it->second;
}

bool Network::bfsForFlow(int source, int sink, map<int, int>& parent) {
    map<int, bool> visited;
    queue<int> q;

    q.push(source);
    visited[source] = true;
    parent[source] = -1;

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        for (const Edge& edge : adjacencyList[u]) {
            int v = edge.endCS;
            if (!visited[v] && edge.capacity > 0) {
                q.push(v);
                parent[v] = u;
                visited[v] = true;
                if (v == sink) return true;
            }
        }
    }
    return false;
}

// НОВЫЙ МЕТОД: максимальный поток (Форд-Фалкерсон)
double Network::maximumFlow(int source, int sink) {
    double maxFlow = 0;
    map<int, int> parent;

    // Создаем копию графа с пропускными способностями
    map<int, vector<Edge>> residualGraph = adjacencyList;

    // Устанавливаем capacity для всех ребер (здесь - диаметр как пропускная способность)
    for (auto& pair : residualGraph) {
        for (Edge& edge : pair.second) {
            edge.capacity = edge.diameter; // или другая формула
        }
    }

    // Ищем увеличивающие пути
    while (bfsForFlow(source, sink, parent)) {
        double pathFlow = INT_MAX;

        // Находим минимальную пропускную способность на пути
        for (int v = sink; v != source; v = parent[v]) {
            int u = parent[v];
            for (const Edge& edge : residualGraph[u]) {
                if (edge.endCS == v) {
                    pathFlow = min(pathFlow, edge.capacity);
                    break;
                }
            }
        }

        // Обновляем остаточные пропускные способности
        for (int v = sink; v != source; v = parent[v]) {
            int u = parent[v];

            // Уменьшаем capacity прямого ребра
            for (Edge& edge : residualGraph[u]) {
                if (edge.endCS == v) {
                    edge.capacity -= pathFlow;
                    break;
                }
            }

            // Увеличиваем capacity обратного ребра
            bool reverseEdgeFound = false;
            for (Edge& edge : residualGraph[v]) {
                if (edge.endCS == u) {
                    edge.capacity += pathFlow;
                    reverseEdgeFound = true;
                    break;
                }
            }

            // Если обратного ребра нет - добавляем
            if (!reverseEdgeFound) {
                Edge reverseEdge;
                reverseEdge.startCS = v;
                reverseEdge.endCS = u;
                reverseEdge.capacity = pathFlow;
                residualGraph[v].push_back(reverseEdge);
            }
        }

        maxFlow += pathFlow;
    }

    return maxFlow;
}

// НОВЫЙ МЕТОД: вес трубы (например, длина)
double Network::getPipeWeight(int pipeId) {
    auto pipeIt = pipes.find(pipeId);
    if (pipeIt != pipes.end()) {
        return pipeIt->second.getLength(); // Длина как вес
    }
    return INT_MAX; // Если труба не найдена
}

// НОВЫЙ МЕТОД: кратчайший путь (Дейкстра)
vector<int> Network::shortestPath(int startCS, int endCS) {
    map<int, double> dist;
    map<int, int> prev;
    set<pair<double, int>> pq;

    for (const auto& pair : adjacencyList) {
        dist[pair.first] = INT_MAX;
        prev[pair.first] = -1;
    }
    for (const auto& pipePair : pipes) {
        // КС, соединенные трубами, уже учтены в adjacencyList
    }

    dist[startCS] = 0;
    pq.insert({ 0, startCS });

    while (!pq.empty()) {
        int u = pq.begin()->second;
        pq.erase(pq.begin());

        if (u == endCS) break;

        for (const Edge& edge : adjacencyList[u]) {
            int v = edge.endCS;
            double weight = getPipeWeight(edge.pipeId);

            if (dist[v] > dist[u] + weight) {
                pq.erase({ dist[v], v });
                dist[v] = dist[u] + weight;
                prev[v] = u;
                pq.insert({ dist[v], v });
            }
        }
    }

    // Восстановление пути
    vector<int> path;
    if (dist[endCS] < INT_MAX) {
        for (int at = endCS; at != -1; at = prev[at]) {
            path.push_back(at);
        }
        reverse(path.begin(), path.end());
    }

    return path;
}