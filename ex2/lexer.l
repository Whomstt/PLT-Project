%{
#include <stdio.h>
%}

%option noyywrap

KEYWORD      "if"|"else"|"while"|"return"|"void"|"int"|"float"|"char"|"main"
IDENTIFIER   [a-zA-Z_][a-zA-Z0-9_]*
CONSTANT     [0-9]+
FLOAT        [0-9]+\.[0-9]+
LITERAL      \"([^\\\"]|\\.)*\" 
ARITHMETIC_OPERATOR (\+|\-|\*|\/|=)
SPECIAL_OPERATOR [\(\)\{\}\[\];,\.]
COMMENT      \/\/.*|\/\*[^*]*\*+([^/*][^*]*\*+)*\/
WHITESPACE   [ \t\r\n]+

%%

{KEYWORD}             { printf("%s Reserved Keyword\n", yytext); }
{IDENTIFIER}          { printf("%s Identifier\n", yytext); }
{FLOAT}               { printf("%s Floating Number\n", yytext); }
{CONSTANT}            { printf("%s Constant\n", yytext); }
{LITERAL}             { printf("%s Literal\n", yytext); }
{ARITHMETIC_OPERATOR} { printf("%s Arithmetic Operator\n", yytext); }
{SPECIAL_OPERATOR}    { printf("%s Special Operator\n", yytext); }
{COMMENT}             { printf("%s Comment\n", yytext); }
{WHITESPACE}          { /* Ignore whitespace */ }
.                     { printf("%s Unknown character\n", yytext); }

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
