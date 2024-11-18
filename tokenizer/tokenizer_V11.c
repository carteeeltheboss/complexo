#include "tokenizer.h"


Token *create_token(TokenType type, const char *value) {
    Token *token = malloc(sizeof(Token));
    if (token == NULL) {
        fprintf(stderr, "Memory allocation failed!\n");
        exit(1); 
    }
    token->type = type;
    token->value = strdup(value); 
    if (token->value == NULL) {
        fprintf(stderr, "Memory allocation failed!\n");
        exit(1); 
    }
    return token;
}

void free_token(Token *token) {
    free(token->value);
    free(token);
}

// Helper function to check if a string is a keyword
int is_keyword(const char *str) {
    const char *keywords[] = {
        "and", "as", "assert", "break", "class", "continue", "def", "del", 
        "elif", "else", "except", "False", "finally", "for", "from", "global", 
        "if", "import", "in", "is", "lambda", "None", "nonlocal", "not", "or", 
        "pass", "raise", "return", "True", "try", "while", "with", "yield"
    };
    int num_keywords = sizeof(keywords) / sizeof(keywords[0]);
    for (int i = 0; i < num_keywords; i++) {
        if (strcmp(str, keywords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

Token **tokenize(const char *code, int *token_count) {
    const char *ptr = code;
    Token **tokens = NULL;
    *token_count = 0;
    int indent_level = 0; 
    int current_line_indent = 0;

    while (*ptr) {
        if (isspace(*ptr)) {
            if (*ptr == '\n') {
                tokens = realloc(tokens, sizeof(Token *) * (*token_count + 1));
                tokens[*token_count] = create_token(TOKEN_NEWLINE, "\n");
                (*token_count)++;
                current_line_indent = 0; 
            } else if (*ptr == ' ') {
                current_line_indent++;
            } else if (*ptr == '\t') {
                // Assuming a tab is equivalent to 4 spaces
                current_line_indent += 4; 
            }
            ptr++;
            continue;
        }

        // Handle indentation
        if (current_line_indent > indent_level) {
            tokens = realloc(tokens, sizeof(Token *) * (*token_count + 1));
            tokens[*token_count] = create_token(TOKEN_INDENT, "INDENT");
            (*token_count)++;
            indent_level = current_line_indent;
        } else if (current_line_indent < indent_level) {
            while (current_line_indent < indent_level) {
                tokens = realloc(tokens, sizeof(Token *) * (*token_count + 1));
                tokens[*token_count] = create_token(TOKEN_DEDENT, "DEDENT");
                (*token_count)++;
                indent_level -= 4; // Assuming dedent is in steps of 4 spaces
            }
        }

        if (isdigit(*ptr)) {
            const char *start = ptr;
            while (isdigit(*ptr) || *ptr == '.') { // Allow for decimal points
                ptr++;
            }
            char *value = strndup(start, ptr - start);
            tokens = realloc(tokens, sizeof(Token *) * (*token_count + 1));
            tokens[*token_count] = create_token(TOKEN_NUMBER, value);
            (*token_count)++;
            free(value);
        } else if (isalpha(*ptr) || *ptr == '_') { 
            const char *start = ptr;
            while (isalnum(*ptr) || *ptr == '_') {
                ptr++;
            }
            char *value = strndup(start, ptr - start);
            TokenType type = is_keyword(value) ? TOKEN_KEYWORD : TOKEN_IDENTIFIER;
            tokens = realloc(tokens, sizeof(Token *) * (*token_count + 1));
            tokens[*token_count] = create_token(type, value);
            (*token_count)++;
            free(value);
        } else if (*ptr == '"' || *ptr == '\'') {
            char quote = *ptr;
            const char *start = ptr;
            ptr++; 

            while (*ptr && *ptr != quote) {
                if (*ptr == '\\') {
                    ptr++; 
                    if (*ptr == '\0') {
                        fprintf(stderr, "Error: Unterminated string literal\n");
                        exit(1);
                    }
                }
                ptr++;
            }

            if (*ptr == quote) {
                ptr++; 
            } else {
                fprintf(stderr, "Error: Unterminated string literal\n");
                exit(1);
            }

            char *value = strndup(start, ptr - start);
            tokens = realloc(tokens, sizeof(Token *) * (*token_count + 1));
            tokens[*token_count] = create_token(TOKEN_STRING, value);
            (*token_count)++;
            free(value);
        } else if (strchr("+-*/=<>!%&|^~:,.()[]{}", *ptr) != NULL) { 
            const char *start = ptr;
            // Handle combined operators (e.g., ==, +=, **, //)
            while (strchr("+-*/=<>!%&|^~:", *ptr) != NULL) { 
                ptr++;
            }
            char *value = strndup(start, ptr - start);
            tokens = realloc(tokens, sizeof(Token *) * (*token_count + 1));
            tokens[*token_count] = create_token(TOKEN_OPERATOR, value);
            (*token_count)++;
            free(value);
        } else if (*ptr == '#') { // Handle single-line comments
            while (*ptr && *ptr != '\n') {
                ptr++;
            }
        } else {
            // Handle unknown characters or errors here
            fprintf(stderr, "Error: Unrecognized character '%c'\n", *ptr);
            exit(1); 
        }
    }

    // Handle dedents at the end of the code
    while (indent_level > 0) {
        tokens = realloc(tokens, sizeof(Token *) * (*token_count + 1));
        tokens[*token_count] = create_token(TOKEN_DEDENT, "DEDENT");
        (*token_count)++;
        indent_level -= 4; 
    }

    tokens = realloc(tokens, sizeof(Token *) * (*token_count + 1));
    tokens[*token_count] = create_token(TOKEN_EOF, "EOF");
    (*token_count)++;

    return tokens;
}