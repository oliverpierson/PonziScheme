#ifndef ATOM_H
#define ATOM_H

#include "Data.h"

class Environment; // Forward declaration

class Atom : public Data {
    public:
        Atom() : Data() { }
        virtual bool IsA(DataType T) { return T == ATOM ? true : Data::IsA(T); }
        Data* Eval(Environment* env) { return this; } // Atoms are mostly self-evaluating except symbols?
};

#endif // ATOM_H
