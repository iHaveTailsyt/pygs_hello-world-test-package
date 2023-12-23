#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

// Token types
typedef enum {
    INTEGER,
    PLUS,
    MINUS,
    MULTIPLY,
    DIVIDE,
    VARIABLE,
    ASSIGN,
    END_OF_FILE
} TokenType;

// Token structure
typedef struct {
    TokenType type;
    int value;  // For INTEGER tokens
    char variable;  // For VARIABLE tokens
} Token;

// Function to get the next token from the input
Token getNextToken() {
    Token token;
    int c = getchar();

    while (isspace(c)) {
        c = getchar();
    }

    if (isdigit(c)) {
        ungetc(c, stdin);
        scanf("%d", &token.value);
        token.type = INTEGER;
    } else if (c == '+') {
        token.type = PLUS;
    } else if (c == '-') {
        token.type = MINUS;
    } else if (c == '*') {
        token.type = MULTIPLY;
    } else if (c == '/') {
        token.type = DIVIDE;
    } else if (isalpha(c)) {
        token.variable = c;
        token.type = VARIABLE;
    } else if (c == '=') {
        token.type = ASSIGN;
    } else if (c == EOF) {
        token.type = END_OF_FILE;
    } else {
        fprintf(stderr, "Error: Invalid character '%c'\n", c);
        exit(EXIT_FAILURE);
    }

    return token;
}

// Function to parse expressions
int parseExpression();

// Function to parse variable assignment
void parseAssignment(char variable);

int main() {
    printf("Simple C-like Language Interpreter\n");

    Token token = getNextToken();

    while (token.type != END_OF_FILE) {
        if (token.type == VARIABLE) {
            // Variable assignment
            parseAssignment(token.variable);
        } else {
            // Expression evaluation
            printf("Result: %d\n", parseExpression());
        }

        token = getNextToken();
    }

    return 0;
}

// Function to parse factor
int parseFactor() {
    Token token = getNextToken();

    if (token.type == INTEGER) {
        return token.value;
    } else if (token.type == VARIABLE) {
        // Assume the variable already has a value
        return token.value;  // In a real interpreter, you'd need a symbol table
    } else if (token.type == MINUS) {
        return -parseFactor();
    } else {
        fprintf(stderr, "Error: Unexpected token in factor\n");
        exit(EXIT_FAILURE);
    }
}

// Function to parse term
int parseTerm() {
    int result = parseFactor();

    Token token = getNextToken();
    while (token.type == MULTIPLY || token.type == DIVIDE) {
        if (token.type == MULTIPLY) {
            result *= parseFactor();
        } else {
            int divisor = parseFactor();
            if (divisor != 0) {
                result /= divisor;
            } else {
                fprintf(stderr, "Error: Division by zero\n");
                exit(EXIT_FAILURE);
            }
        }
        token = getNextToken();
    }

    // Backtrack one token
    ungetc(token.type, stdin);

    return result;
}

// Function to parse expression
int parseExpression() {
    int result = parseTerm();

    Token token = getNextToken();
    while (token.type == PLUS || token.type == MINUS) {
        if (token.type == PLUS) {
            result += parseTerm();
        } else {
            result -= parseTerm();
        }
        token = getNextToken();
    }

    // Backtrack one token
    ungetc(token.type, stdin);

    return result;
}

// Function to parse variable assignment
void parseAssignment(char variable) {
    Token token = getNextToken();
    if (token.type == ASSIGN) {
        int value = parseExpression();
        // In a real interpreter, you'd update the symbol table with the new value
        printf("Variable '%c' assigned the value: %d\n", variable, value);
    } else {
        fprintf(stderr, "Error: Expected '=' after variable\n");
        exit(EXIT_FAILURE);
    }
}
