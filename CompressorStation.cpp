#include "CompressorStation.h"
#include "utils.h"
#include <sstream>

CompressorStation::CompressorStation() : name(""), totalShops(0), workingShops(0), efficiency(0) {}

string CompressorStation::getName() const { return name; }

double CompressorStation::getUnusedPercent() const {
    if (totalShops == 0) return 0;
    return ((totalShops - workingShops) / (double)totalShops) * 100;
}

void CompressorStation::changeWorkingShops(int change) {
    workingShops += change;
    if (workingShops < 0) workingShops = 0;
    if (workingShops > totalShops) workingShops = totalShops;
    log("Изменение рабочих цехов на: " + to_string(change));
}

istream& operator>>(istream& in, CompressorStation& cs) {
    cout << "Введите название КС: ";
    getline(in, cs.name);
    log("Введено название КС: " + cs.name);

    cs.totalShops = readPositive<int>("Введите количество цехов: ", "Неверное количество цехов");
    log("Введено цехов: " + to_string(cs.totalShops));

    cs.workingShops = readPositive<int>("Введите количество рабочих цехов: ", "Неверное количество рабочих цехов", cs.totalShops + 1);
    log("Введено рабочих цехов: " + to_string(cs.workingShops));

    cs.efficiency = readPositive<int>("Введите эффективность (%): ", "Неверная эффективность", 101);
    log("Введена эффективность: " + to_string(cs.efficiency));

    return in;
}

ostream& operator<<(ostream& out, const CompressorStation& cs) {
    double unusedPercent = cs.getUnusedPercent();
    out << "КС: " << cs.name
        << ", Цехи: " << cs.workingShops << "/" << cs.totalShops
        << ", Незадействовано: " << unusedPercent << "%"
        << ", Эффективность: " << cs.efficiency << "%";
    return out;
}

void CompressorStation::load(const string& data) {
    stringstream ss(data);
    string token;

    getline(ss, name, ';');
    getline(ss, token, ';');
    totalShops = stoi(token);
    getline(ss, token, ';');
    workingShops = stoi(token);
    getline(ss, token, ';');
    efficiency = stoi(token);
}

string CompressorStation::save() const {
    return name + ";" + to_string(totalShops) + ";" + to_string(workingShops) + ";" + to_string(efficiency);
}