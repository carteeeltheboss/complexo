#ifndef PARSER_H
#define PARSER_H
#include "tokenizer.h"

typedef struct ASTNode{
	char *type;
	char *value;
	struct ASTNode **children;
	int child_count;
}ASTNode;

ASTNode *create_ast_node(const char *type, const char *value);
void add_child(ASTNode *parent, ASTNode *child);
void free_ast(ASTNode *node);
ASTNode *parse_syntax(Token **tokens, int *index);
ASTNode *parse_exp(Token **tokens, int *index);
ASTNode *parse_for_loop(Token **tokens, int *index);
ASTNode *parse_program(Token **tokens, int *index);



#endif