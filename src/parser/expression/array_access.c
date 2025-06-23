#include "../../include/parser.h"
#include "../../include/token.h"
#include "../../include/ast.h"
#include "../../include/error.h"
#include <stdlib.h>

/**
 * parse_array_access - Парсит выражение доступа к элементу массива.
 *
 * Формат: идентификатор [ индекс ]
 * Возвращает AST узел доступа к массиву или NULL при ошибке.
 */
ASTNode *parse_array_access(TokenStream *ts) {
    if (!ts) {
        report_error("TokenStream is NULL in parse_array_access");
        return NULL;
    }

    // Сохраняем позицию для отката
    size_t pos_backup = token_stream_get_position(ts);

    // Парсим идентификатор массива
    Token *token = token_stream_next(ts);
    if (!token || token->type != TOKEN_IDENTIFIER) {
        token_stream_set_position(ts, pos_backup);
        return NULL;
    }

    // Проверяем наличие открывающей квадратной скобки
    Token *next_token = token_stream_peek(ts);
    if (!next_token || next_token->type != TOKEN_LBRACKET) {
        token_stream_set_position(ts, pos_backup);
        return NULL;
    }
    token_stream_next(ts); // consume '['

    // Парсим индекс (выражение)
    ASTNode *index_expr = parse_expression(ts);
    if (!index_expr) {
        report_error("Failed to parse index expression in array access");
        token_stream_set_position(ts, pos_backup);
        return NULL;
    }

    // Проверяем закрывающую квадратную скобку
    Token *closing_token = token_stream_next(ts);
    if (!closing_token || closing_token->type != TOKEN_RBRACKET) {
        report_error("Expected ']' after index expression in array access");
        ast_node_free(index_expr);
        token_stream_set_position(ts, pos_backup);
        return NULL;
    }

    // Создаем AST узел доступа к массиву
    ASTNode *array_access_node = ast_node_create(AST_ARRAY_ACCESS);
    if (!array_access_node) {
        report_error("Failed to allocate AST node for array access");
        ast_node_free(index_expr);
        return NULL;
    }

    // Узел переменной с именем массива
    ASTNode *array_var_node = ast_node_create(AST_VARIABLE);
    if (!array_var_node) {
        report_error("Failed to allocate AST node for array variable");
        ast_node_free(index_expr);
        ast_node_free(array_access_node);
        return NULL;
    }
    array_var_node->string_value = strdup(token->text);
    if (!array_var_node->string_value) {
        report_error("Failed to allocate memory for array variable name");
        ast_node_free(index_expr);
        ast_node_free(array_access_node);
        ast_node_free(array_var_node);
        return NULL;
    }

    // Добавляем в узел доступа переменную и индекс
    ast_node_add_child(array_access_node, array_var_node);
    ast_node_add_child(array_access_node, index_expr);

    return array_access_node;
}
