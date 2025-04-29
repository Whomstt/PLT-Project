%{
#include "calc.tab.h"  // Include the Bison header
%}

%%

[0-9]+            { yylval.ival = atoi(yytext); return NUM; }
"+"               { return PLUS; }
"-"               { return MINUS; }
"*"               { return MUL; }
"/"               { return DIV; }
"("               { return LP; }
")"               { return RP; }
"\n"              { return EOL; }
[ \t]             { /* Ignore whitespace */ }
.                 { return yytext[0]; }

%%
