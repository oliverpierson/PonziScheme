#ifndef SYMBOL_H
#define SYMBOL_H

#include "Atom.h"

class Symbol : public Atom {
    private:
        std::string name;
    public:
        Symbol(std::string s) : Atom() { name = s; }
        void SelfDestruct() { /* don't delete symbols => do nothing */ }
        virtual bool IsA(DataType T) { return T == SYMBOL ? true : Atom::IsA(T); }
        std::string AsString() { return name; }
        Data* Eval(Environment* env);
};

#endif // SYMBOL_H
