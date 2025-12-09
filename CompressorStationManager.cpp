#include "CompressorStationManager.h"
#include "utils.h"
#include <fstream>

void CompressorStationManager::addStation() {
    CompressorStation station;
    cin >> station;

    while (stations.find(nextId) != stations.end()) {
        nextId++;
    }
    stations[nextId] = station;

    cout << "КС добавлена с ID: " << nextId << endl;
    log("Добавлена КС с ID: " + to_string(nextId));
    nextId++;
}

void CompressorStationManager::displayAllStations() const {
    if (stations.empty()) {
        cout << "КС не добавлены.\n";
        return;
    }
    cout << "Список всех КС:\n";
    for (const auto& pair : stations) {
        cout << "ID " << pair.first << ": " << pair.second << endl;
    }
    log("Показаны все КС");
}

void CompressorStationManager::editStation() {
    if (stations.empty()) {
        cout << "КС не добавлены.\n";
        return;
    }

    int id = readPositive<int>("Введите ID КС: ", "Неверный ID");
    auto it = stations.find(id);
    if (it != stations.end()) {
        cout << "1. Увеличить рабочие цехи\n";
        cout << "2. Уменьшить рабочие цехи\n";
        int choice = inputIntInRange("Выберите действие: ", 1, 2);
        int change = readPositive<int>("Введите количество: ", "Неверное количество");

        if (choice == 1) it->second.changeWorkingShops(change);
        else it->second.changeWorkingShops(-change);

        cout << "КС изменена.\n";
    }
    else {
        cout << "КС с ID " << id << " не найдена.\n";
    }
}

void CompressorStationManager::deleteStation() {
    if (stations.empty()) {
        cout << "КС не добавлены.\n";
        return;
    }

    int id = readPositive<int>("Введите ID КС: ", "Неверный ID");
    if (stations.erase(id)) {
        cout << "КС удалена.\n";
        log("Удалена КС с ID: " + to_string(id));
    }
    else {
        cout << "КС с ID " << id << " не найдена.\n";
    }
}

vector<int> CompressorStationManager::searchByName(const string& name) const {
    vector<int> foundIds;
    for (const auto& pair : stations) {
        if (pair.second.getName().find(name) != string::npos) {
            foundIds.push_back(pair.first);
        }
    }
    log("Поиск КС по имени '" + name + "': найдено " + to_string(foundIds.size()));

    if (!foundIds.empty()) {
        cout << "Найденные КС:\n";
        for (int id : foundIds) {
            auto it = stations.find(id);
            if (it != stations.end()) {
                cout << "ID " << id << ": " << it->second << endl;
            }
        }
    }

    return foundIds;
}

vector<int> CompressorStationManager::searchByUnusedPercent(double percent, char op) const {
    vector<int> foundIds;
    for (const auto& pair : stations) {
        double stationPercent = pair.second.getUnusedPercent();
        bool match = false;

        switch (op) {
        case '=': match = (stationPercent == percent); break;
        case '>': match = (stationPercent > percent); break;
        case '<': match = (stationPercent < percent); break;
        }

        if (match) {
            foundIds.push_back(pair.first);
        }
    }
    log("Поиск КС по проценту " + string(1, op) + " " + to_string(percent) + ": найдено " + to_string(foundIds.size()));

    if (!foundIds.empty()) {
        cout << "Найденные КС:\n";
        for (int id : foundIds) {
            auto it = stations.find(id);
            if (it != stations.end()) {
                cout << "ID " << id << ": " << it->second << endl;
            }
        }
    }

    return foundIds;
}

void CompressorStationManager::saveToFile(const string& filename) const {
    ofstream file(filename);
    if (file.is_open()) {
        for (const auto& pair : stations) {
            file << pair.first << ":" << pair.second.save() << endl;
        }
        file.close();
        cout << "КС сохранены в " << filename << endl;
        log("Сохранение КС в " + filename);
    }
    else {
        cout << "Ошибка сохранения.\n";
    }
}

void CompressorStationManager::loadFromFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Файл не найден.\n";
        return;
    }

    string line;
    int count = 0;
    while (getline(file, line)) {
        size_t pos = line.find(':');
        if (pos != string::npos) {
            int id = stoi(line.substr(0, pos));
            CompressorStation station;
            station.load(line.substr(pos + 1));
            stations[id] = station;
            count++;

            if (id >= nextId) nextId = id + 1;
        }
    }
    file.close();
    cout << "Загружено КС: " << count << endl;
    log("Загрузка КС из " + filename + ": " + to_string(count));
}

CompressorStation CompressorStationManager::getStationById(int id) const {
    auto it = stations.find(id);
    if (it != stations.end()) {
        return it->second;
    }
    return CompressorStation(); // Возвращаем пустую КС если не найдена
}

bool CompressorStationManager::stationExists(int id) const {
    return stations.find(id) != stations.end();
}