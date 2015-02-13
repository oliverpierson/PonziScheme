#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "Exception.h"
#include "ManagedMemory.h"
#include "Data.h"
#include "Symbol.h"
#include <map>

class Symbol;
class Data;

class Frame {
    private:
        std::map<Symbol*, Data*> bindings;
        int refs;
    public:
        ~Frame();
        void AddBinding(Symbol *s, Data *v);
        void UpdateBinding(Symbol *s, Data *v);
        bool BindingExists(Symbol *symbol);
        Data* LookupValue(Symbol *symbol);
};

class Environment : private Frame, public ManagedMemory {
    private:
        int refs;
        Environment *base_env;
    public:
        Environment() { refs = 0; base_env = NULL; }
        Environment(Environment *base_);
        ~Environment();
        void IncRefs() { ++refs; }
        void DecRefs() { --refs; if( refs == 0 ) SelfDestruct(); }
        void SelfDestruct() { delete this; }
        void AddBinding(Symbol *sym, Data *value) { Frame::AddBinding(sym, value); }
        void UpdateBinding(Symbol *sym, Data *value) { Frame::UpdateBinding(sym, value); }
        bool BindingExists(Symbol *s) { return Frame::BindingExists(s); }
        Data* LookupValue(Symbol *symbol);

};

#endif // ENVIRONMENT_H
