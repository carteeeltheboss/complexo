#ifndef PARSER_H
#define PARSER_H
#include "../tokenizer/tokenizer.h"
#include "../hasher/hasher.h"
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
ASTNode *parse_return_statement(Token **tokens, int *index);
ASTNode *parse_if_statement(Token **tokens, int *index);
ASTNode *parse_while_loop(Token **tokens, int *index);
ASTNode *parse_program(Token **tokens, int *index);
ASTNode *parse_function_definition(Token **tokens, int *index);
void trace_ast(ASTNode *node, int level);
ASTNode *parse_index_expression(Token **tokens, int *index);
ASTNode *parse_assignment(Token **tokens, int *index);

#endif