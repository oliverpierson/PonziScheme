#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include <string>
#include "Symbol.h"

class SymbolTable {
    private:
        std::map<std::string, Symbol*> symbols;
    public:
        SymbolTable() { }
        ~SymbolTable();
        Symbol* InternSymbol(std::string name)
        {
            if( symbols.find(name) == symbols.end() ) 
                symbols[name] = new Symbol(name);
            return symbols[name];
        }
};

#endif // SYMBOLTABLE_H
