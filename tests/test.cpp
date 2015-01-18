#include <iostream>
#include "ponzi.h"
#include <assert.h>

using namespace std;

Data* f(vector<Data*> args)
{
    if( !args.empty() ) {
        if( args[0]->IsAtom() && args[1]->IsAtom() ) {
            if( ((Atom*)args[0])->IsNumber() && ((Atom*)args[1])->IsNumber() ) {
                Data * new_num = new Number(((Number*)args[0])->GetValue() + ((Number*)args[1])->GetValue());
                return new_num;
            }
        }
    }
    throw 987;
}

int main(void)
{
    Cons *list = new Cons(new Number(0), nil);
    for(int i = 1; i <= 3; i++) 
        list = new Cons(new Number(i), list);
    list = new Cons(new Symbol(string("scheme!")), list);
    cout << list->AsString() << endl;
    //while( list->Cdr() != nil ) {
    while( !list->IsNil() ) {
        list = (Cons*)list->Cdr();
    }
    
    Symbol x(string("x"));
    Frame frame1, frame2;
    frame1.AddBinding(&x, new Number(10));
    frame2.AddBinding(&x, new Symbol(string("ten")));
    assert(frame1.LookupValue(&x)->AsString() == string("10"));
    assert(frame2.LookupValue(&x)->AsString() == string("ten"));

    cout << "First tests passed. " << endl;
    Environment env;
    env.Push(&frame2);
    env.Push(&frame1);
    Symbol y(string("y"));
    Data *d;
    try {
        d = env.LookupValue(&y);
    } catch (MissingBinding *e) {
        cout << "Unbound variable: " << e->GetMessage() << ", caller: " << e->GetCaller() << endl;
        Frame *frame3 = new Frame();
        frame3->AddBinding(&y, new Number(12345));
        env.Push(frame3);
        d = env.LookupValue(&y);
    }
    assert(d->AsString() == string("12345"));

    try {
       frame1.LookupValue(new Symbol("xyz"));
    } catch (MissingBinding *e) {
        cout << "Unbound variable: " << e->GetMessage() << ", caller: " << e->GetCaller() << endl;
    }

    assert(x.Eval(&env)->AsString() == "10");


    Symbol* z = new Symbol(string("z"));
    Data* i987 = new Number(987);
    list = new Cons(new Symbol(string("define")), new Cons(z, new Cons(i987, nil)));
    cout << "here." << endl;
    list->Eval(&env);
    cout << "here." << endl;
    list->Eval(&env); // this second call shouldn't increase the reference count
    cout << z->Eval(&env)->AsString() << endl;
    cout << "987 has " << i987->GetRefs() << " references." << endl;
    
    list = new Cons(new Symbol(string("quote")), new Cons(new Symbol(string("blah")), nil));
    cout << list->Eval(&env)->AsString() << endl;
    
    SymbolTable *st = new SymbolTable();
    Symbol* a1 = st->InternSymbol("a");
    Symbol* b = st->InternSymbol("b");
    Symbol* a2 = st->InternSymbol("a");
    assert(a1 == a2);
    assert(a1 != b);
    
    list = new Cons(new Symbol(string("lambda")), new Cons( new Cons(b, nil), new Cons(b, nil)));
    cout << list->AsString() << endl;
    cout.flush();
    assert(nil->IsNil());
    Cons* c = nil;
    assert(c->IsNil());
    
    Procedure *proc = ((Procedure*)list->Eval(&env));
    list = new Cons(proc, new Cons(new Number(100), nil));
    cout << "return value is : " << list->Eval(&env)->AsString() << endl;

    // test NotCons exception
    try {
        Symbol *s = new Symbol("s");
        s->Car();
    } catch (NotCons *e) {
        cout << e->GetMessage() << " is not a cons, caller: " << e->GetCaller() << endl;
    }
    try {
        nil->Cadr();    
    } catch (NotCons *e) {
        cout << e->GetMessage() << " is not a cons, caller: " << e->GetCaller() << endl;
    }
    try {
        Cons *cons = new Cons(new Number(1), new Number(2));
        cons->Cadr();
    } catch (NotCons *e) {
        cout << e->GetMessage() << " is not a cons, caller: " << e->GetCaller() << endl;
    }

    PrimitiveProcedure *prim = PrimitiveProcedure::MakeProcedure(&env, f);
    std::vector<Data*> some_nums;
    some_nums.push_back(new Number(1));
    some_nums.push_back(new Number(2));
    cout << "PrimitiveProcedure prim returns : " << prim->Apply(&env, some_nums)->AsString() << endl;
   
    cout << __FILE__ << ": all tests passed." << endl;
    exit(0);
}
