#include "tokenizer.h"

int main() {
    const char *code = 
        "def my_function(a, b):\n"
        "    \"\"\"This is a docstring.\"\"\"\n"
        "    if a > b:\n"
        "        return a * b + 10  # This is a comment\n"
        "    else:\n"
        "        return a / b - 10\n"
        "\n"
        "print(my_function(5, 2))";

    int token_count = 0;
    Token **tokens = tokenize(code, &token_count);

    printf("Token Count: %d\n", token_count);
    for (int i = 0; i < token_count; i++) {
        printf("Token %d: Type = %d, Value = '%s'\n", i, tokens[i]->type, tokens[i]->value);
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