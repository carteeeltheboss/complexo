#include "parser.h"

int main() {
    const char *code = "for i in range(10): print(i)";
    int token_count = 0;

    Token **tokens = tokenize(code, &token_count);

    int index = 0;
    ASTNode *ast = parse_program(tokens, &index);

    if (ast) {
        printf("Parsed program successfully!\n");
        free_ast(ast);
    } else {
        printf("Failed to parse program.\n");
    }

    // Free tokens
    for (int i = 0; i < token_count; i++) {
        free_token(tokens[i]);
    }
    free(tokens);

    return EXIT_SUCCESS;
}
