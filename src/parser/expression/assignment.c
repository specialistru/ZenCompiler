#include "../../include/parser.h"
#include "../../include/token.h"
#include "../../include/ast.h"
#include "../../include/error.h"
#include <stdlib.h>

/**
 * parse_assignment_expression - Парсит выражение присваивания.
 *
 * Формат: идентификатор = выражение
 * 
 * Возвращает AST узел присваивания или NULL при ошибке.
 */
ASTNode *parse_assignment_expression(TokenStream *ts) {
    if (!ts) {
        report_error("TokenStream is NULL in parse_assignment_expression");
        return NULL;
    }

    // Сохраняем позицию для отката
    size_t pos_backup = token_stream_get_position(ts);

    Token *token = token_stream_next(ts);
    if (!token || token->type != TOKEN_IDENTIFIER) {
        token_stream_set_position(ts, pos_backup);
        return NULL;  // Не присваивание, возможно другое выражение
    }

    // Проверяем следующий токен — '='
    Token *next_token = token_stream_peek(ts);
    if (!next_token || next_token->type != TOKEN_ASSIGN) {
        token_stream_set_position(ts, pos_backup);
        return NULL;
    }
    token_stream_next(ts); // consume '='

    // Парсим выражение справа от '='
    ASTNode *expr_right = parse_expression(ts);
    if (!expr_right) {
        report_error("Failed to parse right-hand side expression in assignment");
        token_stream_set_position(ts, pos_backup);
        return NULL;
    }

    // Создаем узел присваивания
    ASTNode *assign_node = ast_node_create(AST_ASSIGNMENT);
    if (!assign_node) {
        report_error("Failed to allocate AST node for assignment");
        ast_node_free(expr_right);
        return NULL;
    }

    // Левый операнд — переменная (идентификатор)
    ASTNode *var_node = ast_node_create(AST_VARIABLE);
    if (!var_node) {
        report_error("Failed to allocate AST node for variable in assignment");
        ast_node_free(expr_right);
        ast_node_free(assign_node);
        return NULL;
    }
    var_node->string_value = strdup(token->text);
    if (!var_node->string_value) {
        report_error("Failed to allocate memory for variable name");
        ast_node_free(expr_right);
        ast_node_free(assign_node);
        ast_node_free(var_node);
        return NULL;
    }

    ast_node_add_child(assign_node, var_node);
    ast_node_add_child(assign_node, expr_right);

    return assign_node;
}
