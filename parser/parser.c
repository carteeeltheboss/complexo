#include "parser.h"


ASTNode *create_ast_node(const char *type, const char *value){
	ASTNode *node = malloc(sizeof(ASTNode));
	node->type = strdup(type);
	// wrong expression to research later
	//node->value ??= strdup(value);
	node->value = value ? strdup(value) : NULL;
	node->children = NULL;
	node->child_count = 0;

	return node;
}


void add_child(ASTNode *parent, ASTNode *child){
	parent->children = realloc(parent->children, sizeof(ASTNode *)*(parent->child_count+1));
	parent->children[parent->child_count] = child;
	parent->child_count++;
}


void free_ast(ASTNode *node){
	if (!node) return;
	for(int i=0;i<node->child_count;i++) {
		free_ast(node->children[i]);
	}
	free(node->children);
	free(node->type);
	free(node->value);
	free(node);
}
//?? to revise !

ASTNode *parse_syntax(Token **tokens, int *index){
	Token *current = tokens[*index];
	if(current->type == TOKEN_KEYWORD && strcmp(current->value, "for") == 0){
		return parse_for_loop(tokens,index);
	}
	return NULL;
}


ASTNode *parse_exp(Token **tokens, int *index){
	Token *current = tokens[*index];
	if(current->type == TOKEN_IDENTIFIER || current->type == TOKEN_NUMBER){
		(*index)++;
		return create_ast_node("EXPRESSION", current->value);
	}
	return NULL;
}
ASTNode *parse_for_loop(Token **tokens, int *index){
	Token *current = tokens[*index];
	if(current->type == TOKEN_KEYWORD && strcmp(current->value, "for") == 0){
		(*index)++;
		ASTNode *node = create_ast_node("FOR_LOOP", NULL);

		//next element should be a varialbe (identifier)
		Token *var_token = tokens[*index];
		if (var_token->type == TOKEN_IDENTIFIER){
			add_child(node, create_ast_node("VARIABLE", var_token->value));
			(*index)++;
		}else{
			printf("Syntax error: Expected TOKEN_IDENTIFIER after 'for'.\n");
			free_ast(node);
			node = NULL;
			return NULL;
		}

		//next element should be (in)
		current = tokens[*index];
		if (current->type == TOKEN_KEYWORD && strcmp(current->value, "in") == 0){
			(*index)++;
		}else{
			printf("Syntax error: Expected 'in' after variable %s.\n", tokens[(*index)-1]->value);
			//printf("Received: %s, strcmp(current->value, 'in')= %d\n", current->value, strcmp(current->value, "in"));
			free_ast(node);
			node = NULL;
			return NULL;
		}

		//expect iterable
		ASTNode *iterable = parse_exp(tokens, index);
		if(iterable){
			add_child(node, iterable);
		} else {
			printf("Syntax error: expected iterable after 'in'.\n");
			free_ast(node); //this frees both node and iterable, iterable neing a node's child
			node = NULL;
			return NULL;
		}

		//expect :
		current = tokens[*index];
		if (current->type == TOKEN_PUNCTUATION && strcmp(current->value, ":") == 0){
			(*index)++;
		} else {
			printf("Syntax error: Expected ':' after iterable.\n");
			printf("Received: %s, strcmp(current->value, ':')= %d\n", current->value, strcmp(current->value, ":"));
			free_ast(node);
			return NULL;
		}

		//parse the content
		ASTNode *body = parse_syntax(tokens, index);
		if(body){
			add_child(node, body);
		}else{
			printf("Syntax error: Expected statement after ':'.\n");
			free_ast(node);
			node = NULL;
			return NULL;
		}
		return node;

	}
	return NULL;
}
ASTNode *parse_program(Token **tokens, int *index){
	ASTNode *root = create_ast_node("PROGRAM", NULL);
	while (tokens[*index]->type != TOKEN_EOF){
		ASTNode *stmt = parse_syntax(tokens, index);
		if(stmt){
			add_child(root, stmt);
		}else{
			printf("Syntax error: Invalid syntax.\n");
			free_ast(root);
			root = NULL;
			return NULL;
		}
	}
	return root;
}

