#include "../../include/parser.h"
#include "../../include/token.h"
#include "../../include/ast.h"
#include "../../include/error.h"
#include <stdlib.h>

/**
 * parse_logical_expression - Парсит логические выражения с операторами AND, OR, NOT.
 *
 * Реализован разбор с учетом приоритетов:
 * - NOT имеет наивысший приоритет
 * - AND следующий
 * - OR самый низкий
 *
 * Возвращает AST узел логического выражения или NULL при ошибке.
 */

// Вспомогательные функции для рекурсивного разбора по приоритетам
static ASTNode *parse_logical_or(TokenStream *ts);
static ASTNode *parse_logical_and(TokenStream *ts);
static ASTNode *parse_logical_not(TokenStream *ts);
static ASTNode *parse_logical_primary(TokenStream *ts);

ASTNode *parse_logical_expression(TokenStream *ts) {
    if (!ts) {
        report_error("TokenStream is NULL in parse_logical_expression");
        return NULL;
    }
    return parse_logical_or(ts);
}

static ASTNode *parse_logical_or(TokenStream *ts) {
    ASTNode *left = parse_logical_and(ts);
    if (!left) return NULL;

    while (true) {
        Token *token = token_stream_peek(ts);
        if (!token || token->type != TOKEN_OR) break;

        token_stream_next(ts); // consume OR

        ASTNode *right = parse_logical_and(ts);
        if (!right) {
            ast_node_free(left);
            return NULL;
        }

        ASTNode *or_node = ast_node_create(AST_LOGICAL_OR);
        if (!or_node) {
            report_error("Failed to allocate AST node for OR");
            ast_node_free(left);
            ast_node_free(right);
            return NULL;
        }
        ast_node_add_child(or_node, left);
        ast_node_add_child(or_node, right);
        left = or_node;
    }
    return left;
}

static ASTNode *parse_logical_and(TokenStream *ts) {
    ASTNode *left = parse_logical_not(ts);
    if (!left) return NULL;

    while (true) {
        Token *token = token_stream_peek(ts);
        if (!token || token->type != TOKEN_AND) break;

        token_stream_next(ts); // consume AND

        ASTNode *right = parse_logical_not(ts);
        if (!right) {
            ast_node_free(left);
            return NULL;
        }

        ASTNode *and_node = ast_node_create(AST_LOGICAL_AND);
        if (!and_node) {
            report_error("Failed to allocate AST node for AND");
            ast_node_free(left);
            ast_node_free(right);
            return NULL;
        }
        ast_node_add_child(and_node, left);
        ast_node_add_child(and_node, right);
        left = and_node;
    }
    return left;
}

static ASTNode *parse_logical_not(TokenStream *ts) {
    Token *token = token_stream_peek(ts);
    if (token && token->type == TOKEN_NOT) {
        token_stream_next(ts); // consume NOT
        ASTNode *operand = parse_logical_not(ts);
        if (!operand) return NULL;

        ASTNode *not_node = ast_node_create(AST_LOGICAL_NOT);
        if (!not_node) {
            report_error("Failed to allocate AST node for NOT");
            ast_node_free(operand);
            return NULL;
        }
        ast_node_add_child(not_node, operand);
        return not_node;
    }
    return parse_logical_primary(ts);
}

static ASTNode *parse_logical_primary(TokenStream *ts) {
    Token *token = token_stream_peek(ts);
    if (!token) {
        report_error("Unexpected end of tokens in logical expression");
        return NULL;
    }

    if (token->type == TOKEN_LPAREN) {
        token_stream_next(ts); // consume '('
        ASTNode *expr = parse_logical_expression(ts);
        if (!expr) return NULL;

        token = token_stream_next(ts);
        if (!token || token->type != TOKEN_RPAREN) {
            report_error("Expected ')' after logical expression");
            ast_node_free(expr);
            return NULL;
        }
        return expr;
    }

    // Если нет скобок, парсим простой идентификатор или литерал
    return parse_expression(ts);
}
