#ifndef NETWORK_H
#define NETWORK_H

#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <queue>
#include <climits>
#include "Pipe.h"
#include "CompressorStation.h"

using namespace std;

struct Edge {
    int pipeId;
    int startCS;
    int endCS;
    int diameter;
    double capacity; // Пропускная способность 
    bool operator==(const Edge& other) const {
        return pipeId == other.pipeId && startCS == other.startCS && endCS == other.endCS;
    }
};

class Network {
private:
    map<int, vector<Edge>> adjacencyList;
    map<int, Pipe> pipes;
    map<int, CompressorStation> stations;
    map<int, bool> usedPipes;

public:
    void addConnection(int pipeId, int startCS, int endCS, int diameter, Pipe& pipe, CompressorStation& startStation, CompressorStation& endStation);
    void displayNetwork() const;
    vector<int> topologicalSort() const;
    bool hasCycle() const;
    bool hasCycleDFS(int v, map<int, bool>& visited, map<int, bool>& recStack) const;
    map<int, vector<Edge>> getAdjacencyList() const { return adjacencyList; }
    bool isPipeUsed(int pipeId) const;

    //расчет максимального потока
    double maximumFlow(int source, int sink);
    bool bfsForFlow(int source, int sink, map<int, int>& parent);

    //кратчайший путь
    vector<int> shortestPath(int startCS, int endCS);
    double getPipeWeight(int pipeId); // Вес трубы для пути (например, длина)
};

#endif