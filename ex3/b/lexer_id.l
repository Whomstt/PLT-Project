%{
#include "ident.tab.h"
%}
%%
[a-zA-Z][a-zA-Z0-9]*   { return ID; }
[ \t\r]+               { /* skip */ }
\n                      { return EOL; }
.                       { return yytext[0]; }
%%
