#include "fuji.h"
#include <string>
#include <iostream>

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
    if( left->IsAtom() ) { 
        if( ((Atom*)left)->IsSymbol() ) {
            std::string head = left->AsString();
            if( head == "define" ){
               if( right->Car()->IsAtom() && ((Atom*)right->Car())->IsSymbol() ) {
                Frame *frame = env->Top();
                Symbol* name = (Symbol*)right->Car(); 
                Data* value = right->Cadr()->Eval(env);
                frame->AddBinding(name, value);
                return nil;
               } else throw new BadForm("Cons::Eval");
            } 
            else if( head == "quote" )
                return this->Cadr();
            else if( head == "lambda" ) {
                if( this->Cadr()->IsCons() )
                    return Procedure::MakeProcedure(env, (Cons*)this->Cadr(), this->Cddr()->Car());
                else throw new BadForm("Cons::Eval");
            }
            else if( left->Eval(env)->IsProcedure() ) {
                if( right->IsCons() )
                    return EvalProcedure((Procedure*)left->Eval(env), (Cons*)right, env);
                else throw new BadForm("Cons::Eval");
            }
        }
        throw new BadForm("Cons::Eval"); // unknown special-form
    } 
    // otherwise treat it as procedure application e.g. ((lambda (x) x) 3) => 3
    if( left->Eval(env)->IsProcedure() )
        return EvalProcedure((Procedure*)left, (Cons*)right, env);
    throw new BadForm("Cons::Eval"); // if nothing matches thrown an error
}

SymbolTable::~SymbolTable()
{
    for(std::map<std::string, Symbol*>::iterator it = symbols.begin(); it != symbols.end(); ++it) {
        delete it->second;
    }
}

Data* Procedure::Apply(Environment *current_env, std::vector<Data*> argvals)
{
    Frame *frame = new Frame();
    std::vector<Symbol*>::iterator symit = args->begin();
    std::vector<Data*>::iterator valit = argvals.begin();
    for( ; symit != args->end() && valit != argvals.end(); ++symit, ++valit )
        frame->AddBinding(*symit, *valit);
    environment->Push(frame);
    Data *return_data = code->Eval(environment);
    environment->Pop();
    delete frame;
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
    if( !frames.empty() ) {
        std::stack<Frame*> tempframes;
        Frame *fp = frames.top();
        while( !fp->BindingExists(symbol) ) {
            tempframes.push(frames.top());
            frames.pop();
            if( frames.empty() ) {
                // the binding doesn't exists so restore the environment and throw exception
                while( !tempframes.empty() ) {
                    frames.push(tempframes.top());
                    tempframes.pop();
                }
                throw new MissingBinding("Environment::LookupValue", symbol->AsString());    
            }
            fp = frames.top();
        }
        while( !tempframes.empty() ) {
            frames.push(tempframes.top());
            tempframes.pop();
        }
        return fp->LookupValue(symbol);
    }
    else {
        // environment contains no frames
        throw new MissingBinding("Environment::LookupValue", symbol->AsString());
    }
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

