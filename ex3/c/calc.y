%{
#include <stdio.h>
#include <stdlib.h>

void yyerror(const char *s);
%}

%union {
    int ival;
}

%token <ival> NUM
%token EOL LP RP PLUS MINUS MUL DIV
%type <ival> expr input

%left PLUS MINUS
%left MUL DIV

%%

input:
    expr EOL      { printf("Result: %d\n", $1); }
    ;

expr:
    expr PLUS expr    { $$ = $1 + $3; }
    | expr MINUS expr   { $$ = $1 - $3; }
    | expr MUL expr     { $$ = $1 * $3; }
    | expr DIV expr     { $$ = $1 / $3; }
    | LP expr RP        { $$ = $2; }
    | NUM               { $$ = $1; }
    ;

%%

int main() {
    printf("Enter arithmetic expression: ");
    yyparse();
    return 0;
}

void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
}
