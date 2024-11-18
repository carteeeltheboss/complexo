#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define EXIT_SUCCESS 0

typedef enum {
	TOKEN_KEYWORD,
	TOKEN_IDENTIFIER,
	TOKEN_NUMBER,
	TOKEN_OPERATOR,
	TOKEN_PUNCTUATION,
	TOKEN_EOF

} TokenType;

typedef struct {
	TokenType type;
	char *value;
} Token;

Token *create_token(TokenType type, const char *value);
void free_token(Token *token);
Token **tokenize(const char *code, int *token_count);


#endif