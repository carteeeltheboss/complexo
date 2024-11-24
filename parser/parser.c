
#include "parser.h"

// Utility function to create an AST node
ASTNode *create_ast_node(const char *type, const char *value) {
    ASTNode *node = malloc(sizeof(ASTNode));
    if (!node) {
        fprintf(stderr, "Memory allocation failed for AST node\n");
        exit(1);
    }
    node->type = strdup(type);
    node->value = value ? strdup(value) : NULL;
    node->children = NULL;
    node->child_count = 0;
    return node;
}

// Adds a child node to a parent AST node
void add_child(ASTNode *parent, ASTNode *child) {
    parent->children = realloc(parent->children, sizeof(ASTNode *) * (parent->child_count + 1));
    if (!parent->children) {
        fprintf(stderr, "Memory allocation failed when adding child\n");
        exit(1);
    }
    parent->children[parent->child_count++] = child;
}

// Frees memory allocated for an AST node
void free_ast(ASTNode *node) {
    if (!node) return;
    for (int i = 0; i < node->child_count; i++) {
        free_ast(node->children[i]);
    }
    free(node->children);
    free(node->type);
    free(node->value);
    free(node);
}

// Parses an expression (e.g., variables or literals)
ASTNode *parse_exp(Token **tokens, int *index) {
    Token *current = tokens[*index];
    if (current->type == TOKEN_IDENTIFIER || current->type == TOKEN_NUMBER) {
        (*index)++;
        return create_ast_node("EXPRESSION", current->value);
    }
    return NULL;
}
ASTNode *parse_for_loop(Token **tokens, int *index) {
    printf("Entering parse_for_loop. Current token index: %d\n", *index);

    Token *current = tokens[*index];

    // State 0: Expect 'for'
    if (current->type == TOKEN_KEYWORD && strcmp(current->value, "for") == 0) {
        printf("Matched 'for'.\n");
        (*index)++;
    } else {
        fprintf(stderr, "Syntax error: Expected 'for'. Current token: %s\n", current->value);
        return NULL;
    }

    // State 1: Expect loop variable
    current = tokens[*index];
    if (current->type == TOKEN_IDENTIFIER) {
        printf("Matched loop variable: %s.\n", current->value);
        ASTNode *node = create_ast_node("FOR_LOOP", NULL);
        add_child(node, create_ast_node("VARIABLE", current->value));
        (*index)++;

        // State 2: Expect 'in'
        current = tokens[*index];
        if (current->type == TOKEN_KEYWORD && strcmp(current->value, "in") == 0) {
            printf("Matched 'in'.\n");
            (*index)++;

            // State 3: Expect iterable
            ASTNode *iterable = parse_exp(tokens, index);
            if (iterable) {
                printf("Matched iterable.\n");
                add_child(node, iterable);

                // State 4: Expect ':'
                current = tokens[*index];
                if (current->type == TOKEN_PUNCTUATION && strcmp(current->value, ":") == 0) {
                    printf("Matched ':'.\n");
                    (*index)++;

                    // State 5: Parse body
                    ASTNode *body = parse_syntax(tokens, index);
                    if (body) {
                        printf("Matched body of the loop.\n");
                        add_child(node, body);
                        printf("Exiting parse_for_loop successfully. Current token index: %d, value: %s\n", *index, tokens[*index]->value);
                        return node;  // Successfully parsed for loop
                    } else {
                        fprintf(stderr, "Syntax error: Invalid body in 'for' loop.\n");
                    }
                } else {
                    fprintf(stderr, "Syntax error: Expected ':' after iterable. Current token: %s\n", current->value);
                }
            } else {
                fprintf(stderr, "Syntax error: Invalid iterable in 'for' loop.\n");
            }
        } else {
            fprintf(stderr, "Syntax error: Expected 'in' after variable. Current token: %s\n", current->value);
        }

        free_ast(node);
        printf("Exiting parse_for_loop with failure. Current token index: %d, value: %s\n", *index, tokens[*index]->value);
        return NULL;
    } else {
        fprintf(stderr, "Syntax error: Expected loop variable after 'for'. Current token: %s\n", current->value);
        return NULL;
    }
}

