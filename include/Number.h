#ifndef NUMBER_H
#define NUMBER_H

#include "Atom.h"

class Number : public Atom {
    private:
        int value;
    public:
        Number(int i) : Atom() { value = i; }
        bool IsNumber() { return true; }
        std::string AsString() { return std::to_string(value); }
        int GetValue() { return value; }
};

#endif // NUMBER_H
