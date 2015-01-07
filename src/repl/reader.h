#ifndef __READER_H__
#define __READER_H__

#include <stdio.h>
#include "fuji.h"

#define INT long int

#define NIL nil

/* functions for the parser */
Data * cons(Data *, Data *);

/* functions for the lexical analyzer */
Data * makeInt(char *);
Data * internSymbol(char *, SymbolTable *);

#endif /* __READER_H__ */
