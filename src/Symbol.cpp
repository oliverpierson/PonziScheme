#include "Symbol.h"

Data* Symbol::Eval(Environment* env) 
{ 
    return env->LookupValue(this); 
}

