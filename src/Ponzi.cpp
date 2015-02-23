#include "ponzi.h"
#include <string>
#include <iostream>
#include "boost/cast.hpp"

using namespace boost;

Data* Nil::Eval(Environment *env)
{
    return nil;
}

Data * EvalProcedure(Procedure * proc, Cons * arglist, Environment * env)
{
    std::vector<Data*> argvals;
    while( !arglist->IsNil() ) {
        argvals.push_back(arglist->Car()->Eval(env));
        arglist = (Cons*)arglist->Cdr();
    }
    return proc->Apply(env, argvals);
}

Data* Cons::Eval(Environment *env)
{
    std::string head = left->AsString();
    if( head == "if" ) {
        if( polymorphic_cast<Bool*>(this->Cadr()->Eval(env))->IsTrue() )
            return this->Cdr()->Cadr()->Eval(env);
        else return this->Cddr()->Cadr()->Eval(env);
    }
    else if( head == "define" ){
        if( this->Cadr()->IsA(SYM) ) {
            Symbol* name = polymorphic_cast<Symbol*>(this->Cadr()); 
            Data* value = right->Cadr()->Eval(env);
            if( env->BindingExists(name) )
                env->UpdateBinding(name, value);
            else 
                env->AddBinding(name, value);
            return value;
        } else if( this->Cadr()->IsA(CONS) ) {
           SchemeProcedure * newproc = SchemeProcedure::MakeProcedure(env, polymorphic_cast<Cons*>(this->Cadr()->Cdr()), this->Cddr() );
           Symbol * name = polymorphic_cast<Symbol*>(this->Cadr()->Car());
           if( env->BindingExists(name) )
               env->UpdateBinding(name, newproc);
           else
               env->AddBinding(name, newproc);
           return newproc;
           } 
    } 
    else if( head == "quote" )
        return this->Cadr();
    else if( head == "lambda" ) {
        if( this->Cadr()->IsA(CONS) )
            return SchemeProcedure::MakeProcedure(env, polymorphic_cast<Cons*>(this->Cadr()), this->Cddr());
    }
    else if( this->Car()->Eval(env)->IsA(PROC) ) {
        if( this->Cdr()->IsA(CONS) )
            return EvalProcedure((SchemeProcedure*)left->Eval(env), polymorphic_cast<Cons*>(right), env);
    }
    throw BADFORM; // unknown special-form
}

SymbolTable::~SymbolTable()
{
    for(std::map<std::string, Symbol*>::iterator it = symbols.begin(); it != symbols.end(); ++it) {
        delete it->second;
    }
}

Data* SchemeProcedure::Apply(Environment *current_env, std::vector<Data*> argvals)
{
    Environment *new_env = new Environment(environment);
    std::vector<Symbol*>::iterator symit = args->begin();
    std::vector<Data*>::iterator valit = argvals.begin();
    for( ; symit != args->end() && valit != argvals.end(); ++symit, ++valit )
        new_env->AddBinding(*symit, *valit);
    Data *return_data;
    Data *current_code = code;
    new_env->IncRefs();
    do {
        return_data = current_code->Car()->Eval(new_env);
        current_code = current_code->Cdr();
    } while( !current_code->IsNil() );
    new_env->DecRefs();
    return return_data;
}

SchemeProcedure * SchemeProcedure::MakeProcedure(Environment *env, Cons *arglist, Data *codelist)
{
    std::vector<Symbol*> *argvec = new std::vector<Symbol*>();
    while( !arglist->IsNil() ) {
        argvec->push_back((Symbol*)arglist->Car());
        arglist = polymorphic_cast<Cons*>(arglist->Cdr());
    }
    SchemeProcedure *proc = new SchemeProcedure(env, codelist, argvec);
    return proc;
}

PrimitiveProcedure * PrimitiveProcedure::MakeProcedure(Environment *env, Data * (*func)(std::vector<Data*>))
{
    return new PrimitiveProcedure(env, func);
}

Data * PrimitiveProcedure::Apply(Environment *env, std::vector<Data*> args)
{
    return call_func(args);
}
