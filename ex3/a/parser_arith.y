%{
#include <stdio.h>
#include <stdlib.h>
void yyerror(const char *s) { fprintf(stderr, "Error: %s\n", s); }
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
  | error EOL       { printf("Error. Failed to parse.\n"); yyclearin; }
  ;

expr
  : expr PLUS expr  { /* handle addition */ }
  | expr MINUS expr { /* handle subtraction */ }
  | expr MUL expr   { /* handle multiplication */ }
  | expr DIV expr   { /* handle division */ }
  | LP expr RP      { /* handle parentheses */ }
  | NUM             { /* handle number */ }
  ;

%%

int main(void) {
  printf("Enter an arithmetic expression:\n");
  yyparse();
  return 0;
}
