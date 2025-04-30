#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define MAX_LINE_LENGTH 1024

const char* keywords[] = {"if", "else", "while", "for", "return", "int", "float", "char", "void", NULL};

int is_keyword(const char* str) {
    for (int i = 0; keywords[i] != NULL; i++) {
        if (strcmp(str, keywords[i]) == 0) return 1;
    }
    return 0;
}

int is_operator(char c) {
    return strchr("+-*/=><", c) != NULL;
}

int is_punctuator(char c) {
    return strchr(";,().{}[]", c) != NULL;
}

int is_special_char(char c) {
    return strchr("#&%^", c) != NULL;
}

void lexer(const char* input) {
    char buffer[256];
    int buffer_index = 0;

    for (int i = 0; input[i] != '\0'; i++) {
        if (input[i] == '/' && input[i + 1] == '/') break;
        if (isspace(input[i])) continue;

        if (isalpha(input[i]) || input[i] == '_') {
            buffer_index = 0;
            while (isalnum(input[i]) || input[i] == '_') buffer[buffer_index++] = input[i++];
            buffer[buffer_index] = '\0';
            printf("%s: %s\n", is_keyword(buffer) ? "Keyword" : "Identifier", buffer);
            i--;
        } else if (isdigit(input[i])) {
            printf("Constant: ");
            while (isdigit(input[i])) putchar(input[i++]);
            printf("\n");
            i--;
        } else if (input[i] == '"') {
            buffer_index = 0;
            while (input[++i] != '"' && input[i] != '\0') buffer[buffer_index++] = input[i];
            buffer[buffer_index] = '\0';
            if (input[i] == '"') printf("Literal: %s\n", buffer);
        } else if (is_operator(input[i])) {
            printf("Operator: %c\n", input[i]);
        } else if (is_punctuator(input[i])) {
            printf("Punctuator: %c\n", input[i]);
        } else if (is_special_char(input[i])) {
            printf("Special Character: %c\n", input[i]);
        } else {
            printf("Unknown character: %c\n", input[i]);
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    FILE* file = fopen(argv[1], "r");
    if (!file) {
        perror("Error opening file");
        return 1;
    }

    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = '\0';
        lexer(line);
    }

    fclose(file);
    return 0;
}
