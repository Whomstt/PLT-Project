%{
#include <stdio.h>
#include <stdlib.h>
void yyerror(const char *s) { fprintf(stderr, "Error: %s\n", s); }
%}

%token ID EOL
%start input

%%

input
  : ID EOL        { printf("Valid Identifier\n"); }
  | error EOL     { printf("Error. Failed to parse.\n"); yyclearin; }
  ;

%%

int main(void) {
  printf("Enter an identifier (or press Ctrl+D to exit):\n");
  yyparse();
  return 0;
}
