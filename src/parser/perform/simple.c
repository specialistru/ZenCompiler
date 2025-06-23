#include "../../include/parser.h"
#include "../../include/token.h"
#include "../../include/ast.h"
#include "../../include/error.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * parse_perform_simple - Парсит простой оператор PERFORM.
 *
 * Поддерживает синтаксис:
 * PERFORM <form_name>.
 *
 * Возвращает AST узел или NULL при ошибке.
 */
ASTNode *parse_perform_simple(TokenStream *ts) {
    if (!ts) {
        report_error("TokenStream is NULL in parse_perform_simple");
        return NULL;
    }

    Token *token = token_stream_next(ts);
    if (!token || token->type != TOKEN_KEYWORD_PERFORM) {
        report_error("Expected PERFORM keyword");
        return NULL;
    }

    // Следующий токен — имя формы (идентификатор)
    token = token_stream_next(ts);
    if (!token || token->type != TOKEN_IDENTIFIER) {
        report_error("Expected form name identifier after PERFORM");
        return NULL;
    }

    ASTNode *perform_node = ast_node_create(AST_PERFORM);
    if (!perform_node) {
        report_error("Failed to create AST node for PERFORM");
        return NULL;
    }

    perform_node->perform_form_name = strdup(token->text);
    if (!perform_node->perform_form_name) {
        report_error("Memory allocation failed for form name");
        ast_node_free(perform_node);
        return NULL;
    }

    // Ожидаем точку (конец оператора)
    token = token_stream_next(ts);
    if (!token || token->type != TOKEN_DOT) {
        report_error("Expected '.' after PERFORM form name");
        ast_node_free(perform_node);
        return NULL;
    }

    return perform_node;
}
