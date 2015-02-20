#ifndef CONS_H
#define CONS_H

#include "Data.h"
#include "Number.h"
#include "Exception.h"

class Cons : public Data {
    protected:
        Data *left;
        Data *right; 
    public:
        Cons(Data *x, Data *y) : Data() { left = x; right = y; x->IncRefs(); y->IncRefs(); }
        ~Cons() { left->DecRefs(); right->DecRefs(); }
        bool IsCons() { return true; }
        virtual bool IsA(DataType T) { return T == CONS ? true : Data::IsA(T); }
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

#endif // CONS_H
