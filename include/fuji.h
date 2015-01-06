#include <iostream>
#include <string>
#include <stack>
#include <map>
#include <vector>

#ifndef FUJI_H 
#define FUJI_H 

class Exception {
    private:
        std::string error;
        std::string message;
        std::string caller;
    public:
        Exception(const char *caller_, const std::string message_) { message = message_; caller = caller_; }
        std::string GetMessage() { return message; }
};

/* Data Types */
class Environment; // Need to declare this early for Data::Eval()
class Data { 
    private:
        int refs;
    public:
        Data() { refs = 0; }
        virtual Data* Eval(Environment* env) { return NULL; } // throw error
        virtual std::string AsString() { return std::string("Error"); } // throw error if this is reached 
        int deref() { if(refs > 0) refs--; return refs; } //else throw error 

        // some builtins -- should be a predicate for each builtin datatype
        virtual bool IsAtom() { return false; }
        virtual bool IsCons() { return false; }
        virtual bool IsProcedure() { return false; }
};

class Symbol; // Need to declare this early for classees Binding, Frame, Environment
class Binding {
    private:
        Symbol *name;
        Data *value;
    public:
        Binding(Symbol *n, Data* v) { name = n; value = v; }
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
    public:
        void AddBinding(Symbol *s, Data *v) { bindings[s] = v; }
        bool BindingExists(Symbol *symbol)
        {
            if( bindings.find(symbol) == bindings.end() )
                return false;
            else return true;
        }
        Data* LookupValue(Symbol *symbol);
};

class Environment {
    private:
        std::stack<Frame*> frames;
    public:
        Frame* Pop() { Frame *retval = frames.top(); frames.pop(); return retval; }
        Frame* Top() { return frames.top(); }
        void Push(Frame* f) { frames.push(f); }
        Data* LookupValue(Symbol *symbol) 
        { 
            if( !frames.empty() ) {
                std::stack<Frame*> tempframes;
                Frame *fp = frames.top();
                while( !fp->BindingExists(symbol) ) {
                    tempframes.push(frames.top());
                    frames.pop();
                    if( frames.empty() )
                        break;
                    fp = frames.top();
                }
                while( !tempframes.empty() ) {
                    frames.push(tempframes.top());
                    tempframes.pop();
                }
                return fp->LookupValue(symbol);
            } else {
                std::cout << "Failed to lookup " << std::endl;
                throw 10;
            }
        }
};

class Cons : public Data {
    protected:
        Data *left;
        Data *right; 
    public:
        Cons(Data *x, Data *y) : Data() { left = x; right = y; }
        bool IsCons() { return true; }
        virtual bool IsNil() { return false; }
        std::string AsString() { return "(cons " + left->AsString() + " " + right->AsString() + ")"; }
        Data* Eval(Environment*);
        Data* Car() { return left; }
        Data* Cdr() { return right; }
        Data* Cadr() { return ((Cons*)right)->Car(); }
        Data* Cddr() { return ((Cons*)right)->Cdr(); }
};

class Nil : public Cons {
    public:
        Nil() : Cons(NULL, NULL) { }
        bool IsNil() { return true; }
        std::string AsString() { return std::string("()"); }
        Data* Eval(Environment*);
        Data* Car() { throw 3; }
        Data* Cdr() { throw 3; }
        Data* Cadr() { throw 3; }
        Data* Cddr() { throw 3; }
};

Nil *const nil = new Nil();

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
};

class Symbol : public Atom {
    private:
        std::string name;
    public:
        Symbol(std::string s) : Atom() { name = s; }
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
};

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
class Procedure : public Data {
    private:
        Environment* environment;
        Data* code;
        std::vector<Symbol*> *args;
    public:
        Procedure(Environment *env, Data* code_, std::vector<Symbol*> *args_) { environment = env; code = code_; args = args_; }
        Data* Apply(Environment *env, std::vector<Data*> args);
        std::string AsString() { return std::string("#<function>"); }
        bool IsProcedure() { return true; }
        Data* Eval(Environment *env) { return this; }
};


#endif // FUJI_H
