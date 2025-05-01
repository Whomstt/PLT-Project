#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Operator precedence levels
typedef enum {
    PREC_LOW = 1,   // +, -
    PREC_HIGH = 2   // *, /
} Precedence;

// Return the precedence of an operator, or 0 if not an operator
static Precedence get_precedence(char op) {
    switch (op) {
        case '+': case '-': return PREC_LOW;
        case '*': case '/': return PREC_HIGH;
        default:            return 0;
    }
}

// Compare precedence of op1 vs op2
// Returns >0 if op1 > op2, 0 if equal, <0 if op1 < op2
static int compare_precedence(char op1, char op2) {
    return get_precedence(op1) - get_precedence(op2);
}

// Parse an arithmetic expression (single-digit operands) and
// demonstrate operator precedence using a stack
void parse_with_precedence(const char *expr) {
    char stack[100];    // operator stack
    int top = -1;

    printf("Parsing expression: %s\n", expr);

    for (size_t i = 0; expr[i] != '\0'; ++i) {
        char c = expr[i];

        if (c >= '0' && c <= '9') {
            // Operand
            printf("Operand: %c\n", c);
        }
        else if (strchr("+-*/", c)) {
            // Operator
            printf("Operator: %c\n", c);

            // Pop while top of stack has higher or equal precedence
            while (top >= 0 && compare_precedence(stack[top], c) >= 0) {
                printf("Popping operator: %c\n", stack[top--]);
            }

            // Push current operator
            stack[++top] = c;
        }
        else {
            fprintf(stderr, "Error: Invalid character '%c' in expression\n", c);
            exit(EXIT_FAILURE);
        }
    }

    // Pop remaining operators
    while (top >= 0) {
        printf("Popping operator: %c\n", stack[top--]);
    }

    printf("Parsing completed.\n");
}

int main(void) {
    char expr[101];

    printf("Enter an arithmetic expression: ");
    if (scanf("%100s", expr) != 1) {
        fprintf(stderr, "Failed to read expression.\n");
        return EXIT_FAILURE;
    }

    parse_with_precedence(expr);
    return EXIT_SUCCESS;
}
