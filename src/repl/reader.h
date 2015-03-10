#ifndef __READER_H__
#define __READER_H__

#include <stdio.h>
#include "ponzi.h"

#define INT long int

#define NIL nil

/* functions for the parser */
//Data * cons(Data *, Data *);

/* functions for the lexical analyzer */
Data * InternSymbol(char *, SymbolTable *);
Data * MakeInt(char *);

#endif /* __READER_H__ */
