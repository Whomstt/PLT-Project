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
    char value[20];
} Token;

// Function prototypes
void tokenize(const char* expression, Token* tokens, int* token_count);
void parse_and_generate_tac(Token* tokens, int token_count);

int main() {
    char expression[100];
    Token tokens[100];
    int token_count = 0;
    
    printf("Enter an expression (e.g., x = a + b * c - d / e + f): ");
    fgets(expression, sizeof(expression), stdin);
    
    // Remove newline character if present
    expression[strcspn(expression, "\n")] = 0;
    
    // Tokenize the expression
    tokenize(expression, tokens, &token_count);
    
    // Generate TAC
    printf("\nThree-Address Code (TAC):\n");
    parse_and_generate_tac(tokens, token_count);
    
    return 0;
}

void tokenize(const char* expression, Token* tokens, int* token_count) {
    int i = 0;
    int token_index = 0;
    
    while (expression[i] != '\0') {
        // Skip whitespace
        if (isspace(expression[i])) {
            i++;
            continue;
        }
        
        // Check for operators
        if (expression[i] == '+' || expression[i] == '-' || 
            expression[i] == '*' || expression[i] == '/') {
            tokens[token_index].type = TOKEN_OPERATOR;
            tokens[token_index].value[0] = expression[i];
            tokens[token_index].value[1] = '\0';
            token_index++;
            i++;
            continue;
        }
        
        // Check for equals
        if (expression[i] == '=') {
            tokens[token_index].type = TOKEN_EQUALS;
            tokens[token_index].value[0] = '=';
            tokens[token_index].value[1] = '\0';
            token_index++;
            i++;
            continue;
        }
        
        // Check for variables or numbers
        if (isalnum(expression[i])) {
            int j = 0;
            while (isalnum(expression[i])) {
                tokens[token_index].value[j++] = expression[i++];
            }
            tokens[token_index].value[j] = '\0';
            
            // Determine if it's a variable or number
            if (isdigit(tokens[token_index].value[0])) {
                tokens[token_index].type = TOKEN_NUMBER;
            } else {
                tokens[token_index].type = TOKEN_VARIABLE;
            }
            
            token_index++;
            continue;
        }
        
        // Skip unknown characters
        i++;
    }
    
    // Add end token
    tokens[token_index].type = TOKEN_END;
    tokens[token_index].value[0] = '\0';
    *token_count = token_index;
}

// Helper function to check operator precedence
int get_precedence(char op) {
    if (op == '*' || op == '/') return 2;
    if (op == '+' || op == '-') return 1;
    return 0;
}

void parse_and_generate_tac(Token* tokens, int token_count) {
    int temp_count = 1;
    int i;
    char left_side[20] = "";
    int target_index = -1;
    
    // Find the position of the equals sign
    for (i = 0; i < token_count; i++) {
        if (tokens[i].type == TOKEN_EQUALS) {
            target_index = i;
            strcpy(left_side, tokens[i-1].value);
            break;
        }
    }
    
    if (target_index == -1) {
        printf("Error: No assignment operator found\n");
        return;
    }
    
    // Start parsing from after the equals sign
    i = target_index + 1;
    
    // We'll implement a simple expression parser that respects operator precedence
    // First, let's handle multiplication and division
    while (i < token_count) {
        if (tokens[i].type == TOKEN_END) break;
        
        if (tokens[i].type == TOKEN_OPERATOR && 
            (tokens[i].value[0] == '*' || tokens[i].value[0] == '/')) {
            
            // Generate TAC for multiplication or division
            printf("t%d = %s %c %s\n", temp_count, tokens[i-1].value, 
                  tokens[i].value[0], tokens[i+1].value);
            
            // Replace the expression with the temporary variable
            strcpy(tokens[i-1].value, "");
            sprintf(tokens[i-1].value, "t%d", temp_count);
            
            // Remove the operator and right operand
            for (int j = i; j < token_count - 2; j++) {
                tokens[j] = tokens[j+2];
            }
            
            token_count -= 2;
            i -= 1; // Move back to process the temporary variable in next iteration
            temp_count++;
        } else {
            i++;
        }
    }
    
    // Reset for addition and subtraction
    i = target_index + 1;
    
    // Then handle addition and subtraction
    while (i < token_count) {
        if (tokens[i].type == TOKEN_END) break;
        
        if (tokens[i].type == TOKEN_OPERATOR && 
            (tokens[i].value[0] == '+' || tokens[i].value[0] == '-')) {
            
            // Generate TAC for addition or subtraction
            printf("t%d = %s %c %s\n", temp_count, tokens[i-1].value, 
                  tokens[i].value[0], tokens[i+1].value);
            
            // Replace the expression with the temporary variable
            strcpy(tokens[i-1].value, "");
            sprintf(tokens[i-1].value, "t%d", temp_count);
            
            // Remove the operator and right operand
            for (int j = i; j < token_count - 2; j++) {
                tokens[j] = tokens[j+2];
            }
            
            token_count -= 2;
            i -= 1; // Move back to process the temporary variable in next iteration
            temp_count++;
        } else {
            i++;
        }
    }
    
    // Final assignment
    printf("%s = %s\n", left_side, tokens[target_index + 1].value);
}