ASTNode *parse_if_statement(Token **tokens, int *index) {
    printf("Parsing 'if' statement at index %d\n", *index);

    Token *current = tokens[*index];
    if (current->type == TOKEN_KEYWORD && strcmp(current->value, "if") == 0) {
        (*index)++;
        ASTNode *node = create_ast_node("IF_STATEMENT", NULL);

        // Parse condition
        ASTNode *condition = parse_exp(tokens, index);
        if (!condition) {
            fprintf(stderr, "Syntax error: Invalid condition in 'if' statement.\n");
            free_ast(node);
            return NULL;
        }
        add_child(node, condition);

        // Expect ':'
        current = tokens[*index];
        if (current->type == TOKEN_PUNCTUATION && strcmp(current->value, ":") == 0) {
            (*index)++;
        } else {
            fprintf(stderr, "Syntax error: Expected ':' after 'if' condition.\n");
            free_ast(node);
            return NULL;
        }

        // Parse body
        ASTNode *body = parse_syntax(tokens, index);
        if (!body) {
            fprintf(stderr, "Syntax error: Invalid body in 'if' statement.\n");
            free_ast(node);
            return NULL;
        }
        add_child(node, body);

        return node;
    }

    fprintf(stderr, "Syntax error: Expected 'if'.\n");
    return NULL;
}

ASTNode *parse_while_loop(Token **tokens, int *index) {
    printf("Parsing 'while' loop at index %d\n", *index);

    Token *current = tokens[*index];
    if (current->type == TOKEN_KEYWORD && strcmp(current->value, "while") == 0) {
        (*index)++;
        ASTNode *node = create_ast_node("WHILE_LOOP", NULL);

        // Parse condition
        ASTNode *condition = parse_exp(tokens, index);
        if (!condition) {
            fprintf(stderr, "Syntax error: Invalid condition in 'while' loop.\n");
            free_ast(node);
            return NULL;
        }
        add_child(node, condition);

        // Expect ':'
        current = tokens[*index];
        if (current->type == TOKEN_PUNCTUATION && strcmp(current->value, ":") == 0) {
            (*index)++;
        } else {
            fprintf(stderr, "Syntax error: Expected ':' after 'while' condition.\n");
            free_ast(node);
            return NULL;
        }

        // Parse body
        ASTNode *body = parse_syntax(tokens, index);
        if (!body) {
            fprintf(stderr, "Syntax error: Invalid body in 'while' loop.\n");
            free_ast(node);
            return NULL;
        }
        add_child(node, body);

        return node;
    }

    fprintf(stderr, "Syntax error: Expected 'while'.\n");
    return NULL;
}
ASTNode *parse_function_definition(Token **tokens, int *index) {
    Token *current = tokens[*index];

    if (current->type == TOKEN_KEYWORD && strcmp(current->value, "def") == 0) {
        (*index)++;
        ASTNode *node = create_ast_node("FUNCTION_DEF", NULL);

        // Parse function name
        current = tokens[*index];
        if (current->type == TOKEN_IDENTIFIER) {
            add_child(node, create_ast_node("FUNCTION_NAME", current->value));
            (*index)++;
        } else {
            fprintf(stderr, "Syntax error: Expected function name after 'def'.\n");
            free_ast(node);
            return NULL;
        }

        // Expect opening parentheses for parameters
        current = tokens[*index];
        if (current->type == TOKEN_PUNCTUATION && strcmp(current->value, "(") == 0) {
            (*index)++;
        } else {
            fprintf(stderr, "Syntax error: Expected '(' after function name.\n");
            free_ast(node);
            return NULL;
        }

        // Parse parameters (handling multiple parameters and commas)
        while (1) {
            current = tokens[*index];

            // If we encounter a closing parenthesis, stop parsing parameters
            if (current->type == TOKEN_PUNCTUATION && strcmp(current->value, ")") == 0) {
                (*index)++;
                break;
            }

            // Expect a parameter name (identifier)
            if (current->type == TOKEN_IDENTIFIER) {
                add_child(node, create_ast_node("PARAMETER", current->value));
                (*index)++;
            } else {
                fprintf(stderr, "Syntax error: Expected parameter name in function definition.\n");
                free_ast(node);
                return NULL;
            }

            // If the next token is a comma, continue parsing for the next parameter
            current = tokens[*index];
            if (current->type == TOKEN_PUNCTUATION && strcmp(current->value, ",") == 0) {
                (*index)++;
                continue;
            }

            // If we encounter a closing parenthesis, stop parsing parameters
            if (current->type == TOKEN_PUNCTUATION && strcmp(current->value, ")") == 0) {
                (*index)++;
                break;
            }

            // If something unexpected occurs, raise an error
            fprintf(stderr, "Syntax error: Expected ',' or ')' in function parameter list.\n");
            free_ast(node);
            return NULL;
        }

        // Expect colon (':') after function definition
        current = tokens[*index];
        if (current->type == TOKEN_PUNCTUATION && strcmp(current->value, ":") == 0) {
            (*index)++;
        } else {
            fprintf(stderr, "Syntax error: Expected ':' after function definition.\n");
            free_ast(node);
            return NULL;
        }

        // Parse the function body (this could be a block of statements)
        ASTNode *body = parse_syntax(tokens, index);
        if (body) {
            add_child(node, body);
        } else {
            fprintf(stderr, "Syntax error: Expected body after function definition.\n");
            free_ast(node);
            return NULL;
        }

        return node;
    }

    return NULL;
}



