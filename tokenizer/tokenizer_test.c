#include "tokenizer.h" 

int main() {
    const char *code = "while k: j++";
    int token_count = 0;

    Token **tokens = tokenize(code, &token_count);
    printf("Token count: (%d)\n", token_count);
    for (int i = 0; i < token_count; i++) {
        printf("Token: %s (%d)\n", tokens[i]->value, tokens[i]->type);
    }
    return EXIT_SUCCESS;
}
