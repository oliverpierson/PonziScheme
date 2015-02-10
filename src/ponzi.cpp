#include "ponzi.h"
#include <string>
#include <iostream>

Data* Nil::Eval(Environment *env)
{
    return nil;
}

Data * EvalProcedure(BareProcedure * proc, Cons * arglist, Environment * env)
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
    if( head == "if" && this->Cadr()->Eval(env)->IsBool() ) {
        if( ((Bool*)this->Cadr()->Eval(env))->IsTrue() )
            return this->Cdr()->Cadr()->Eval(env);
        else return this->Cddr()->Cadr()->Eval(env);
    }
    else if( head == "define" ){
        if( this->Cadr()->IsAtom() && ((Atom*)this->Cadr())->IsSymbol() ) {
            Symbol* name = (Symbol*)right->Car(); 
            Data* value = right->Cadr()->Eval(env);
            if( env->BindingExists(name) )
                env->UpdateBinding(name, value);
            else 
                env->AddBinding(name, value);
            return value;
       } else if( this->Cadr()->IsCons() ) { //right->Car()->IsCons() ) {
           Procedure * newproc = Procedure::MakeProcedure(env, (Cons*)this->Cadr()->Cdr(), this->Cddr());
           Symbol * name = (Symbol*)this->Cadr()->Car();
           if( env->BindingExists(name) )
               env->UpdateBinding(name, newproc);
           else
               env->AddBinding(name, newproc);
           return newproc;
       } else throw new BadForm("Cons::Eval");
    } 
    else if( head == "quote" )
        return this->Cadr();
    else if( head == "lambda" ) {
        if( this->Cadr()->IsCons() )
            return Procedure::MakeProcedure(env, (Cons*)this->Cadr(), this->Cddr());
        else throw new BadForm("Cons::Eval");
    }
    else if( this->Car()->Eval(env)->IsProcedure() ) {
        if( this->Cdr()->IsCons() )
            return EvalProcedure((Procedure*)left->Eval(env), (Cons*)right, env);
        else throw new BadForm("Cons::Eval");
    }
    throw new BadForm("Cons::Eval"); // unknown special-form
}

SymbolTable::~SymbolTable()
{
    for(std::map<std::string, Symbol*>::iterator it = symbols.begin(); it != symbols.end(); ++it) {
        delete it->second;
    }
}

Data* Procedure::Apply(Environment *current_env, std::vector<Data*> argvals)
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

Data* Frame::LookupValue(Symbol *symbol) 
{ 
    if( !BindingExists(symbol) )
        throw new MissingBinding("Frame::Lookup", symbol->AsString());
    return bindings[symbol]; 
}

Data* Environment::LookupValue(Symbol *symbol)
{ 
    if( BindingExists(symbol) )
        return Frame::LookupValue(symbol);
    else if( base_env != NULL )
        return base_env->LookupValue(symbol);
    else throw new MissingBinding("Environment::LookupValue", symbol->AsString());
}

Procedure * Procedure::MakeProcedure(Environment *env, Cons *arglist, Data *codelist)
{
    std::vector<Symbol*> *argvec = new std::vector<Symbol*>();
    while( !arglist->IsNil() ) {
        argvec->push_back((Symbol*)arglist->Car());
        arglist = (Cons*)arglist->Cdr();
    }
    Procedure *proc = new Procedure(env, codelist, argvec);
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