ASTNode *parse_return_statement(Token **tokens, int *index) {
    printf("Parsing 'return' statement at index %d\n", *index);

    Token *current = tokens[*index];
    if (current->type == TOKEN_KEYWORD && strcmp(current->value, "return") == 0) {
        (*index)++;
        ASTNode *node = create_ast_node("RETURN_STATEMENT", NULL);

        // Parse return value (optional)
        ASTNode *return_value = parse_exp(tokens, index);
        if (return_value) {
            add_child(node, return_value);
        }

        return node;
    }

    fprintf(stderr, "Syntax error: Expected 'return'.\n");
    return NULL;
}

ASTNode *parse_index_expression(Token **tokens, int *index) {
    // Current token should be the variable being indexed
    Token *current = tokens[*index];
    if (current->type != TOKEN_IDENTIFIER) {
        fprintf(stderr, "Syntax error: Expected an identifier before '['.\n");
        return NULL;
    }

    // Create the base node for the variable
    ASTNode *node = create_ast_node("INDEX_EXPRESSION", current->value);
    (*index)++;

    // Expect '['
    current = tokens[*index];
    if (current->type != TOKEN_PUNCTUATION || strcmp(current->value, "[") != 0) {
        fprintf(stderr, "Syntax error: Expected '[' after identifier.\n");
        free_ast(node);
        return NULL;
    }
    (*index)++;

    // Parse the index expression inside the brackets
    ASTNode *index_expr = parse_exp(tokens, index);
    if (!index_expr) {
        fprintf(stderr, "Syntax error: Expected an index expression inside '[]'.\n");
        free_ast(node);
        return NULL;
    }
    add_child(node, index_expr);

    // Expect ']'
    current = tokens[*index];
    if (current->type != TOKEN_PUNCTUATION || strcmp(current->value, "]") != 0) {
        fprintf(stderr, "Syntax error: Expected ']' after index expression.\n");
        free_ast(node);
        free_ast(index_expr);
        return NULL;
    }
    (*index)++;

    return node; 
}

