#ifndef NETWORK_H
#define NETWORK_H

#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <queue>
#include "Pipe.h"
#include "CompressorStation.h"

using namespace std;

struct Edge {
    int pipeId;
    int startCS;
    int endCS;
    int diameter;
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
};

#endif