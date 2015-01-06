#include "fuji.h"
#include <string>
#include <iostream>

Data* Nil::Eval(Environment *env)
{
    return nil;
}

Data* Cons::Eval(Environment *env)
{

    // TODO probably should check that this cons is a list first
    if( left->IsAtom() ) { 
        if( ((Atom*)left)->IsSymbol() ) {
            if( left->AsString() == std::string("define") ) {
                Frame *frame = env->Top();
                Symbol* name = (Symbol*)((Cons*)right)->Car();
                Data* value = ((Cons*)right)->Cadr()->Eval(env);
                frame->AddBinding(name, value);
                return nil;
            }
            else if( left->AsString() == std::string("quote") ) {
                return this->Cadr();
            }
            else if (left->AsString() == std::string("lambda") ) {
                //make procedue -- maybe move to Procedure class and add delete method
                Cons *arglist = (Cons*)this->Cadr();
                std::vector<Symbol*> *args = new std::vector<Symbol*>();
                Data* code = ((Cons*)this->Cddr())->Car();
                while( !arglist->IsNil() ) {
                    args->push_back((Symbol*)arglist->Car());
                    arglist = (Cons*)arglist->Cdr();
                }
                Procedure *proc = new Procedure(env, code, args);
                return proc;
            }
        }
    }
    //else: function application
    Procedure *proc = (Procedure*)left->Eval(env);
    std::vector<Data*> argvals;
    Cons* arglist = (Cons*)right;
    while( !arglist->IsNil() ) {
        argvals.push_back(arglist->Car()->Eval(env));
        arglist = (Cons*)arglist->Cdr();
    }
    return proc->Apply(env, argvals);
}

SymbolTable::~SymbolTable()
{
    for(std::map<std::string, Symbol*>::iterator it = symbols.begin(); it != symbols.end(); ++it) {
        delete it->second;
    }
}

Data* Procedure::Apply(Environment *env, std::vector<Data*> argvals)
{
    Frame *frame = new Frame();
    std::vector<Symbol*>::iterator symit = args->begin();
    std::vector<Data*>::iterator valit = argvals.begin();
    for( ; symit != args->end() && valit != argvals.end(); ++symit, ++valit )
        frame->AddBinding(*symit, *valit);
    env->Push(frame);
    Data *return_data = code->Eval(env);
    env->Pop();
    delete frame;
    return return_data;
}

Data* Frame::LookupValue(Symbol *symbol) 
{ 
    if( !BindingExists(symbol) )
        throw new MissingBinding("Frame::Lookup", symbol->AsString());
    return bindings[symbol]; 
}
