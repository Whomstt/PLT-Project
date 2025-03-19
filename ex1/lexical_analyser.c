#include <stdio.h>
#include <ctype.h>
#include <string.h>


void lexer(const char* input) {
    int length = strlen(input);
    for(int i = 0; i < length; i++) {
        if(isspace(input[i])) {
            continue;
        }
        if(isdigit(input[i])) {
            printf("NUMBER: ");
            while(isdigit(input[i])) {
                putchar(input[i]);
                i++;
            }
            printf("\n");
        }

    }
}

int main() {
    lexer("123 456");
    return 0;
}