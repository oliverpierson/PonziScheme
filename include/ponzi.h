#include <iostream>
#include <string>
#include <stack>
#include <map>
#include <vector>
#include <deque>

#ifndef PONZI_H
#define PONZI_H

class ManagedMemory {
    public:
        virtual void IncRefs() = 0;
        virtual void DecRefs() = 0;
        virtual void SelfDestruct() = 0;
        virtual ~ManagedMemory() {}
};

class Exception {
    private:
        std::string error;
        std::string message;
        std::string caller;
    public:
        Exception(const char *caller_, const std::string message_) { message = message_; caller = caller_; }
        std::string GetMessage() { return message; }
        std::string GetCaller() { return caller; }
};

class NotCons : public Exception {
    private:
        std::string error = "Attempted list access on non-list data.";
    public:
        NotCons(const char * caller_, const std::string message_) : Exception(caller_, message_) { }
}; 

class BadForm : public Exception {
    private:
        std::string error = "Badly formed input detected.";
    public:
        BadForm(const char * caller_) : Exception(caller_, "Badly formed expression detected") {}
};

/* Data Types */
class Environment; // Need to declare this early for Data::Eval()
class Data : public ManagedMemory { 
    private:
        int refs;
    public:
        Data() { refs = 0; }
        virtual Data* Eval(Environment* env) { return NULL; } // throw error
        virtual std::string AsString() { return std::string("Error"); } // throw error if this is reached 
        virtual bool Equals(Data*, Data*) { return false; }
        void IncRefs() { ++refs; }
        void DecRefs() 
        { 
            if(refs > 0) refs--;  //else throw error 
            if(refs == 0) this->SelfDestruct();
        }
        void SelfDestruct() { std::cout << "deleting data..." << this->AsString() << std::endl; delete this; }
        int GetRefs() { return refs; }

        // some builtins -- should be a predicate for each builtin datatype
        virtual bool IsAtom() { return false; }
        virtual bool IsCons() { return false; }
        virtual bool IsProcedure() { return false; }
        virtual bool IsNil() { return false; } 
        virtual bool IsBool() { return false; }

        virtual Data * Car() { throw new NotCons("Data::Car", this->AsString()); }
        virtual Data * Cdr() { throw new NotCons("Data::Cdr", this->AsString()); }
        virtual Data * Cadr() { throw new NotCons("Data::Cadr", this->AsString()); }
        virtual Data * Cddr() { throw new NotCons("Data::Cddr", this->AsString()); }
};

class Symbol; // Need to declare this early for classees Binding, Frame, Environment
class Binding {
    private:
        Symbol *name;
        Data *value;
    public:
        Binding(Symbol *n, Data* v) { name = n; value = v; value->IncRefs(); }
        void SetValue(Data* v) { value = v; }
};

class MissingBinding : public Exception {
    private:
        std::string error = "Unbound symbol";
    public:
        MissingBinding(const char * caller_, const std::string message_) : Exception(caller_, message_) { }
};

class Frame {
    private:
        std::map<Symbol*, Data*> bindings;
        int refs;
    public:
        ~Frame()
        {
            for(std::map<Symbol*, Data*>::iterator it = bindings.begin();
                it != bindings.end(); ++it) {
                std::cout << "DecRef " << it->second->AsString() << std::endl;
                it->second->DecRefs();
            }
        }
        void AddBinding(Symbol *s, Data *v)
        {
            if( !BindingExists(s) ) {
                bindings[s] = v;
                v->IncRefs();
            } else throw 1234;
        } 
        void UpdateBinding(Symbol *s, Data *v)
        {
            if( BindingExists(s) ) {
                bindings[s]->DecRefs();
                bindings[s] = v;
                v->IncRefs();
            } else throw 1234;
        }
        bool BindingExists(Symbol *symbol)
        {
            if( bindings.find(symbol) == bindings.end() )
                return false;
            else return true;
        }
        Data* LookupValue(Symbol *symbol);
};

class Environment : private Frame, public ManagedMemory {
    private:
        int refs;
        Environment *base_env;
    public:
        Environment() { refs = 0; base_env = NULL; }
        Environment(Environment *base_) : base_env(base_) { refs = 0; base_env->IncRefs(); }
        ~Environment() { if( base_env != NULL ) base_env->DecRefs(); }
        void IncRefs() { ++refs; }
        void DecRefs() { --refs; if( refs == 0 ) SelfDestruct(); }
        void SelfDestruct() { std::cout << "deleting environment... " << std::endl; delete this; }
        void AddBinding(Symbol *sym, Data *value) { Frame::AddBinding(sym, value); }
        void UpdateBinding(Symbol *sym, Data *value) { Frame::UpdateBinding(sym, value); }
        bool BindingExists(Symbol *s) { return Frame::BindingExists(s); }
        Data* LookupValue(Symbol *symbol);

};

