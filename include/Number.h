#ifndef NUMBER_H
#define NUMBER_H

#include "boost/cast.hpp"
#include "Atom.h"
#include "Bool.h"

using namespace boost;

class Number : public Atom {
    protected:
        union {
            int i;
            float f;
        } value;
        DataType T;
        Number() : Atom() { }
    public:
        static Number* Integer(int ival) { Number* n = new Number(); n->value.i = ival; n->T = INTEGER; return n; }
        static Number* Float(float fval) { Number* n = new Number(); n->value.f = fval; n->T = FLOAT; return n; }
        bool IsA(DataType S) { return T == S ? true : Atom::IsA(S); }
        DataType Type() { return T; }
        std::string AsString()
        {
            switch(T) {
                case INTEGER:
                    return std::to_string(value.i);
                case FLOAT:
                    return std::to_string(value.f);
                default:
                    throw INTERNAL_ERROR;
            }
        }

        Bool * Equals(Number *that)
        {
            if( this->Type() > that->Type() ) {
                return new Bool((float)that->value.i == this->value.f);
            } else if ( that->Type() > this->Type() ) {
                return new Bool((float)this->value.i == that->value.f);
            } else {
                if( that->IsA(FLOAT) )
                    return new Bool(that->value.f == this->value.f);
                else // that->IsA(INTEGER)
                    return new Bool(that->value.i == this->value.i);
            }
        }

        Number * Plus(Number *that)
        {
            if( this->Type() > that->Type() ) {
                return Number::Float((float)that->value.i + this->value.f);
            } else if ( that->Type() > this->Type() ) {
                return Number::Float((float)this->value.i + that->value.f);
            } else {
                if( that->IsA(FLOAT) )
                    return Number::Float(that->value.f + this->value.f);
                else // that->IsA(INTEGER)
                    return Number::Integer(that->value.i + this->value.i);
            }
        }

        Number * Minus(Number *that)
        {
            if( this->Type() > that->Type() ) {
                return Number::Float(this->value.f - (float)that->value.i);
            } else if ( that->Type() > this->Type() ) {
                return Number::Float((float)this->value.i - that->value.f);
            } else {
                if( that->IsA(FLOAT) )
                    return Number::Float(this->value.f - that->value.f);
                else // that->IsA(INTEGER)
                    return Number::Integer(this->value.i - that->value.i);
            }
        }

        Number * Mul(Number *that)
        {
            if( this->Type() > that->Type() ) {
                return Number::Float((float)that->value.i * this->value.f);
            } else if ( that->Type() > this->Type() ) {
                return Number::Float((float)this->value.i * that->value.f);
            } else {
                if( that->IsA(FLOAT) )
                    return Number::Float(that->value.f * this->value.f);
                else // that->IsA(INTEGER)
                    return Number::Integer(that->value.i * this->value.i);
            }
        }

};

#endif // NUMBER_H
