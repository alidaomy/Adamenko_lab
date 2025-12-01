#ifndef COMPRESSORSTATION_H
#define COMPRESSORSTATION_H

#include <iostream>
#include <string>

using namespace std;

class CompressorStation {
private:
    string name;
    int totalShops;
    int workingShops;
    int efficiency;

public:
    CompressorStation();
    string getName() const;
    double getUnusedPercent() const;
    void changeWorkingShops(int change);

    friend istream& operator>>(istream& in, CompressorStation& cs);
    friend ostream& operator<<(ostream& out, const CompressorStation& cs);

    void load(const string& data);
    string save() const;
};

#endif