%{
#include "parser_tac.tab.h"
#include <string.h>
#include <stdlib.h>
%}

%%

"main"               { return MAIN; }
"int"                { return INT; }

[ \t\r\n]+           { /* Skip whitespace */ }

[0-9]+               { 
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

.                    { /* Ignore other single characters */ }

%%

int yywrap(void) { return 1; }
