%{
  #include "reader.h"
  #define YYSTYPE Data*
  #include "reader.tab.hpp"

  extern SymbolTable * symbol_table;
%}
%option noyywrap
%%

[ \t\n] { ; }
[a-zA-Z\+\-\*\\\=\?\.][a-zA-Z0-9\+\-\*\\\=\?]* { yylval = InternSymbol(yytext, symbol_table); return SYMBOL; }
[0-9]+ { yylval = MakeInt(yytext); return INTEGER; }
. { return yytext[0]; }

%%
