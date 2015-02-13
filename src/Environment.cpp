#include "Environment.h"

Frame::~Frame()
{
    for(std::map<Symbol*, Data*>::iterator it = bindings.begin();
        it != bindings.end(); ++it) {
        it->second->DecRefs();
    }
}

void Frame::AddBinding(Symbol *s, Data *v)
{
    if( !BindingExists(s) ) {
        bindings[s] = v;
        v->IncRefs();
    } else throw 1234;
} 

void Frame::UpdateBinding(Symbol *s, Data *v)
{
    if( BindingExists(s) ) {
        bindings[s]->DecRefs();
        bindings[s] = v;
        v->IncRefs();
    } else throw 1234;
}

bool Frame::BindingExists(Symbol *symbol)
{
    if( bindings.find(symbol) == bindings.end() )
        return false;
    else return true;
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

Environment::Environment(Environment *base_) : base_env(base_) 
{ 
    refs = 0; base_env->IncRefs(); 
}

Environment::~Environment() 
{ 
    if( base_env != NULL ) base_env->DecRefs(); 
}


