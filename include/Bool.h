#ifndef BOOL_H
#define BOOL_H

#include "Atom.h"

class Bool : public Atom {
    private:
        bool value;
    public:
        Bool(bool b) : Atom() { value = b; }
        bool IsBool() { return true; }
        std::string AsString() { return std::to_string(value); }
        bool IsTrue() { return value ? true : false; }
        bool IsFalse() { return !IsTrue(); } 
};

#endif // BOOL_H