ASTNode *parse_assignment(Token **tokens, int *index) {
    // Parse the left-hand side of the assignment
    ASTNode *lhs = parse_index_expression(tokens, index);  // This supports `x[i]`
    if (!lhs) {
        fprintf(stderr, "Syntax error: Invalid left-hand side in assignment.\n");
        return NULL;
    }

    // Expect '='
    Token *current = tokens[*index];
    if (current->type != TOKEN_PUNCTUATION || strcmp(current->value, "=") != 0) {
        fprintf(stderr, "Syntax error: Expected '=' in assignment.\n");
        free_ast(lhs);
        return NULL;
    }
    (*index)++;

    // Parse the right-hand side expression
    ASTNode *rhs = parse_exp(tokens, index);
    if (!rhs) {
        fprintf(stderr, "Syntax error: Invalid right-hand side in assignment.\n");
        free_ast(lhs);
        return NULL;
    }

    ASTNode *node = create_ast_node("ASSIGNMENT", NULL);
    add_child(node, lhs);
    add_child(node, rhs);

    return node;
}


ASTNode *parse_syntax(Token **tokens, int *index) {
    Token *current = tokens[*index];
    printf("Parsing syntax at index %d: Token type=%d, Token value=%s\n", *index, current->type, current->value);

    if (current->type == TOKEN_IDENTIFIER) {
        // Peek ahead to check for '[' or '='
        Token *next = tokens[*index + 1];
        if (next->type == TOKEN_PUNCTUATION && strcmp(next->value, "[") == 0) {
            return parse_assignment(tokens, index);
        }
    }
if (current->type == TOKEN_KEYWORD) {
    switch (hash(current->value)) {
        case hash("for"):
            return parse_for_loop(tokens, index);
        case hash("def"):
            return parse_function_definition(tokens, index);
        case hash("if"):
            return parse_if_statement(tokens, index);
        case hash("while"):
            return parse_while_loop(tokens, index);
        case hash("return"):
            return parse_return_statement(tokens, index);
        case hash("and"):
            (*index)++;
            return create_ast_node("LOGICAL_OP", "and");
        case hash("or"):
            (*index)++;
            return create_ast_node("LOGICAL_OP", "or");
        case hash("not"):
            (*index)++;
            return create_ast_node("LOGICAL_OP", "not");
        case hash("pass"):
            (*index)++;
            return create_ast_node("PASS", NULL);
        case hash("break"):
            (*index)++;
            return create_ast_node("BREAK", NULL);
        case hash("continue"):
            (*index)++;
            return create_ast_node("CONTINUE", NULL);
        case hash("assert"):
            (*index)++;
            return create_ast_node("ASSERT", NULL);
        case hash("None"):
            (*index)++;
            return create_ast_node("VALUE", "None");
        case hash("True"):
        case hash("False"):
            (*index)++;
            return create_ast_node("BOOLEAN", current->value); 
        case hash("lambda"):
            (*index)++;
            return create_ast_node("LAMBDA", NULL);
    }
}
// ... rest of your code ...

    if (current->type == TOKEN_NEWLINE) {
        (*index)++;
        return parse_syntax(tokens, index); 
    }

    if (current->type == TOKEN_INDENT) {
        ASTNode *body = parse_syntax(tokens, index);
        if (body) {
            return body;
        } else {
            fprintf(stderr, "Syntax error: Expected indented block.\n");
            return NULL;
        }
    }

    // If none of the above, handle expressions as standalone statements (e.g., function calls)
    ASTNode *expr = parse_exp(tokens, index);
    if (expr) {
        printf("Parsed standalone expression: %s\n", expr->value);
        return expr;
    }

    // Unrecognized syntax
    fprintf(stderr, "Syntax error: Unrecognized syntax at index %d: %s\n", *index, current->value);
    return NULL;
}


// Parses an entire Python program into an AST
ASTNode *parse_program(Token **tokens, int *index) {
    ASTNode *root = create_ast_node("PROGRAM", NULL);
    while (tokens[*index]->type != TOKEN_EOF) {
        ASTNode *stmt = parse_syntax(tokens, index);
        if (stmt) {
            add_child(root, stmt);
        } else {
            fprintf(stderr, "Syntax error: Invalid syntax\n");
            free_ast(root);
            return NULL;
        }
    }
    return root;
}
