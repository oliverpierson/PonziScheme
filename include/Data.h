#ifndef DATA_H 
#define DATA_H

#include "ManagedMemory.h"
#include "Environment.h"
#include "Exception.h"

// Note that the ordering of the numeric types here
// is important because it reflects the hierarchy of
// the numeric tower, e.g. FLOAT > INTEGER 
typedef enum
{
    ATOM,
    CONS,
    BOOL,
    NUMBER,
    INTEGER,
    FLOAT,
    SYMBOL,
    PROC
} DataType;

class Environment; // Forward declaration

class Data : public ManagedMemory { 
    private:
        int refs;
    public:
        Data() { refs = 0; }
        virtual Data* Eval(Environment* env) { return NULL; } // throw error
        virtual std::string AsString() { return std::string("Error"); } // throw error if this is reached 
        virtual bool Equals(Data*, Data*) { return false; }
        void IncRefs() { ++refs; }
        void DecRefs() 
        { 
            if(refs > 0) refs--;  //else throw error 
            if(refs == 0) this->SelfDestruct();
        }
        void SelfDestruct() { delete this; }
        int GetRefs() { return refs; }

        virtual bool IsNil() { return false; } 
        virtual bool IsA(DataType) { return false; }

        virtual Data * Car() { throw NOTCONS; } // new NotCons("Data::Car", this->AsString()); }
        virtual Data * Cdr() { throw NOTCONS; } // new NotCons("Data::Cdr", this->AsString()); }
        virtual Data * Cadr() { throw NOTCONS; } // new NotCons("Data::Cadr", this->AsString()); }
        virtual Data * Cddr() { throw NOTCONS; } // new NotCons("Data::Cddr", this->AsString()); }
};

#endif // DATA_H

