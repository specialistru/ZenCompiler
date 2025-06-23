#include "../../include/parser.h"
#include "../../include/token.h"
#include "../../include/ast.h"
#include "../../include/error.h"
#include <stdlib.h>
#include <string.h>

/**
 * parse_complex_assignment - Парсит сложные операторы присваивания,
 * где левый операнд может быть сложным выражением (доступ к полям,
 * массивам, вызовам функций).
 *
 * Возвращает AST узел присваивания или NULL при ошибке.
 */

ASTNode *parse_complex_assignment(TokenStream *ts) {
    if (!ts) {
        report_error("TokenStream is NULL in parse_complex_assignment");
        return NULL;
    }

    // Парсим левый операнд (может быть сложным, например, itab[...]-field)
    ASTNode *left_expr = parse_lvalue_expression(ts);
    if (!left_expr) {
        report_error("Failed to parse left side of complex assignment");
        return NULL;
    }

    // Ожидаем '='
    Token *assign_token = token_stream_next(ts);
    if (!assign_token || assign_token->type != TOKEN_ASSIGN) {
        report_error("Expected '=' in complex assignment");
        ast_node_free(left_expr);
        return NULL;
    }

    // Парсим правую часть - выражение
    ASTNode *right_expr = parse_expression(ts);
    if (!right_expr) {
        report_error("Failed to parse right side expression in complex assignment");
        ast_node_free(left_expr);
        return NULL;
    }

    // Создаем узел присваивания
    ASTNode *assign_node = ast_node_create(AST_ASSIGNMENT);
    if (!assign_node) {
        report_error("Failed to allocate AST node for complex assignment");
        ast_node_free(left_expr);
        ast_node_free(right_expr);
        return NULL;
    }

    ast_node_add_child(assign_node, left_expr);
    ast_node_add_child(assign_node, right_expr);

    // Проверяем символ конца оператора (точка или точка с запятой)
    Token *end_token = token_stream_next(ts);
    if (!end_token || (end_token->type != TOKEN_DOT && end_token->type != TOKEN_SEMICOLON)) {
        report_error("Expected '.' or ';' at end of complex assignment");
        ast_node_free(assign_node);
        return NULL;
    }

    return assign_node;
}
