#ifndef COMPRESSORSTATIONMANAGER_H
#define COMPRESSORSTATIONMANAGER_H

#include <iostream>
#include <map>
#include <vector>
#include "CompressorStation.h"

using namespace std;

class CompressorStationManager {
private:
    map<int, CompressorStation> stations;
    int nextId = 1;

public:
    void addStation();
    void displayAllStations() const;
    void editStation();
    void deleteStation();
    vector<int> searchByName(const string& name) const;
    vector<int> searchByUnusedPercent(double percent, char op) const;
    void saveToFile(const string& filename) const;
    void loadFromFile(const string& filename);
    bool isEmpty() const { return stations.empty(); }
};

#endif
