#include "tokenizer.h"

Token *create_token(TokenType type, const char *value){
	Token *token = malloc(sizeof(Token));
	token->type = type;
	token->value = strdup(value);

	return token;
}

void free_token(Token *token){
	free(token->value);
	free(token);
	token = NULL;
}

Token **tokenize(const char *code, int *token_count){
	
}