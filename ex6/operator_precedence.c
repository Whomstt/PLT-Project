#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define operator precedence levels
#define LOW_PRECEDENCE 1
#define HIGH_PRECEDENCE 2

// Function to return precedence of operators
int precedence(char op) {
    switch(op) {
        case '+':
        case '-':
            return LOW_PRECEDENCE;  // Lower precedence for + and -
        case '*':
        case '/':
            return HIGH_PRECEDENCE; // Higher precedence for * and /
        default:
            return -1; // Invalid operator
    }
}

// Function to check the operator precedence between two operators
int compare_precedence(char op1, char op2) {
    int p1 = precedence(op1);
    int p2 = precedence(op2);

    if (p1 > p2) {
        return 1; // op1 has higher precedence
    } else if (p1 < p2) {
        return -1; // op2 has higher precedence
    } else {
        return 0; // Same precedence
    }
}

// Function to parse the expression and apply operator precedence rules
void operator_precedence_parser(char *expression) {
    int i = 0;
    char stack[100];  // Stack to store operators
    int top = -1;     // Stack pointer

    printf("Parsing expression: %s\n", expression);

    while (expression[i] != '\0') {
        char current = expression[i];

        // If it's an operand (we assume single character operands for simplicity)
        if ((current >= '0' && current <= '9')) {
            printf("Operand: %c\n", current);
            i++;
        } 
        // If it's an operator
        else if (current == '+' || current == '-' || current == '*' || current == '/') {
            printf("Operator: %c\n", current);

            // While there's an operator on the stack with higher or equal precedence
            while (top >= 0 && compare_precedence(stack[top], current) >= 0) {
                printf("Popping operator: %c\n", stack[top]);
                top--;
            }

            // Push the current operator onto the stack
            stack[++top] = current;
            i++;
        } 
        // Invalid character
        else {
            printf("Invalid character encountered: %c\n", current);
            exit(1);
        }
    }

    // Pop all remaining operators in the stack
    while (top >= 0) {
        printf("Popping operator: %c\n", stack[top]);
        top--;
    }

    printf("Parsing completed.\n");
}

int main() {
    char expression[100];

    // Ask the user to enter the expression
    printf("Enter an arithmetic expression: ");
    scanf("%s", expression);

    // Call the operator precedence parser
    operator_precedence_parser(expression);

    return 0;
}
