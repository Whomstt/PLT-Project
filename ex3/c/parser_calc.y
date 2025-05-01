%{
#include <stdio.h>
#include <stdlib.h>

void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
}
%}

%union {
    int ival;  /* Integer value */
}

%token <ival> NUM
%token EOL LP RP PLUS MINUS MUL DIV
%type <ival> expr input

%left PLUS MINUS  /* Set precedence for addition and subtraction */
%left MUL DIV     /* Set precedence for multiplication and division */

%%

input:
    expr EOL      { printf("Result: %d\n", $1); }    /* Print the result */
    | error EOL   { printf("Invalid expression. Try again.\n"); yyclearin; }
    ;

expr:
    expr PLUS expr    { $$ = $1 + $3; }
    | expr MINUS expr { $$ = $1 - $3; }
    | expr MUL expr   { $$ = $1 * $3; }
    | expr DIV expr   { 
        if ($3 == 0) {
            yyerror("Division by zero");
            YYERROR; 
        } else {
            $$ = $1 / $3;
        }
    }
    | LP expr RP      { $$ = $2; }
    | NUM             { $$ = $1; }
    ;

%%

int main(void) {
    printf("Enter an arithmetic expression:\n");
    yyparse();
    return 0;
}
