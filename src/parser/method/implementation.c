#include "../../include/parser.h"
#include "../../include/token.h"
#include "../../include/ast.h"
#include "../../include/error.h"
#include <stdlib.h>
#include <string.h>

/**
 * parse_method_implementation - Парсит реализацию метода.
 *
 * Ожидается структура:
 * METHOD method_name.
 *   ... тело метода ...
 * ENDMETHOD.
 *
 * Возвращает AST узел с телом метода или NULL при ошибке.
 */
ASTNode *parse_method_implementation(TokenStream *ts) {
    if (!ts) {
        report_error("TokenStream is NULL in parse_method_implementation");
        return NULL;
    }

    Token *token = token_stream_next(ts);
    if (!token || token->type != TOKEN_KEYWORD_METHOD) {
        report_error("Expected 'METHOD' keyword to start method implementation");
        return NULL;
    }

    token = token_stream_next(ts);
    if (!token || token->type != TOKEN_IDENTIFIER) {
        report_error("Expected method name after 'METHOD'");
        return NULL;
    }

    ASTNode *method_impl_node = ast_node_create(AST_METHOD_IMPLEMENTATION);
    if (!method_impl_node) {
        report_error("Failed to allocate AST node for method implementation");
        return NULL;
    }

    method_impl_node->method_name = strdup(token->text);
    if (!method_impl_node->method_name) {
        report_error("Failed to allocate memory for method name");
        ast_node_free(method_impl_node);
        return NULL;
    }

    // Парсим тело метода до ENDMETHOD.
    ASTNodeList *body_statements = ast_node_list_create();
    if (!body_statements) {
        report_error("Failed to allocate AST node list for method body");
        ast_node_free(method_impl_node);
        return NULL;
    }

    Token *next_token;
    while ((next_token = token_stream_peek(ts)) != NULL) {
        if (next_token->type == TOKEN_KEYWORD_ENDMETHOD) {
            token_stream_next(ts); // consume ENDMETHOD
            Token *dot_token = token_stream_next(ts);
            if (!dot_token || dot_token->type != TOKEN_DOT) {
                report_error("Expected '.' after ENDMETHOD");
                ast_node_list_free(body_statements);
                ast_node_free(method_impl_node);
                return NULL;
            }
            break;
        }

        // Здесь можно вызвать функции парсинга инструкций тела
        // Для упрощения просто пропускаем токены тела
        token_stream_next(ts);
    }

    method_impl_node->body = body_statements;

    return method_impl_node;
}
