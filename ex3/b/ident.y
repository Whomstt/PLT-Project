%{
#include <stdio.h>
void yyerror(const char *s) { /* no default message */ }
%}

%token ID EOL
%start input

%%

input
  : ID EOL        { printf("Valid Identifier\n"); }
  | error EOL     { printf("Error. Failed to parse.\n"); }
  ;

%%

int main(void) {
  yyparse();
  return 0;
}
