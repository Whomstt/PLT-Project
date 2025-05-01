%{
#include "parser_arith.tab.h"
#include <stdlib.h>
%}

%%

[0-9]+      { yylval.ival = atoi(yytext); return NUM; }
[ \t\r]+    { /* skip whitespace */ }
\n          { return EOL; }
"+"         { return PLUS; }
"-"         { return MINUS; }
"*"         { return MUL; }
"/"         { return DIV; }
"("         { return LP; }
")"         { return RP; }
.           { return yytext[0]; } /* return any unmatched character */
%%

int yywrap(void) {
  return 1;
}
