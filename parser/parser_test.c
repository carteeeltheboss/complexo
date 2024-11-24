#include "parser.h"

// Function to trace the AST recursively
void trace_ast(ASTNode *node, int level) {
    if (node == NULL) {
        return;
    }
    for (int i = 0; i < level; i++) {
        printf("  ");
    }
    printf("Node Type: %s", node->type);
    if (node->value) {
        printf(", Value: %s", node->value);
    }
    printf("\n");

    for (int i = 0; i < node->child_count; i++) {
        trace_ast(node->children[i], level + 1);
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

    const char *code1 = "for i in x:"
                        "   x[i]=5";

    Token **tokens = tokenize(code1, &token_count);

    int index = 0;
    ASTNode *ast = parse_program(tokens, &index);

    if (ast) {
        printf("Parsed program successfully!\n");
        trace_ast(ast, 0);
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