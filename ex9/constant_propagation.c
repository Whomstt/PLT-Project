#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define MAX_VARIABLES 100
#define MAX_LINE_LENGTH 256
#define MAX_IDENTIFIER_LENGTH 64

typedef struct {
    char name[MAX_IDENTIFIER_LENGTH];
    int value;
    bool is_constant;
} Variable;

// Global variables table
Variable variables[MAX_VARIABLES];
int var_count = 0;

// Function to find a variable in the table
int find_variable(const char* name) {
    for (int i = 0; i < var_count; i++) {
        if (strcmp(variables[i].name, name) == 0) {
            return i;
        }
    }
    return -1;
}

// Function to add or update a variable
void set_variable(const char* name, int value, bool is_constant) {
    int index = find_variable(name);
    if (index == -1) {
        // Variable doesn't exist, add it
        if (var_count >= MAX_VARIABLES) {
            fprintf(stderr, "Error: Too many variables\n");
            exit(1);
        }
        strcpy(variables[var_count].name, name);
        variables[var_count].value = value;
        variables[var_count].is_constant = is_constant;
        var_count++;
    } else {
        // Update existing variable
        variables[index].value = value;
        variables[index].is_constant = is_constant;
    }
}

// Function to evaluate a simple expression
bool evaluate_expression(const char* expr, int* result) {
    // If the expression is just a number, return it directly
    if (expr[0] && (isdigit(expr[0]) || (expr[0] == '-' && expr[1] && isdigit(expr[1])))) {
        *result = atoi(expr);
        return true;
    }
    
    // Parse simple expressions like "x + 5", "10 - y", etc.
    char term1[MAX_IDENTIFIER_LENGTH] = {0};
    char term2[MAX_IDENTIFIER_LENGTH] = {0};
    char operator = 0;
    int value1, value2;
    bool term1_is_constant = false;
    bool term2_is_constant = false;
    
    // Extract terms and operator
    int pos = 0;
    int term_pos = 0;
    
    // Skip leading whitespace
    while (expr[pos] && isspace(expr[pos])) pos++;
    
    // Parse first term
    while (expr[pos] && !isspace(expr[pos]) && expr[pos] != '+' && expr[pos] != '-' && expr[pos] != '*' && expr[pos] != '/') {
        term1[term_pos++] = expr[pos++];
    }
    term1[term_pos] = '\0';
    
    // Skip whitespace
    while (expr[pos] && isspace(expr[pos])) pos++;
    
    // Get operator
    if (expr[pos] && (expr[pos] == '+' || expr[pos] == '-' || expr[pos] == '*' || expr[pos] == '/')) {
        operator = expr[pos++];
    } else {
        // Just a single term
        if (isdigit(term1[0]) || (term1[0] == '-' && isdigit(term1[1]))) {
            *result = atoi(term1);
            return true;
        } else {
            int var_index = find_variable(term1);
            if (var_index != -1 && variables[var_index].is_constant) {
                *result = variables[var_index].value;
                return true;
            }
            return false;
        }
    }
    
    // Skip whitespace
    while (expr[pos] && isspace(expr[pos])) pos++;
    
    // Parse second term
    term_pos = 0;
    while (expr[pos] && !isspace(expr[pos]) && expr[pos] != '+' && expr[pos] != '-' && expr[pos] != '*' && expr[pos] != '/') {
        term2[term_pos++] = expr[pos++];
    }
    term2[term_pos] = '\0';
    
    // Evaluate first term
    if (isdigit(term1[0]) || (term1[0] == '-' && isdigit(term1[1]))) {
        value1 = atoi(term1);
        term1_is_constant = true;
    } else {
        int var_index = find_variable(term1);
        if (var_index != -1) {
            value1 = variables[var_index].value;
            term1_is_constant = variables[var_index].is_constant;
        } else {
            return false;  // Variable not found
        }
    }
    
    // Evaluate second term
    if (isdigit(term2[0]) || (term2[0] == '-' && isdigit(term2[1]))) {
        value2 = atoi(term2);
        term2_is_constant = true;
    } else {
        int var_index = find_variable(term2);
        if (var_index != -1) {
            value2 = variables[var_index].value;
            term2_is_constant = variables[var_index].is_constant;
        } else {
            return false;  // Variable not found
        }
    }
    
    // Both terms must be constant to evaluate the expression
    if (term1_is_constant && term2_is_constant) {
        switch (operator) {
            case '+': *result = value1 + value2; break;
            case '-': *result = value1 - value2; break;
            case '*': *result = value1 * value2; break;
            case '/': 
                if (value2 == 0) {
                    fprintf(stderr, "Error: Division by zero\n");
                    exit(1);
                }
                *result = value1 / value2; 
                break;
            default:
                return false;  // Unknown operator
        }
        return true;
    }
    
    return false;  // Not all terms are constant
}

