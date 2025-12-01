#include <iostream>
#include <string>
#include <windows.h>
#include "PipeManager.h"
#include "CompressorStationManager.h"
#include "utils.h"

using namespace std;

void displayMainMenu() {
    cout << "\n=== ГЛАВНОЕ МЕНЮ ===\n";
    cout << "1. Управление трубами\n";
    cout << "2. Управление КС\n";
    cout << "3. Сохранить все\n";
    cout << "4. Загрузить все\n";
    cout << "5. Показать все объекты\n";
    cout << "0. Выход\n";
}

void displayPipeMenu() {
    cout << "\n=== УПРАВЛЕНИЕ ТРУБАМИ ===\n";
    cout << "1. Добавить трубу\n";
    cout << "2. Показать все трубы\n";
    cout << "3. Редактировать трубу\n";
    cout << "4. Удалить трубу\n";
    cout << "5. Поиск труб\n";
    cout << "0. Назад\n";
}

void displayStationMenu() {
    cout << "\n=== УПРАВЛЕНИЕ КС ===\n";
    cout << "1. Добавить КС\n";
    cout << "2. Показать все КС\n";
    cout << "3. Редактировать КС\n";
    cout << "4. Удалить КС\n";
    cout << "5. Поиск КС\n";
    cout << "0. Назад\n";
}

void displaySearchPipeMenu() {
    cout << "\n=== ПОИСК ТРУБ ===\n";
    cout << "1. По названию\n";
    cout << "2. По статусу ремонта\n";
    cout << "0. Назад\n";
}

void displaySearchStationMenu() {
    cout << "\n=== ПОИСК КС ===\n";
    cout << "1. По названию\n";
    cout << "2. По проценту незадействованных цехов\n";
    cout << "0. Назад\n";
}

void handlePipeSearch(PipeManager& pipeManager) {
    while (true) {
        displaySearchPipeMenu();
        int choice = inputIntInRange("Выберите действие: ", 0, 2);

        switch (choice) {
        case 1: {
            string name;
            cout << "Введите название для поиска: ";
            getline(cin, name);
            vector<int> found = pipeManager.searchByName(name);

            if (!found.empty()) {
                cout << "\nНайдено труб: " << found.size() << endl;
                cout << "1. Пакетное редактирование\n";
                cout << "2. Пакетное удаление\n";
                cout << "0. Назад\n";

                int action = inputIntInRange("Выберите действие: ", 0, 2);
                if (action == 1) pipeManager.batchEdit(found);
                else if (action == 2) pipeManager.batchDelete(found);
            }
            else {
                cout << "Трубы не найдены.\n";
            }
            break;
        }
        case 2: {
            cout << "1. В ремонте\n";
            cout << "2. Работает\n";
            int status = inputIntInRange("Выберите статус: ", 1, 2);
            vector<int> found = pipeManager.searchByRepairStatus(status == 1);

            if (!found.empty()) {
                cout << "\nНайдено труб: " << found.size() << endl;
                cout << "1. Пакетное редактирование\n";
                cout << "2. Пакетное удаление\n";
                cout << "0. Назад\n";

                int action = inputIntInRange("Выберите действие: ", 0, 2);
                if (action == 1) pipeManager.batchEdit(found);
                else if (action == 2) pipeManager.batchDelete(found);
            }
            else {
                cout << "Трубы не найдены.\n";
            }
            break;
        }
        case 0:
            return;
        }
    }
}

void handleStationSearch(CompressorStationManager& stationManager) {
    while (true) {
        displaySearchStationMenu();
        int choice = inputIntInRange("Выберите действие: ", 0, 2);

        switch (choice) {
        case 1: {
            string name;
            cout << "Введите название для поиска: ";
            getline(cin, name);
            vector<int> found = stationManager.searchByName(name);

            if (found.empty()) {
                cout << "КС не найдены.\n";
            }
            break;
        }
        case 2: {
            double percent = readPositive<double>("Введите процент: ", "Неверный процент");
            cout << "1. Больше (>)\n";
            cout << "2. Меньше (<)\n";
            cout << "3. Равно (=)\n";
            int opChoice = inputIntInRange("Выберите оператор: ", 1, 3);

            char op = '=';
            if (opChoice == 1) op = '>';
            else if (opChoice == 2) op = '<';

            vector<int> found = stationManager.searchByUnusedPercent(percent, op);

            if (found.empty()) {
                cout << "КС не найдены.\n";
            }
            break;
        }
        case 0:
            return;
        }
    }
}

void handlePipes(PipeManager& pipeManager) {
    while (true) {
        displayPipeMenu();
        int choice = inputIntInRange("Выберите действие: ", 0, 5);

        switch (choice) {
        case 1:
            pipeManager.addPipe();
            break;
        case 2:
            pipeManager.displayAllPipes();
            break;
        case 3:
            pipeManager.editPipe();
            break;
        case 4:
            pipeManager.deletePipe();
            break;
        case 5:
            handlePipeSearch(pipeManager);
            break;
        case 0:
            return;
        }
    }
}

void handleStations(CompressorStationManager& stationManager) {
    while (true) {
        displayStationMenu();
        int choice = inputIntInRange("Выберите действие: ", 0, 5);

        switch (choice) {
        case 1:
            stationManager.addStation();
            break;
        case 2:
            stationManager.displayAllStations();
            break;
        case 3:
            stationManager.editStation();
            break;
        case 4:
            stationManager.deleteStation();
            break;
        case 5:
            handleStationSearch(stationManager);
            break;
        case 0:
            return;
        }
    }
}

void saveAll(PipeManager& pipeManager, CompressorStationManager& stationManager) {
    string filename;
    cout << "Введите имя файла: ";
    getline(cin, filename);
    log("Сохранение всех данных в " + filename);

    pipeManager.saveToFile(filename + "_pipes.txt");
    stationManager.saveToFile(filename + "_stations.txt");
}

void loadAll(PipeManager& pipeManager, CompressorStationManager& stationManager) {
    string filename;
    cout << "Введите имя файла: ";
    getline(cin, filename);
    log("Загрузка всех данных из " + filename);

    pipeManager.loadFromFile(filename + "_pipes.txt");
    stationManager.loadFromFile(filename + "_stations.txt");
}

int main() {

    SetConsoleCP(1251); 
    SetConsoleOutputCP(1251);

    PipeManager pipeManager;
    CompressorStationManager stationManager;

    log("=== ПРОГРАММА ЗАПУЩЕНА ===");

    while (true) {
        displayMainMenu();
        int choice = inputIntInRange("Выберите действие: ", 0, 5);
        log("Главное меню - выбор: " + to_string(choice));

        switch (choice) {
        case 1:
            handlePipes(pipeManager);
            break;
        case 2:
            handleStations(stationManager);
            break;
        case 3:
            saveAll(pipeManager, stationManager);
            break;
        case 4:
            loadAll(pipeManager, stationManager);
            break;
        case 5:
            pipeManager.displayAllPipes();
            stationManager.displayAllStations();
            break;
        case 0:
            log("=== ПРОГРАММА ЗАВЕРШЕНА ===");
            return 0;
        }
    }
}