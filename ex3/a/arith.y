%{
#include <stdio.h>
void yyerror(const char *s) { /* suppress default msg */ }
%}

%union {
  int ival;
}

%token <ival> NUM
%token PLUS MINUS MUL DIV LP RP EOL
%start input

%%

input
  : expr EOL        { printf("Valid Arithmetic Expression\n"); }
  | error EOL       { printf("Error. Failed to parse.\n"); }
  ;

expr
  : expr PLUS expr
  | expr MINUS expr
  | expr MUL expr
  | expr DIV expr
  | LP expr RP
  | NUM
  ;

%%

int main(void) {
  yyparse();
  return 0;
}
