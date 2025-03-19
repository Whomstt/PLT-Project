%{
#include <stdio.h>
%}

%option noyywrap

KEYWORD     "if"|"else"|"while"|"return"
IDENTIFIER  [a-zA-Z_][a-zA-Z0-9_]*
CONSTANT    [0-9]+
LITERAL     \"([^\\\"]|\\.)*\"
OPERATOR    (\+|\-|\*|\/|==|=|<=|>=|<|>|!)
PUNCTUATOR  [\(\)\{\}\[\];,\.]
WHITESPACE  [ \t\r\n]+

%%
{KEYWORD}      { printf("Keyword: %s\n", yytext); }
{IDENTIFIER}   { printf("Identifier: %s\n", yytext); }
{CONSTANT}     { printf("Constant: %s\n", yytext); }
{LITERAL}      { printf("Literal: %s\n", yytext); }
{OPERATOR}     { printf("Operator: %s\n", yytext); }
{PUNCTUATOR}   { printf("Punctuator: %s\n", yytext); }
{WHITESPACE}   { /* Ignore whitespace */ }
.              { printf("Special character: %s\n", yytext); }

%%

int main(int argc, char **argv) {
    if (argc > 1) {
        FILE *file = fopen(argv[1], "r");
        if (!file) {
            perror("Error opening file");
            return 1;
        }
        yyin = file;
    }
    yylex();
    return 0;
}
