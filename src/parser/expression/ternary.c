#include "../../include/parser.h"
#include "../../include/token.h"
#include "../../include/ast.h"
#include "../../include/error.h"
#include <stdlib.h>

/**
 * parse_ternary_expression - Парсит тернарный оператор.
 *
 * Формат: condition ? expr_true : expr_false
 * 
 * Возвращает AST узел тернарного оператора или NULL при ошибке.
 */
ASTNode *parse_ternary_expression(TokenStream *ts) {
    if (!ts) {
        report_error("TokenStream is NULL in parse_ternary_expression");
        return NULL;
    }

    // Парсим условие (выражение)
    ASTNode *condition = parse_expression(ts);
    if (!condition) {
        report_error("Failed to parse condition in ternary expression");
        return NULL;
    }

    Token *token = token_stream_peek(ts);
    if (!token || token->type != TOKEN_QUESTION) {
        // Не тернарный оператор, возвращаем условие как есть
        return condition;
    }

    token_stream_next(ts); // consume '?'

    // Парсим выражение при true
    ASTNode *true_expr = parse_expression(ts);
    if (!true_expr) {
        report_error("Failed to parse true expression in ternary operator");
        ast_node_free(condition);
        return NULL;
    }

    token = token_stream_next(ts);
    if (!token || token->type != TOKEN_COLON) {
        report_error("Expected ':' in ternary operator");
        ast_node_free(condition);
        ast_node_free(true_expr);
        return NULL;
    }

    // Парсим выражение при false
    ASTNode *false_expr = parse_expression(ts);
    if (!false_expr) {
        report_error("Failed to parse false expression in ternary operator");
        ast_node_free(condition);
        ast_node_free(true_expr);
        return NULL;
    }

    ASTNode *ternary_node = ast_node_create(AST_TERNARY_OPERATOR);
    if (!ternary_node) {
        report_error("Failed to allocate AST node for ternary operator");
        ast_node_free(condition);
        ast_node_free(true_expr);
        ast_node_free(false_expr);
        return NULL;
    }

    ast_node_add_child(ternary_node, condition);
    ast_node_add_child(ternary_node, true_expr);
    ast_node_add_child(ternary_node, false_expr);

    return ternary_node;
}
