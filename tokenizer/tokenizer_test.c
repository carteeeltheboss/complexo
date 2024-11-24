#include "tokenizer.h"

const char *token_type_to_string(TokenType type) {
    switch (type) {
        case TOKEN_IDENTIFIER: return "TOKEN_IDENTIFIER";
        case TOKEN_KEYWORD: return "TOKEN_KEYWORD";
        case TOKEN_NUMBER: return "TOKEN_NUMBER";
        case TOKEN_OPERATOR: return "TOKEN_OPERATOR";
        case TOKEN_DOCSTRING: return "TOKEN_DOCSTRING";
        case TOKEN_STRING: return "TOKEN_STRING";
        case TOKEN_PUNCTUATION: return "TOKEN_PUNCTUATION";
        case TOKEN_INDENT: return "TOKEN_INDENT";
        case TOKEN_DEDENT: return "TOKEN_DEDENT";
        case TOKEN_NEWLINE: return "TOKEN_NEWLINE";
        case TOKEN_EOF: return "TOKEN_EOF";
        default: return "UNKNOWN_TOKEN";
    }
}


int main() {
    int token_count = 0;

    const char *code =
            "def my_function(a, b):\n"
            "    \"\"\"This is a docstring.\"\"\"\n"
            "    if a > b:\n"
            "        return a * b + 10  # Comment\n"
            "    else:\n"
            "        return a / b - 10\n"
            "print(my_function(5, 2))";

    const char *code1 = "for i in x: q";


    Token **tokens = tokenize(code, &token_count);

    printf("Token Count: %d\n", token_count);
for (int i = 0; i < token_count; i++) {
    printf("Token %d: Type = %s, Value = '%s'\n", i, token_type_to_string(tokens[i]->type), tokens[i]->value);
    free_token(tokens[i]);
}
free(tokens);


    return 0;
}




/*
int main() {
    const char *code = "for i in range(10): print(i)";
    int token_count = 0;

    Token **tokens = tokenize(code, &token_count);
    printf("Token count: (%d)\n", token_count);
    for (int i = 0; i < token_count; i++) {
        printf("Token: %s (%d)\n", tokens[i]->value, tokens[i]->type);
    }
    return EXIT_SUCCESS;
}
*/















