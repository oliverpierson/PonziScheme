#include <iostream>
#include "ponzi.h"
#include <assert.h>
#include <stdlib.h>

using namespace std;

int main(void)
{
    Cons *list = new Cons(Number::Integer(0), nil);
    for(int i = 1; i <= 3; i++) 
        list = new Cons(Number::Integer(i), list);
    list = new Cons(new Symbol(string("scheme!")), list);
    while( !list->IsNil() ) {
        list = (Cons*)list->Cdr();
    }
   
    // test MissingBinding exception 
    Symbol x(string("x"));
    Environment parent_env;
    Environment child_env(&parent_env);
    parent_env.AddBinding(&x, Number::Integer(10));
    child_env.AddBinding(&x, new Symbol(string("ten")));
    Symbol y(string("y"));
    Data *d;
    try {
        d = child_env.LookupValue(&y);
        cout << "LookupValue test failed." << endl;
        exit(EXIT_FAILURE);
    } catch (Exception e) {
        assert(e == MISSINGBINDING);
        cout << "MissingBinding exception test passed." << std::endl;
    }

    // test variable name evaluation
    assert(x.Eval(&parent_env)->AsString() == "10");
    cout << "Variable lookup test passed." << endl;

    // test SymbolTable interning
    SymbolTable *st = new SymbolTable();
    Symbol* a1 = st->InternSymbol("a");
    Symbol* b = st->InternSymbol("b");
    Symbol* a2 = st->InternSymbol("a");
    assert(a1 == a2);
    assert(a1 != b);
    cout << "Symbol interning test passed." << endl;
    
    // test NotCons exception
    try {
        Symbol *s = new Symbol("s");
        s->Car();
        exit(EXIT_FAILURE);
    } catch (Exception e) {
        assert(e == NOTCONS);
        cout << "NotCons exception test 1 passed." << endl;
    }
    try {
        nil->Cadr();    
        exit(EXIT_FAILURE);
    } catch (Exception e) {
        assert(e == NOTCONS);
        cout << "NotCons exception test 2 passed." << endl;
    }
    try {
        Cons *cons = new Cons(Number::Integer(1), Number::Integer(2));
        cons->Cadr();
        exit(EXIT_FAILURE);
    } catch (Exception e) {
        assert(e == NOTCONS);
        cout << "NotCons exception test 3 passed." << endl;
    }


    // test numeric tower
    Number* one = Number::Integer(1);
    Number* two = Number::Integer(2);
    Number* pi = Number::Float(3.14);
    std::cout << (one->Plus(two))->AsString() << std::endl;
    std::cout << (one->Plus(pi))->AsString() << std::endl;
    std::cout << ((one->Plus(pi))->Plus(two))->AsString() << std::endl;

    cout << __FILE__ << ": all tests passed." << endl;
    exit(EXIT_SUCCESS);
}
