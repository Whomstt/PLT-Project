#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define MAX_VARIABLES 100
#define MAX_LINE_LENGTH 256
#define MAX_IDENTIFIER_LENGTH 64

// Represents a variable or constant in the symbol table
typedef struct {
    char name[MAX_IDENTIFIER_LENGTH];
    int value;
    bool is_constant;
} Variable;

static Variable variables[MAX_VARIABLES];
static int var_count = 0;

// Function prototypes
int find_variable(const char *name);
void set_variable(const char *name, int value, bool is_constant);
bool evaluate_expression(const char *expr, int *result);
void propagate_constants(const char *expr, char *result);
void process_line(char *line);

int main(void) {
    char line[MAX_LINE_LENGTH];

    printf("===== Constant Propagation Optimizer =====\n");
    printf("Enter your code (empty line to finish):\n");

    while (fgets(line, sizeof(line), stdin)) {
        line[strcspn(line, "\n")] = '\0';  // remove newline
        if (line[0] == '\0') break;
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

// Find the index of a variable by name, or -1 if not found
int find_variable(const char *name) {
    for (int i = 0; i < var_count; i++) {
        if (strcmp(variables[i].name, name) == 0) {
            return i;
        }
    }
    return -1;
}

// Add a new variable or update an existing one
void set_variable(const char *name, int value, bool is_constant) {
    int idx = find_variable(name);
    if (idx == -1) {
        if (var_count >= MAX_VARIABLES) {
            fprintf(stderr, "Error: Too many variables\n");
            exit(EXIT_FAILURE);
        }
        strncpy(variables[var_count].name, name, MAX_IDENTIFIER_LENGTH - 1);
        variables[var_count].value = value;
        variables[var_count].is_constant = is_constant;
        var_count++;
    } else {
        variables[idx].value = value;
        variables[idx].is_constant = is_constant;
    }
}

// Evaluate an expression if all terms are constant; returns true if successful
bool evaluate_expression(const char *expr, int *result) {
    // Direct integer literal
    if (expr[0] && (isdigit(expr[0]) || (expr[0] == '-' && isdigit(expr[1])))) {
        *result = atoi(expr);
        return true;
    }

    char term1[MAX_IDENTIFIER_LENGTH] = {0};
    char term2[MAX_IDENTIFIER_LENGTH] = {0};
    char op = 0;
    int v1 = 0, v2 = 0;
    bool c1 = false, c2 = false;
    int pos = 0, tpos = 0;

    // Skip leading whitespace
    while (expr[pos] && isspace(expr[pos])) pos++;

    // Read first term
    while (expr[pos] && !isspace(expr[pos]) && strchr("+-*/,", expr[pos]) == NULL) {
        term1[tpos++] = expr[pos++];
    }
    term1[tpos] = '\0';

    // Skip to operator
    while (expr[pos] && isspace(expr[pos])) pos++;
    if (strchr("+-*/", expr[pos])) {
        op = expr[pos++];
    } else {
        // Single term (variable or literal)
        if (isdigit(term1[0]) || (term1[0] == '-' && isdigit(term1[1]))) {
            *result = atoi(term1);
            return true;
        }
        int idx = find_variable(term1);
        if (idx >= 0 && variables[idx].is_constant) {
            *result = variables[idx].value;
            return true;
        }
        return false;
    }

    // Read second term
    while (expr[pos] && isspace(expr[pos])) pos++;
    tpos = 0;
    while (expr[pos] && !isspace(expr[pos]) && strchr("+-*/,", expr[pos]) == NULL) {
        term2[tpos++] = expr[pos++];
    }
    term2[tpos] = '\0';

    // Resolve term1
    if (isdigit(term1[0]) || (term1[0] == '-' && isdigit(term1[1]))) {
        v1 = atoi(term1);
        c1 = true;
    } else {
        int idx = find_variable(term1);
        if (idx < 0) return false;
        v1 = variables[idx].value;
        c1 = variables[idx].is_constant;
    }

    // Resolve term2
    if (isdigit(term2[0]) || (term2[0] == '-' && isdigit(term2[1]))) {
        v2 = atoi(term2);
        c2 = true;
    } else {
        int idx = find_variable(term2);
        if (idx < 0) return false;
        v2 = variables[idx].value;
        c2 = variables[idx].is_constant;
    }

    if (c1 && c2) {
        switch (op) {
            case '+': *result = v1 + v2; break;
            case '-': *result = v1 - v2; break;
            case '*': *result = v1 * v2; break;
            case '/': 
                if (v2 == 0) {
                    fprintf(stderr, "Error: Division by zero\n");
                    exit(EXIT_FAILURE);
                }
                *result = v1 / v2; break;
            default: return false;
        }
        return true;
    }
    return false;
}

// Replace constant variables in the expression with their values
void propagate_constants(const char *expr, char *out) {
    char term1[MAX_IDENTIFIER_LENGTH] = {0};
    char term2[MAX_IDENTIFIER_LENGTH] = {0};
    char op = 0;
    int pos = 0, tpos = 0;

    // Skip whitespace
    while (expr[pos] && isspace(expr[pos])) pos++;

    // First term
    while (expr[pos] && !isspace(expr[pos]) && strchr("+-*/", expr[pos]) == NULL) {
        term1[tpos++] = expr[pos++];
    }
    term1[tpos] = '\0';

    // Operator
    while (expr[pos] && isspace(expr[pos])) pos++;
    if (strchr("+-*/", expr[pos])) {
        op = expr[pos++];
    } else {
        // Single term propagation
        if (isdigit(term1[0]) || (term1[0] == '-' && isdigit(term1[1]))) {
            strcpy(out, term1);
        } else {
            int idx = find_variable(term1);
            if (idx >= 0 && variables[idx].is_constant) {
                sprintf(out, "%d", variables[idx].value);
            } else {
                strcpy(out, term1);
            }
        }
        return;
    }

    // Second term
    while (expr[pos] && isspace(expr[pos])) pos++;
    tpos = 0;
    while (expr[pos] && !isspace(expr[pos]) && strchr("+-*/", expr[pos]) == NULL) {
        term2[tpos++] = expr[pos++];
    }
    term2[tpos] = '\0';

    // Propagate term1
    char p1[MAX_IDENTIFIER_LENGTH];
    if (isdigit(term1[0]) || (term1[0] == '-' && isdigit(term1[1]))) {
        strcpy(p1, term1);
    } else {
        int idx = find_variable(term1);
        if (idx >= 0 && variables[idx].is_constant) {
            sprintf(p1, "%d", variables[idx].value);
        } else {
            strcpy(p1, term1);
        }
    }

    // Propagate term2
    char p2[MAX_IDENTIFIER_LENGTH];
    if (isdigit(term2[0]) || (term2[0] == '-' && isdigit(term2[1]))) {
        strcpy(p2, term2);
    } else {
        int idx = find_variable(term2);
        if (idx >= 0 && variables[idx].is_constant) {
            sprintf(p2, "%d", variables[idx].value);
        } else {
            strcpy(p2, term2);
        }
    }

    sprintf(out, "%s %c %s", p1, op, p2);
}

// Process a line of input: parse assignment, propagate constants, evaluate if possible
void process_line(char *line) {
    char var[MAX_IDENTIFIER_LENGTH];
    char expr[MAX_LINE_LENGTH];
    char opt_expr[MAX_LINE_LENGTH];

    char *eq = strchr(line, '=');
    if (!eq) {
        printf("%s\n", line);
        return;
    }

    // Extract variable name
    int len = eq - line;
    strncpy(var, line, len);
    var[len] = '\0';
    // Trim whitespace
    char *vstart = var;
    while (isspace(*vstart)) vstart++;
    char *vend = vstart + strlen(vstart) - 1;
    while (vend > vstart && isspace(*vend)) *vend-- = '\0';

    // Extract expression
    strcpy(expr, eq + 1);
    char *estart = expr;
    while (isspace(*estart)) estart++;
    char *eend = estart + strlen(estart) - 1;
    while (eend > estart && isspace(*eend)) *eend-- = '\0';

    printf("Original: %s = %s\n", vstart, estart);

    propagate_constants(estart, opt_expr);
    int value = 0;
    bool is_const = evaluate_expression(estart, &value);
    set_variable(vstart, value, is_const);

    printf("Optimized: %s = %s\n\n", vstart, opt_expr);
}
