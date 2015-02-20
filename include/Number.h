#ifndef NUMBER_H
#define NUMBER_H

#include "Atom.h"

class Number : public Atom {
    private:
        int value;
    public:
        Number(int i) : Atom() { value = i; }
        virtual bool IsA(DataType T) { return T == NUMBER ? true : Atom::IsA(T); }
        std::string AsString() { return std::to_string(value); }
        int GetValue() { return value; }
};

#endif // NUMBER_H
