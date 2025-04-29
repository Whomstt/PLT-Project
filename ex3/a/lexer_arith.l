%{
#include "arith.tab.h"
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
.           { return yytext[0]; }
%%
