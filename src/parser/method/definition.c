#include "../../include/parser.h"
#include "../../include/token.h"
#include "../../include/ast.h"
#include "../../include/error.h"
#include <stdlib.h>
#include <string.h>

/**
 * parse_method_definition - Парсит определение метода.
 *
 * Ожидается синтаксис:
 * METHOD method_name.
 *   "тело метода"
 * ENDMETHOD.
 *
 * Возвращает ASTNode с описанием метода или NULL при ошибке.
 */
ASTNode *parse_method_definition(TokenStream *ts) {
    if (!ts) {
        report_error("TokenStream is NULL in parse_method_definition");
        return NULL;
    }

    Token *token = token_stream_next(ts);
    if (!token || token->type != TOKEN_KEYWORD_METHOD) {
        report_error("Expected 'METHOD' keyword at start of method definition");
        return NULL;
    }

    token = token_stream_next(ts);
    if (!token || token->type != TOKEN_IDENTIFIER) {
        report_error("Expected method name identifier after 'METHOD'");
        return NULL;
    }

    ASTNode *method_node = ast_node_create(AST_METHOD_DEFINITION);
    if (!method_node) {
        report_error("Failed to allocate AST node for method definition");
        return NULL;
    }

    method_node->method_name = strdup(token->text);
    if (!method_node->method_name) {
        report_error("Failed to allocate memory for method name");
        ast_node_free(method_node);
        return NULL;
    }

    // Пропускаем тело метода (упрощённо — реальный парсер должен обрабатывать содержимое)
    while ((token = token_stream_next(ts)) != NULL) {
        if (token->type == TOKEN_KEYWORD_ENDMETHOD) {
            Token *dot_token = token_stream_next(ts);
            if (!dot_token || dot_token->type != TOKEN_DOT) {
                report_error("Expected '.' after 'ENDMETHOD'");
                ast_node_free(method_node);
                return NULL;
            }
            break;
        }
        // Тут можно добавить парсинг тела метода, если нужно
    }

    return method_node;
}
