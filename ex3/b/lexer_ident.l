%{
#include "parser_ident.tab.h"
#include <stdlib.h>
%}

%%

[a-zA-Z][a-zA-Z0-9]*   { return ID; }   /* Match identifiers */
[ \t\r]+               { /* Skip whitespace */ }
\n                     { return EOL; }  /* Match newline as end-of-line */
.                      { return yytext[0]; } /* Match any unmatched character */

%%

int yywrap(void) {
  return 1;
}
