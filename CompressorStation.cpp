#include "CompressorStation.h"
#include "utils.h"
#include <sstream>
#include <cmath>

using namespace std;

CompressorStation::CompressorStation() :
    name(""), totalShops(0), workingShops(0), efficiency(0) {
}

string CompressorStation::getName() const {
    return name;
}

double CompressorStation::getUnusedPercent() const {
    if (totalShops == 0) return 0.0;
    return ((totalShops - workingShops) * 100.0) / totalShops;
}

void CompressorStation::changeWorkingShops(int change) {
    workingShops += change;
    if (workingShops < 0) workingShops = 0;
    if (workingShops > totalShops) workingShops = totalShops;
    log("Изменено рабочих цехов у КС " + name + ": " + to_string(workingShops));
}

istream& operator>>(istream& in, CompressorStation& cs) {
    cout << "Введите название КС: ";
    getline(in, cs.name);
    log("Введено название КС: " + cs.name);

    cs.totalShops = readPositive<int>("Введите общее количество цехов: ", "Неверное количество");
    log("Введено общее количество цехов: " + to_string(cs.totalShops));

    cs.workingShops = readPositive<int>("Введите количество рабочих цехов: ", "Неверное количество");
    if (cs.workingShops > cs.totalShops) {
        cs.workingShops = cs.totalShops;
        cout << "Количество рабочих цехов уменьшено до общего количества.\n";
    }
    log("Введено рабочих цехов: " + to_string(cs.workingShops));

    cs.efficiency = readPositive<int>("Введите эффективность КС (%): ", "Неверная эффективность");
    log("Введена эффективность: " + to_string(cs.efficiency));

    return in;
}

ostream& operator<<(ostream& out, const CompressorStation& cs) {
    out << "КС: " << cs.name
        << ", Цехов: " << cs.workingShops << "/" << cs.totalShops
        << ", Неиспользуется: " << cs.getUnusedPercent() << "%"
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
    return name + ";" +
        to_string(totalShops) + ";" +
        to_string(workingShops) + ";" +
        to_string(efficiency);
}