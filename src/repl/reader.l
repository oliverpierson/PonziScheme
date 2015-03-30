%{
  #include "reader.h"
  #define YYSTYPE Data*
  #include "reader.tab.hpp"

  extern SymbolTable * symbol_table;
%}
%option noyywrap
%%

[ \t\n] { ; }
[a-zA-Z\+\-\*\\\=][a-zA-Z0-9\+\-\*\\\=\?]* { yylval = InternSymbol(yytext, symbol_table); return _SYMBOL; }
[0-9]+ { yylval = MakeInt(yytext); return _INTEGER; }
[0-9]*\.[0-9]+ { yylval = MakeFloat(yytext); return _FLOAT; }
. { return yytext[0]; }

%%
