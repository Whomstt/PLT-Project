%{
#include "parser_calc.tab.h"
#include <stdlib.h>
%}

%%

[0-9]+            { yylval.ival = atoi(yytext); return NUM; }   /* Match numbers */
"+"               { return PLUS; }
"-"               { return MINUS; }
"*"               { return MUL; }
"/"               { return DIV; }
"("               { return LP; }
")"               { return RP; }
"\n"              { return EOL; }                              /* Match newline */
[ \t]             { /* Ignore whitespace */ }
.                 { return yytext[0]; }                        /* Return unmatched characters */

%%

int yywrap(void) {
    return 1;
}
