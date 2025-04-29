%{
#include "tac.tab.h"
#include <string.h>
%}

%%

"main"               { return MAIN; }
"int"                { return INT; }

[ \t\r\n]+           { /* skip whitespace */ }

[0-9]+               {
                        /* copy the digits into sval */
                        yylval.sval = strdup(yytext);
                        return NUM;
                     }

[a-zA-Z_][a-zA-Z0-9_]* {
                        yylval.sval = strdup(yytext);
                        return ID;
                     }

"="                  { return EQUAL; }
"+"                  { return PLUS; }
"-"                  { return MINUS; }
"*"                  { return MUL; }
"/"                  { return DIV; }
";"                  { return SEMICOLON; }
","                  { return COMMA; }
"("                  { return LP; }
")"                  { return RP; }
"{"                  { return LBRACE; }
"}"                  { return RBRACE; }

.                    { /* ignore any other single char */ }

%%

int yywrap(void) { return 1; }
