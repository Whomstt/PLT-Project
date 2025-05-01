#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Token types
typedef enum {
    TOKEN_VARIABLE,
    TOKEN_OPERATOR,
    TOKEN_NUMBER,
    TOKEN_EQUALS,
    TOKEN_END
} TokenType;

// Token structure
typedef struct {
    TokenType type;
    char value[32];
} Token;

// Function prototypes
static void tokenize(const char *expr, Token *tokens, int *count);
static void generate_tac(Token *tokens, int count);
static int precedence(char op);

int main(void) {
    char input[256];
    Token tokens[128];
    int token_count = 0;

    printf("Enter an expression (e.g., x = a + b * c - d / e + f): ");
    if (!fgets(input, sizeof(input), stdin)) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    input[strcspn(input, "\n")] = '\0';  // strip newline

    tokenize(input, tokens, &token_count);

    printf("\nThree-Address Code (TAC):\n");
    generate_tac(tokens, token_count);

    return EXIT_SUCCESS;
}

// Split input into tokens
static void tokenize(const char *expr, Token *tokens, int *count) {
    int i = 0, idx = 0;
    while (expr[i]) {
        if (isspace(expr[i])) { i++; continue; }

        if (strchr("+-*/", expr[i])) {
            tokens[idx].type = TOKEN_OPERATOR;
            tokens[idx].value[0] = expr[i++];
            tokens[idx].value[1] = '\0';
        }
        else if (expr[i] == '=') {
            tokens[idx].type = TOKEN_EQUALS;
            tokens[idx].value[0] = expr[i++];
            tokens[idx].value[1] = '\0';
        }
        else if (isalnum(expr[i])) {
            int j = 0;
            while (isalnum(expr[i])) {
                tokens[idx].value[j++] = expr[i++];
            }
            tokens[idx].value[j] = '\0';
            tokens[idx].type = isdigit(tokens[idx].value[0]) ? TOKEN_NUMBER : TOKEN_VARIABLE;
        }
        else {
            // skip unknown
            i++;
            continue;
        }
        idx++;
    }
    tokens[idx].type = TOKEN_END;
    tokens[idx].value[0] = '\0';
    *count = idx;
}

// Return operator precedence
static int precedence(char op) {
    return (op == '*' || op == '/') ? 2 :
           (op == '+' || op == '-') ? 1 : 0;
}

// Generate Three-Address Code respecting precedence
static void generate_tac(Token *toks, int count) {
    char target[32] = "";
    int eq_pos = -1, temp_id = 1;

    // locate '=' and LHS variable
    for (int i = 0; i < count; i++) {
        if (toks[i].type == TOKEN_EQUALS && i > 0) {
            eq_pos = i;
            strcpy(target, toks[i-1].value);
            break;
        }
    }
    if (eq_pos < 0) {
        fprintf(stderr, "Error: no assignment operator\n");
        return;
    }

    // process ops by precedence: 2 then 1
    for (int prec = 2; prec >= 1; prec--) {
        for (int i = eq_pos + 1; i < count - 1; i++) {
            if (toks[i].type == TOKEN_OPERATOR && precedence(toks[i].value[0]) == prec) {
                printf("t%d = %s %c %s\n", temp_id,
                       toks[i-1].value, toks[i].value[0], toks[i+1].value);

                // replace left operand with temp
                sprintf(toks[i-1].value, "t%d", temp_id);

                // shift tokens left to remove op and right operand
                memmove(&toks[i], &toks[i+2], sizeof(Token) * (count - i - 2));
                count -= 2;
                temp_id++;
                i--; // re-evaluate at this position
            }
        }
    }

    // final assign
    printf("%s = %s\n", target, toks[eq_pos + 1].value);
}
