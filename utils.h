
#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <sstream>
#include <algorithm>

using namespace std;

void log(const string& action);

template <typename T>
T readPositive(const string& prompt, const string& errorMessage) {
    T value;
    while (true) {
        cout << prompt;
        string input;
        getline(cin, input);
        log("Ввод: " + input);

        if (input.empty()) {
            cout << "Строка пустая.\n";
            continue;
        }

        // Заменяем запятые на точки для дробных чисел
        for (size_t i = 0; i < input.size(); i++) {
            if (input[i] == ',') input[i] = '.';
        }

        stringstream ss(input);
        if (ss >> value && value > 0) {
            return value;
        }
        cout << errorMessage << endl;
    }
}

template <typename T>
T readPositive(const string& prompt, const string& errorMessage, const T maxValue) {
    T value;
    while (true) {
        value = readPositive<T>(prompt, errorMessage);
        if (value < maxValue) {
            return value;
        }
        cout << errorMessage << endl;
    }
}

int inputIntInRange(const string& prompt, int minValue, int maxValue);

#endif