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
	const char *ptr = code;
	Token **tokens = NULL;
	*token_count = 0;

	while(*ptr){
		if(isspace(*ptr)){
			ptr++;
			continue;
		}

		if(isdigit(*ptr)){
			const char *start = ptr;
			while(isdigit(*ptr)) ptr++;
			char *value = strndup(start, ptr-start);
			tokens = realloc(tokens, sizeof(Token *)*(*token_count+1));
			tokens[*token_count] = create_token(TOKEN_NUMBER, value);
			(*token_count)++;
			free(value);
			value = NULL;
		}else if(isalpha(*ptr)){
			const char *start = ptr;
			while(isalnum(*ptr)||*ptr=='_') ptr++;
			char *value = strndup(start, ptr-start);

			TokenType type = TOKEN_IDENTIFIER;
			if(strcmp(value, "def")*strcmp(value, "while")*strcmp(value, "if")*strcmp(value, "for") == 0){
				type = TOKEN_KEYWORD;
			}

			tokens = realloc(tokens, sizeof(Token *)*(*token_count+1));
			tokens[*token_count] = create_token(type, value);
			(*token_count)++;
			free(value);
			value = NULL;
		}else{
			char value[2]={*ptr, '\0'};
			tokens = realloc(tokens, sizeof(Token *), (*token_count+1));
			tokens[*token_count]=create_token(TOKEN_PONCTUATION, value);
			(*token_count)++;
			ptr++;
		}

	}
	tokens = realloc(tokens, sizeof(Token *)*(*token_count+1));
	tokens[*token_count] = create_token(TOKEN_EOF, "EOF");
	(*token_count)++;
	return tokens;

}





















