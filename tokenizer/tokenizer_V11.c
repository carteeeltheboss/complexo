#include "tokenizer.h"

// Helper function to check if a string is a Python keyword
int is_keyword(const char* str) {
    const char* keywords[] = {
        "and",  "as",       "assert", "break",  "class", "continue", "def",
        "del",  "elif",     "else",   "except", "False", "finally",  "for",
        "from", "global",   "if",     "import", "in",    "is",       "lambda",
        "None", "nonlocal", "not",    "or",     "pass",  "raise",    "return",
        "True", "try",      "while",  "with",   "yield"};
    int num_keywords = sizeof(keywords) / sizeof(keywords[0]);
    for (int i = 0; i < num_keywords; i++) {
        if (strcmp(str, keywords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

// Create a token
Token* create_token(TokenType type, const char* value) {
    Token* token = malloc(sizeof(Token));
    if (!token) {
        fprintf(stderr, "Memory allocation failed for token\n");
        exit(1);
    }
    token->type = type;
    token->value = strdup(value);
    if (!token->value) {
        fprintf(stderr, "Memory allocation failed for token value\n");
        exit(1);
    }
    return token;
}

// Free a token
void free_token(Token* token) {
    if (token) {
        free(token->value);
        free(token);
    }
}

// Tokenize Python code
Token** tokenize(const char* code, int* token_count) {
    const char* ptr = code;
    Token** tokens = NULL;
    *token_count = 0;
    int indent_level = 0;
    int current_indent = 0;

    while (*ptr) {
        // Skip whitespace and track indentation
        if (isspace(*ptr)) {
            if (*ptr == '\n') {
                tokens = realloc(tokens, sizeof(Token*) * (*token_count + 1));
                tokens[*token_count] = create_token(TOKEN_NEWLINE, "\\n");
                (*token_count)++;
                current_indent = 0;
            } else if (*ptr == ' ') {
                //current_indent++;
            } else if (*ptr == '\t') {
                current_indent += 4; // Treat tab as 4 spaces
            }
            ptr++;
            continue;
        }

        // Handle indentation tokens
        if (current_indent > indent_level) {
            tokens = realloc(tokens, sizeof(Token*) * (*token_count + 1));
            tokens[*token_count] = create_token(TOKEN_INDENT, "INDENT");
            (*token_count)++;
            indent_level = current_indent;
        } else if (current_indent < indent_level) {
            while (current_indent < indent_level) {
                tokens = realloc(tokens, sizeof(Token*) * (*token_count + 1));
                tokens[*token_count] = create_token(TOKEN_DEDENT, "DEDENT");
                (*token_count)++;
                indent_level -= 4;
            }
        }

        // Handle numbers
        if (isdigit(*ptr)) {
            const char* start = ptr;
            while (isdigit(*ptr) || *ptr == '.')
                ptr++;
            char* value = strndup(start, ptr - start);
            tokens = realloc(tokens, sizeof(Token*) * (*token_count + 1));
            tokens[*token_count] = create_token(TOKEN_NUMBER, value);
            (*token_count)++;
            free(value);
            continue;
        }

        // Handle identifiers and keywords
        if (isalpha(*ptr) || *ptr == '_') {
            const char* start = ptr;
            while (isalnum(*ptr) || *ptr == '_')
                ptr++;
            char* value = strndup(start, ptr - start);
            TokenType type =
                is_keyword(value) ? TOKEN_KEYWORD : TOKEN_IDENTIFIER;
            tokens = realloc(tokens, sizeof(Token*) * (*token_count + 1));
            tokens[*token_count] = create_token(type, value);
            (*token_count)++;
            free(value);
            continue;
        }

        // Handle operators and punctuation
        if (strchr("+-*/=<>!%&|^~:,.()[]{}", *ptr)) {
            const char* start = ptr++;
            char* value = strndup(start, 1); // Single character token
            tokens = realloc(tokens, sizeof(Token*) * (*token_count + 1));
            tokens[*token_count] = create_token(TOKEN_OPERATOR, value);
            (*token_count)++;
            free(value);
            continue;
        }

        // Skip comments
        if (*ptr == '#') {
            while (*ptr && *ptr != '\n')
                ptr++;
            continue;
        }

        // Handle string literals
        if (*ptr == '"' || *ptr == '\'') {
            char quote_char =
                *ptr++; // Store the type of quote (single or double)
            const char* start = ptr;

            while (*ptr && *ptr != quote_char) {
                if (*ptr == '\\' && *(ptr + 1)) {
                    ptr += 2; // Skip escaped characters
                } else {
                    ptr++;
                }
            }

            if (*ptr != quote_char) {
                fprintf(stderr, "Error: Unterminated string literal\n");
                exit(1);
            }

            // Create a string token
            char* value = strndup(start, ptr - start);
            tokens = realloc(tokens, sizeof(Token*) * (*token_count + 1));
            tokens[*token_count] = create_token(TOKEN_STRING, value);
            (*token_count)++;
            free(value);

            ptr++; // Move past the closing quote
            continue;
        }
    }

    // Add EOF token
    tokens = realloc(tokens, sizeof(Token*) * (*token_count + 1));
    tokens[*token_count] = create_token(TOKEN_EOF, "EOF");
    (*token_count)++;
    return tokens;
}