// Function to perform constant propagation on an expression
void propagate_constants(const char* expr, char* result) {
    // Parse simple expressions like "x + 5", "10 - y", etc.
    char term1[MAX_IDENTIFIER_LENGTH] = {0};
    char term2[MAX_IDENTIFIER_LENGTH] = {0};
    char operator = 0;
    int value1, value2;
    bool term1_is_constant = false;
    bool term2_is_constant = false;
    
    // Extract terms and operator
    int pos = 0;
    int term_pos = 0;
    
    // Skip leading whitespace
    while (expr[pos] && isspace(expr[pos])) pos++;
    
    // Parse first term
    while (expr[pos] && !isspace(expr[pos]) && expr[pos] != '+' && expr[pos] != '-' && expr[pos] != '*' && expr[pos] != '/') {
        term1[term_pos++] = expr[pos++];
    }
    term1[term_pos] = '\0';
    
    // Skip whitespace
    while (expr[pos] && isspace(expr[pos])) pos++;
    
    // Get operator
    if (expr[pos] && (expr[pos] == '+' || expr[pos] == '-' || expr[pos] == '*' || expr[pos] == '/')) {
        operator = expr[pos++];
    } else {
        // Just a single term
        if (isdigit(term1[0]) || (term1[0] == '-' && isdigit(term1[1]))) {
            strcpy(result, term1);
        } else {
            int var_index = find_variable(term1);
            if (var_index != -1 && variables[var_index].is_constant) {
                sprintf(result, "%d", variables[var_index].value);
            } else {
                strcpy(result, term1);
            }
        }
        return;
    }
    
    // Skip whitespace
    while (expr[pos] && isspace(expr[pos])) pos++;
    
    // Parse second term
    term_pos = 0;
    while (expr[pos] && !isspace(expr[pos]) && expr[pos] != '+' && expr[pos] != '-' && expr[pos] != '*' && expr[pos] != '/') {
        term2[term_pos++] = expr[pos++];
    }
    term2[term_pos] = '\0';
    
    // Evaluate first term
    char propagated_term1[MAX_IDENTIFIER_LENGTH];
    if (isdigit(term1[0]) || (term1[0] == '-' && isdigit(term1[1]))) {
        strcpy(propagated_term1, term1);
    } else {
        int var_index = find_variable(term1);
        if (var_index != -1 && variables[var_index].is_constant) {
            sprintf(propagated_term1, "%d", variables[var_index].value);
        } else {
            strcpy(propagated_term1, term1);
        }
    }
    
    // Evaluate second term
    char propagated_term2[MAX_IDENTIFIER_LENGTH];
    if (isdigit(term2[0]) || (term2[0] == '-' && isdigit(term2[1]))) {
        strcpy(propagated_term2, term2);
    } else {
        int var_index = find_variable(term2);
        if (var_index != -1 && variables[var_index].is_constant) {
            sprintf(propagated_term2, "%d", variables[var_index].value);
        } else {
            strcpy(propagated_term2, term2);
        }
    }
    
    // Combine the propagated terms
    sprintf(result, "%s %c %s", propagated_term1, operator, propagated_term2);
}

// Function to process a single line of code
void process_line(char* line) {
    char variable[MAX_IDENTIFIER_LENGTH];
    char expression[MAX_LINE_LENGTH];
    char optimized_expr[MAX_LINE_LENGTH];
    
    // Parse assignment statement (format: "x = expr")
    char* equals_pos = strchr(line, '=');
    if (equals_pos) {
        // Extract variable name
        int var_len = equals_pos - line;
        strncpy(variable, line, var_len);
        variable[var_len] = '\0';
        
        // Trim whitespace
        char* trimmed_var = variable;
        while (isspace(*trimmed_var)) trimmed_var++;
        char* end = trimmed_var + strlen(trimmed_var) - 1;
        while (end > trimmed_var && isspace(*end)) *end-- = '\0';
        
        // Extract expression
        char* expr_start = equals_pos + 1;
        while (isspace(*expr_start)) expr_start++;
        strcpy(expression, expr_start);
        
        // Trim whitespace from expression
        char* trimmed_expr = expression;
        while (isspace(*trimmed_expr)) trimmed_expr++;
        end = trimmed_expr + strlen(trimmed_expr) - 1;
        while (end > trimmed_expr && isspace(*end)) *end-- = '\0';
        
        // Show original line
        printf("Original: %s = %s\n", trimmed_var, trimmed_expr);
        
        // Attempt constant propagation
        propagate_constants(trimmed_expr, optimized_expr);
        
        // Check if expression can be evaluated
        int result;
        bool is_constant = evaluate_expression(trimmed_expr, &result);
        
        // Update the variable table
        set_variable(trimmed_var, result, is_constant);
        
        // Output the optimized line
        printf("Optimized: %s = %s\n\n", trimmed_var, optimized_expr);
    } else {
        // Not an assignment, just echo the line
        printf("%s\n", line);
    }
}

int main() {
    char line[MAX_LINE_LENGTH];
    
    printf("===== Constant Propagation Optimizer =====\n");
    printf("Enter your code (empty line to finish):\n");
    
    while (fgets(line, sizeof(line), stdin)) {
        // Remove newline character
        line[strcspn(line, "\n")] = 0;
        
        // Break on empty line
        if (strlen(line) == 0) {
            break;
        }
        
        process_line(line);
    }
    
    printf("\n===== Variable Table After Optimization =====\n");
    for (int i = 0; i < var_count; i++) {
        printf("%s = %d (Constant: %s)\n", 
               variables[i].name, 
               variables[i].value, 
               variables[i].is_constant ? "Yes" : "No");
    }
    
    return 0;
}