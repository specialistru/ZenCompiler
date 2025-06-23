#include "../../include/parser.h"
#include "../../include/token.h"
#include "../../include/ast.h"
#include "../../include/error.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * parse_if_else - Парсит оператор IF с веткой ELSE.
 *
 * Возвращает AST узел IF с телом и веткой ELSE или NULL при ошибке.
 */
ASTNode *parse_if_else(TokenStream *ts) {
    if (!ts) {
        fprintf(stderr, "TokenStream is NULL\n");
        return NULL;
    }

    Token *token = token_stream_peek(ts);
    if (!token || token->type != TOKEN_KEYWORD_IF) {
        report_error("Expected IF keyword");
        return NULL;
    }
    token_stream_next(ts); // съесть IF

    ASTNode *if_node = ast_node_create(AST_IF);
    if (!if_node) {
        report_error("Failed to create AST IF node");
        return NULL;
    }

    ASTNode *condition = parse_expression(ts);
    if (!condition) {
        report_error("Failed to parse IF condition");
        ast_node_free(if_node);
        return NULL;
    }

    token = token_stream_next(ts);
    if (!token || token->type != TOKEN_DOT) {
        report_error("Expected '.' after IF condition");
        ast_node_free(condition);
        ast_node_free(if_node);
        return NULL;
    }

    if_node->if_condition = condition;
    if_node->if_body = ast_node_list_create();
    if (!if_node->if_body) {
        report_error("Failed to create ASTNodeList for IF body");
        ast_node_free(if_node);
        return NULL;
    }

    while (1) {
        token = token_stream_peek(ts);
        if (!token) {
            report_error("Unexpected end of input inside IF body");
            ast_node_free(if_node);
            return NULL;
        }

        if (token->type == TOKEN_KEYWORD_ELSE || token->type == TOKEN_KEYWORD_ENDIF) {
            break;
        }

        ASTNode *stmt = parse_statement(ts);
        if (!stmt) {
            report_error("Failed to parse statement inside IF body");
            ast_node_free(if_node);
            return NULL;
        }
        ast_node_list_append(if_node->if_body, stmt);
    }

    if (token_stream_peek(ts)->type == TOKEN_KEYWORD_ELSE) {
        token_stream_next(ts); // съесть ELSE

        token = token_stream_next(ts);
        if (!token || token->type != TOKEN_DOT) {
            report_error("Expected '.' after ELSE");
            ast_node_free(if_node);
            return NULL;
        }

        if_node->else_body = ast_node_list_create();
        if (!if_node->else_body) {
            report_error("Failed to create ASTNodeList for ELSE body");
            ast_node_free(if_node);
            return NULL;
        }

        while (1) {
            token = token_stream_peek(ts);
            if (!token) {
                report_error("Unexpected end of input inside ELSE body");
                ast_node_free(if_node);
                return NULL;
            }

            if (token->type == TOKEN_KEYWORD_ENDIF) {
                break;
            }

            ASTNode *stmt = parse_statement(ts);
            if (!stmt) {
                report_error("Failed to parse statement inside ELSE body");
                ast_node_free(if_node);
                return NULL;
            }
            ast_node_list_append(if_node->else_body, stmt);
        }
    }

    token = token_stream_next(ts);
    if (!token || token->type != TOKEN_KEYWORD_ENDIF) {
        report_error("Expected ENDIF keyword");
        ast_node_free(if_node);
        return NULL;
    }

    token = token_stream_next(ts);
    if (!token || token->type != TOKEN_DOT) {
        report_error("Expected '.' after ENDIF");
        ast_node_free(if_node);
        return NULL;
    }

    return if_node;
}
