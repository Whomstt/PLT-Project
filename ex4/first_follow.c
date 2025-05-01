#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_COUNT 100
#define MAX_LENGTH 256

// Compute FIRST and FOLLOW sets for a grammar read from a file.
// Productions use the form A=... and '#' denotes epsilon.

// Global storage
static char production[MAX_COUNT][MAX_LENGTH];
static int prod_count = 0;

static char first_set[MAX_COUNT][MAX_LENGTH];  // first_set[i][0] = non-terminal, rest = symbols
static char follow_set[MAX_COUNT][MAX_LENGTH]; // follow_set[i][0] = non-terminal, rest = symbols

// Temporary arrays for computing
static char first_buf[MAX_LENGTH];
static int first_buf_len = 0;
static char follow_buf[MAX_LENGTH];
static int follow_buf_len = 0;

// Function declarations
void compute_first(char symbol, int call_prod, int call_pos);
void compute_follow(char symbol);
void collect_follow_first(char symbol, int prod_idx, int pos_in_rhs);

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <grammar-file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    FILE *file = fopen(argv[1], "r");
    if (!file) {
        perror("Error opening grammar file");
        return EXIT_FAILURE;
    }

    // Read productions
    while (prod_count < MAX_COUNT && fgets(production[prod_count], MAX_LENGTH, file)) {
        char *p = production[prod_count] + strlen(production[prod_count]) - 1;
        while (p >= production[prod_count] && (*p == '\n' || *p == '\r' || isspace(*p))) {
            *p-- = '\0';
        }
        if (strlen(production[prod_count]) > 0)
            prod_count++;
    }
    fclose(file);

    // Initialize result arrays
    for (int i = 0; i < prod_count; i++) {
        memset(first_set[i], '!', MAX_LENGTH);
        memset(follow_set[i], '!', MAX_LENGTH);
    }

    // Compute FIRST sets
    char done_nts[MAX_COUNT];
    int done_count = 0;
    for (int i = 0; i < prod_count; i++) {
        char A = production[i][0];
        int seen = 0;
        for (int j = 0; j < done_count; j++) if (done_nts[j] == A) seen = 1;
        if (seen) continue;

        first_buf_len = 0;
        compute_first(A, -1, 0);

        // Store unique FIRST(A)
        first_set[done_count][0] = A;
        int idx = 1;
        for (int k = 0; k < first_buf_len; k++) {
            char c = first_buf[k];
            int dup = 0;
            for (int x = 1; x < idx; x++) if (first_set[done_count][x] == c) dup = 1;
            if (!dup) first_set[done_count][idx++] = c;
        }

        printf("First(%c) = { ", A);
        for (int x = 1; x < idx; x++) printf("%c ", first_set[done_count][x]);
        printf("}\n");

        done_nts[done_count++] = A;
    }

    // Compute FOLLOW sets
    done_count = 0;
    for (int i = 0; i < prod_count; i++) {
        char A = production[i][0];
        int seen = 0;
        for (int j = 0; j < done_count; j++) if (done_nts[j] == A) seen = 1;
        if (seen) continue;

        follow_buf_len = 0;
        compute_follow(A);

        // Store unique FOLLOW(A)
        follow_set[done_count][0] = A;
        int idx = 1;
        for (int k = 0; k < follow_buf_len; k++) {
            char c = follow_buf[k];
            if (c == '=') continue;  // skip '='
            int dup = 0;
            for (int x = 1; x < idx; x++) if (follow_set[done_count][x] == c) dup = 1;
            if (!dup) follow_set[done_count][idx++] = c;
        }

        printf("Follow(%c) = { ", A);
        for (int x = 1; x < idx; x++) printf("%c ", follow_set[done_count][x]);
        printf("}\n");

        done_nts[done_count++] = A;
    }

    return EXIT_SUCCESS;
}

void compute_first(char symbol, int call_prod, int call_pos) {
    if (!isupper(symbol)) {
        first_buf[first_buf_len++] = symbol;
        return;
    }
    for (int i = 0; i < prod_count; i++) {
        if (production[i][0] == symbol && production[i][1] == '=') {
            char *rhs = production[i] + 2;
            if (rhs[0] == '#') {
                first_buf[first_buf_len++] = '#';
                if (call_prod >= 0 && production[call_prod][call_pos]) {
                    compute_first(production[call_prod][call_pos], call_prod, call_pos + 1);
                }
            } else if (!isupper(rhs[0])) {
                first_buf[first_buf_len++] = rhs[0];
            } else {
                compute_first(rhs[0], i, 3);
            }
        }
    }
}

void compute_follow(char symbol) {
    // start symbol gets $
    if (production[0][0] == symbol) follow_buf[follow_buf_len++] = '$';
    for (int i = 0; i < prod_count; i++) {
        char *rhs = production[i] + 2;
        for (int j = 0; rhs[j]; j++) {
            if (rhs[j] == symbol) {
                if (rhs[j+1]) {
                    collect_follow_first(rhs[j+1], i, j+3);
                } else if (production[i][0] != symbol) {
                    compute_follow(production[i][0]);
                }
            }
        }
    }
}

void collect_follow_first(char symbol, int prod_idx, int pos_in_rhs) {
    if (!isupper(symbol)) {
        follow_buf[follow_buf_len++] = symbol;
    } else {
        // find FIRST(symbol)
        int row = -1;
        for (int i = 0; i < prod_count; i++) if (first_set[i][0] == symbol) { row = i; break; }
        if (row < 0) return;
        for (int j = 1; first_set[row][j] != '!'; j++) {
            char c = first_set[row][j];
            if (c == '#') {
                // epsilon: look further
                if (!production[prod_idx][pos_in_rhs-2]) {
                    compute_follow(production[prod_idx][0]);
                } else {
                    collect_follow_first(production[prod_idx][pos_in_rhs-2], prod_idx, pos_in_rhs+1);
                }
            } else {
                follow_buf[follow_buf_len++] = c;
            }
        }
    }
}