class Cons : public Data {
    protected:
        Data *left;
        Data *right; 
    public:
        Cons(Data *x, Data *y) : Data() { left = x; right = y; x->IncRefs(); y->IncRefs(); }
        ~Cons() { left->DecRefs(); right->DecRefs(); std::cout << "cons deleted." << std::endl;}
        bool IsCons() { return true; }
        std::string AsString() 
        {
            std::string list_str("(");
            for(Data * it = this; !it->IsNil(); it = it->Cdr() ) { 
                list_str += it->Car()->AsString() + " ";
                if( it->Cdr()->IsAtom() ) { // in this case, this is a cons cell not a list
                    list_str += ". ";
                    list_str += it->Cdr()->AsString() + " ";
                    break;
                }
            }
            list_str.pop_back(); // remove trailing space after last element in the list
            list_str += ")";
            return list_str;
        }
        Data* Eval(Environment*);
        Data* Car() { return left; }
        Data* Cdr() { return right; }
        Data* Cadr() { return right->Car(); }
        Data* Cddr() { return right->Cdr(); }
};

class Atom : public Data {
    public:
        Atom() : Data() { }
        virtual bool IsAtom() { return true; }
        virtual bool IsSymbol() { return false; }
        virtual bool IsNumber() { return false; }
        virtual bool IsBool() { return false; }
        Data* Eval(Environment* env) { return this; } // Atoms are mostly self-evaluating except symbols?
};

class Number : public Atom {
    private:
        int value;
    public:
        Number(int i) : Atom() { value = i; }
        bool IsNumber() { return true; }
        std::string AsString() { return std::to_string(value); }
        int GetValue() { return value; }
};

class Symbol : public Atom {
    private:
        std::string name;
    public:
        Symbol(std::string s) : Atom() { name = s; }
        void SelfDestruct() { std::cout << "symbol not deleted... " << std::endl; /* don't delete symbols => do nothing */ }
        bool IsSymbol() { return true; }
        std::string AsString() { return name; }
        Data* Eval(Environment* env) { return env->LookupValue(this); }
};

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

class Nil : public Cons {
    public:
        Nil() : Cons(new Number(0), new Number(0)) { }
        bool IsNil() { return true; }
        std::string AsString() { return std::string("()"); }
        Data* Eval(Environment*);
        Data * Car() { throw new NotCons("Data::Car", this->AsString()); }
        Data * Cdr() { throw new NotCons("Data::Cdr", this->AsString()); }
        Data * Cadr() { throw new NotCons("Data::Cadr", this->AsString()); }
        Data * Cddr() { throw new NotCons("Data::Cddr", this->AsString()); }
};

Nil *const nil = new Nil();

/* Symbol Table */
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

/* Procedure data structures */
class BareProcedure : public Data {
    public:
        BareProcedure() : Data() { }
        virtual Data* Apply(Environment *env, std::vector<Data*> args) { throw 101; }
        //Data* Apply(Environment *, std::vector<Data*>);
        std::string AsString() { return std::string("#<function>"); }
        bool IsProcedure() { return true; }
        Data * Eval(Environment *env) { return this; }
};

class Procedure : public BareProcedure {
    private:
        Environment* environment;
        Data* code;
        std::vector<Symbol*> *args;
        Procedure(Environment *env, Data* code_, std::vector<Symbol*> *args_) : BareProcedure() 
        { 
            environment = env; code = code_; args = args_; 
            code->IncRefs(); 
            env->IncRefs();
        }
        ~Procedure() { delete args; code->DecRefs(); environment->DecRefs(); }
    public:
        Data* Apply(Environment *env, std::vector<Data*> args);
        static Procedure * MakeProcedure(Environment*, Cons*, Data*);
};

class PrimitiveProcedure : public BareProcedure {
    private:
        Environment * environment;
        Data * (*call_func)(std::vector<Data*>);
        PrimitiveProcedure(Environment * env, Data * (*f)(std::vector<Data*>)) : BareProcedure() { environment = env; call_func = f; }
    public:
        Data* Apply(Environment *env, std::vector<Data*> args);
        static PrimitiveProcedure * MakeProcedure(Environment*, Data * (*f)(std::vector<Data*>));
};

#endif // PONZI_H 
