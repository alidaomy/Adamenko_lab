#include "Network.h"
#include "utils.h"
#include <iostream>

void Network::addConnection(int pipeId, int startCS, int endCS, int diameter, Pipe& pipe, CompressorStation& startStation, CompressorStation& endStation) {
    Edge edge{ pipeId, startCS, endCS, diameter };

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