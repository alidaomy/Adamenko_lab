#include "Pipe.h"
#include "utils.h"
#include <sstream>

Pipe::Pipe() : name(""), length(0), diameter(0), underRepair(false) {}

string Pipe::getName() const { return name; }
double Pipe::getLength() const { return length; }
int Pipe::getDiameter() const { return diameter; }
bool Pipe::getUnderRepair() const { return underRepair; }

void Pipe::toggleRepair() {
    underRepair = !underRepair;
    log("Состояние трубы изменено: " + string(underRepair ? "в ремонте" : "работает"));
}

istream& operator>>(istream& in, Pipe& pipe) {
    cout << "Введите название трубы: ";
    getline(in, pipe.name);
    log("Введено название трубы: " + pipe.name);

    pipe.length = readPositive<double>("Введите длину трубы (км): ", "Неверная длина");
    log("Введена длина: " + to_string(pipe.length));

    pipe.diameter = readPositive<int>("Введите диаметр трубы (мм): ", "Неверный диаметр");
    log("Введен диаметр: " + to_string(pipe.diameter));

    pipe.underRepair = false;

    return in;
}

ostream& operator<<(ostream& out, const Pipe& pipe) {
    out << "Труба: " << pipe.name
        << ", Длина: " << pipe.length << " км"
        << ", Диаметр: " << pipe.diameter << " мм"
        << ", В ремонте: " << (pipe.underRepair ? "Да" : "Нет");
    return out;
}

void Pipe::load(const string& data) {
    stringstream ss(data);
    string token;

    getline(ss, name, ';');
    getline(ss, token, ';');
    length = stod(token);
    getline(ss, token, ';');
    diameter = stoi(token);
    getline(ss, token, ';');
    underRepair = (token == "1");
}

string Pipe::save() const {
    return name + ";" + to_string(length) + ";" + to_string(diameter) + ";" + (underRepair ? "1" : "0");
}