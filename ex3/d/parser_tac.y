%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void yyerror(const char *s);
int yylex(void);

/* Helper functions for TAC generation */
static int tempCount = 0, posCount = 0;

static char *newTemp(void) {
    char buf[16];
    snprintf(buf, sizeof(buf), "t%d", tempCount++);
    return strdup(buf);
}

static void emit(const char *op, const char *a1, const char *a2, const char *res) {
    printf("%d %s %s %s %s\n",
           posCount++,
           op,
           a1 ? a1 : "-",
           a2 ? a2 : "-",
           res ? res : "-");
}
%}

%union {
    char *sval;
}

%token MAIN INT
%token <sval> ID NUM
%token EQUAL PLUS MINUS MUL DIV SEMICOLON COMMA LP RP LBRACE RBRACE
%type <sval> expr

%left PLUS MINUS
%left MUL DIV

%%

program:
    MAIN LP RP block
    ;

block:
    LBRACE decls stmts RBRACE
    ;

decls:
      /* empty */
    | decls INT id_list SEMICOLON
    ;

id_list:
      ID
    | id_list COMMA ID
    ;

stmts:
      /* empty */
    | stmts stmt SEMICOLON
    ;

stmt:
    ID EQUAL expr     { emit("=", $3, NULL, $1); }
    ;

expr:
    expr PLUS expr   { char *t = newTemp(); emit("+", $1, $3, t); $$ = t; }
  | expr MINUS expr  { char *t = newTemp(); emit("-", $1, $3, t); $$ = t; }
  | expr MUL expr    { char *t = newTemp(); emit("*", $1, $3, t); $$ = t; }
  | expr DIV expr    { char *t = newTemp(); emit("/", $1, $3, t); $$ = t; }
  | LP expr RP       { $$ = $2; }
  | ID               { $$ = $1; }
  | NUM              { $$ = $1; }
  ;

%%

int main(void) {
    printf("-----------------------------------\n");
    printf("Pos Operator Arg1 Arg2 Result\n");
    printf("-----------------------------------\n");
    yyparse();
    return 0;
}

void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
}
