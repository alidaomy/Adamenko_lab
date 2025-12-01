#ifndef PIPEMANAGER_H
#define PIPEMANAGER_H

#include <iostream>
#include <map>
#include <vector>
#include "Pipe.h"

using namespace std;

class PipeManager {
private:
    map<int, Pipe> pipes;
    int nextId = 1;

public:
    void addPipe();
    void displayAllPipes() const;
    void editPipe();
    void deletePipe();
    vector<int> searchByName(const string& name) const;
    vector<int> searchByRepairStatus(bool inRepair) const;
    void batchEdit(const vector<int>& ids);
    void batchDelete(const vector<int>& ids);
    void saveToFile(const string& filename) const;
    void loadFromFile(const string& filename);
    bool isEmpty() const { return pipes.empty(); }
};

#endif
