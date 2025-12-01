#include "PipeManager.h"
#include "utils.h"
#include <fstream>

void PipeManager::addPipe() {
    Pipe pipe;
    cin >> pipe;

    while (pipes.find(nextId) != pipes.end()) {
        nextId++;
    }
    pipes[nextId] = pipe;

    cout << "Труба добавлена с ID: " << nextId << endl;
    log("Труба добавлена с ID: " + to_string(nextId));
    nextId++;
}

void PipeManager::displayAllPipes() const {
    if (pipes.empty()) {
        cout << "Трубы не добавлены.\n";
        return;
    }
    cout << "Список всех труб:\n";
    for (const auto& pair : pipes) {
        cout << "ID " << pair.first << ": " << pair.second << endl;
    }
    log("Отображаются все трубы");
}

void PipeManager::editPipe() {
    if (pipes.empty()) {
        cout << "Трубы не добавлены.\n";
        return;
    }

    int id = readPositive<int>("Введите ID трубы: ", "Неверный ID");
    auto it = pipes.find(id);
    if (it != pipes.end()) {
        cout << "\nТекущая информация о трубе:\n";
        cout << "ID " << id << ": " << it->second << endl;

        cout << "\nИзменить статус ремонта:\n";
        cout << "1. Установить 'В ремонте'\n";
        cout << "2. Установить 'Работает'\n";
        cout << "3. Переключить (инвертировать текущий статус)\n";
        cout << "0. Отмена\n";

        int choice = inputIntInRange("Выберите действие: ", 0, 3);

        // Создаем копию трубы для редактирования
        Pipe pipe = it->second;

        switch (choice) {
        case 1:
            if (!pipe.getUnderRepair()) {
                pipe.toggleRepair(); // меняем на "в ремонте"
            }
            cout << "Статус установлен: В ремонте\n";
            break;
        case 2:
            if (pipe.getUnderRepair()) {
                pipe.toggleRepair(); // меняем на "работает"
            }
            cout << "Статус установлен: Работает\n";
            break;
        case 3:
            pipe.toggleRepair();
            cout << "Статус переключен\n";
            break;
        case 0:
            cout << "Отмена.\n";
            return;
        }

        // Сохраняем измененную трубу
        pipes[id] = pipe;
        cout << "Изменения сохранены.\n";
    }
    else {
        cout << "Труба с ID " << id << " не найдена.\n";
    }
}

void PipeManager::deletePipe() {
    if (pipes.empty()) {
        cout << "Трубы не добавлены.\n";
        return;
    }

    int id = readPositive<int>("Введите ID трубы: ", "Неверный ID");
    if (pipes.erase(id)) {
        cout << "Труба удалена.\n";
        log("Удалена труба с ID: " + to_string(id));
    }
    else {
        cout << "Труба с ID " << id << " не найдена.\n";
    }
}

vector<int> PipeManager::searchByName(const string& name) const {
    vector<int> foundIds;
    for (const auto& pair : pipes) {
        if (pair.second.getName().find(name) != string::npos) {
            foundIds.push_back(pair.first);
        }
    }
    log("Поиск труб по имени '" + name + "': найдено " + to_string(foundIds.size()));

    if (!foundIds.empty()) {
        cout << "Найденные трубы:\n";
        for (int id : foundIds) {
            auto it = pipes.find(id);
            if (it != pipes.end()) {
                cout << "ID " << id << ": " << it->second << endl;
            }
        }
    }
    else {
        cout << "Трубы не найдены.\n";
    }

    return foundIds;
}

vector<int> PipeManager::searchByRepairStatus(bool inRepair) const {
    vector<int> foundIds;
    for (const auto& pair : pipes) {
        if (pair.second.getUnderRepair() == inRepair) {
            foundIds.push_back(pair.first);
        }
    }
    log("Поиск труб по статусу ремонта: найдено " + to_string(foundIds.size()));

    if (!foundIds.empty()) {
        cout << "Найденные трубы:\n";
        for (int id : foundIds) {
            auto it = pipes.find(id);
            if (it != pipes.end()) {
                cout << "ID " << id << ": " << it->second << endl;
            }
        }
    }
    else {
        cout << "Трубы не найдены.\n";
    }

    return foundIds;
}

void PipeManager::batchEdit(const vector<int>& ids) {
    for (int id : ids) {
        auto it = pipes.find(id);
        if (it != pipes.end()) {
            it->second.toggleRepair();
        }
    }
    cout << "Пакетное редактирование завершено.\n";
    log("Пакетное редактирование " + to_string(ids.size()) + " труб");
}

void PipeManager::batchDelete(const vector<int>& ids) {
    for (int id : ids) {
        pipes.erase(id);
    }
    cout << "Пакетное удаление завершено.\n";
    log("Пакетное удаление " + to_string(ids.size()) + " труб");
}

void PipeManager::saveToFile(const string& filename) const {
    ofstream file(filename);
    if (file.is_open()) {
        for (const auto& pair : pipes) {
            file << pair.first << ":" << pair.second.save() << endl;
        }
        file.close();
        cout << "Трубы сохранены в " << filename << endl;
        log("Сохранение труб в " + filename);
    }
    else {
        cout << "Ошибка сохранения.\n";
    }
}

void PipeManager::loadFromFile(const string& filename) {
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
            Pipe pipe;
            pipe.load(line.substr(pos + 1));
            pipes[id] = pipe;
            count++;

            if (id >= nextId) nextId = id + 1;
        }
    }
    file.close();
    cout << "Загружено труб: " << count << endl;
    log("Загрузка труб из " + filename + ": " + to_string(count));
}