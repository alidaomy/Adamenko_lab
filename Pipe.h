#ifndef PIPE_H
#define PIPE_H

#include <iostream>
#include <string>

using namespace std;

class Pipe {
private:
    string name;
    double length;
    int diameter;
    bool underRepair;

public:
    Pipe();
    string getName() const;
    double getLength() const;
    int getDiameter() const;
    bool getUnderRepair() const;
    void toggleRepair();

    friend istream& operator>>(istream& in, Pipe& pipe);
    friend ostream& operator<<(ostream& out, const Pipe& pipe);

    void load(const string& data);
    string save() const;
};

#endif
