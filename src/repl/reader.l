%{
  #include "reader.h"
  #define YYSTYPE Data*
  #include "reader.tab.hpp"

  extern SymbolTable * symbol_table;
%}
%option noyywrap
%%

[ \t\n] { ; }
[a-zA-Z\+\-\*\\\=\?][a-zA-Z0-9\+\-\*\\\=\?]* { yylval = internSymbol(yytext, symbol_table); return SYMBOL; }
[0-9]+ { yylval = makeInt(yytext); return INTEGER; }
. { return yytext[0]; }

%%
