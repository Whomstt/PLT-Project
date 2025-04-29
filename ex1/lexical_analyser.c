#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define MAX_LINE_LENGTH 1024

const char* keywords[] = {
    "if", "else", "while", "for", "return", "int", "float", "char", "void"
};

int is_keyword(const char* str) {
    for(int i = 0; keywords[i] != NULL; i++) {
        if (strcmp(str, keywords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

int is_operator(char c) {
    return (c == '+' || c == '-' || c == '*' || c== '/' || c == '=' || c == '>' || c == '<');
}

int is_punctuator(char c) {
    return (c == ';' || c == ',' || c == '.' || c == '(' || c == ')' || c == '{' || c == '}');
}

int is_special_char(char c) {
    return (c == '#' || c == '&' || c == '%' || c == '^');
}

void lexer(const char* input) {
    int length = strlen(input);
    char buffer[256];
    int buffer_index = 0;

    for(int i = 0; i < length; i++) {
        if(isspace(input[i])) {
            continue;
        }
        if(isalpha(input[i]) || input[i] == '_') {
            buffer_index = 0;
            while(isalnum(input[i]) || input[i] == '_') {
                buffer[buffer_index++] = input[i++];
            }
            buffer[buffer_index] = '\0';
            if(is_keyword(buffer)) {
                printf("Keyword: %s\n", buffer);
            } else {
                printf("Identifier: %s\n", buffer);
            }
            i--;
        }

        else if(is_operator(input[i])) {
            printf("Operator: %c\n", input[i]);
        }

        else if(isdigit(input[i])) {
            printf("Constant: ");
            while(isdigit(input[i])) {
                putchar(input[i]);
                i++;
            }
            printf("\n");
            i--;
        }

        else if (input[i] == '"') {
            buffer_index = 0;
            i++;
            while(input[i] != '"' && input[i] != '\0') {
                buffer[buffer_index++] = input[i++];
            }
            buffer[buffer_index] = '\0';
            if (input[i] == '"') {
                printf("String Literal: %s\n", buffer);
            }
        }

        else if(is_punctuator(input[i])) {
            printf("Punctuator: %c\n", input[i]);
        }

        else if(is_special_char(input[i])) {
            printf("Special Character: %c\n", input[i]);
        }

        else {
            printf("Unknown character: %c\n", input[i]);
        }
    }
}

int main() {
    char filename[] = "input.txt";
    FILE* file = fopen(filename, "r");
    if(file == NULL) {
        perror("Error opening file");
        return 1;
    }

    char line[MAX_LINE_LENGTH];
    while(fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = 0; // Remove newline character if present
        lexer(line);
    }
    fclose(file);
    return 0;
}