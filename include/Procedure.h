#ifndef PROCEDURE_H
#define PROCEDURE_H

#include "Data.h"
#include "Exception.h"
#include <vector>

class Procedure : public Data {
    public:
        Procedure() : Data() { }
        virtual Data* Apply(Environment *env, std::vector<Data*> args) { throw 101; }
        //Data* Apply(Environment *, std::vector<Data*>);
        std::string AsString() { return std::string("#<function>"); }
        bool IsProcedure() { return true; }
        Data * Eval(Environment *env) { return this; }
};

class SchemeProcedure : public Procedure {
    private:
        Environment* environment;
        Data* code;
        std::vector<Symbol*> *args;
        SchemeProcedure(Environment *env, Data* code_, std::vector<Symbol*> *args_) : Procedure() 
        { 
            environment = env; code = code_; args = args_; 
            code->IncRefs(); 
            env->IncRefs();
        }
        ~SchemeProcedure() { delete args; code->DecRefs(); environment->DecRefs(); }
    public:
        Data* Apply(Environment *env, std::vector<Data*> args);
        static SchemeProcedure * MakeProcedure(Environment*, Cons*, Data*);
};

class PrimitiveProcedure : public Procedure {
    private:
        Environment * environment;
        Data * (*call_func)(std::vector<Data*>);
        PrimitiveProcedure(Environment * env, Data * (*f)(std::vector<Data*>)) : Procedure() { environment = env; call_func = f; }
    public:
        Data* Apply(Environment *env, std::vector<Data*> args);
        static PrimitiveProcedure * MakeProcedure(Environment*, Data * (*f)(std::vector<Data*>));
};

#endif // PROCEDURE_H
