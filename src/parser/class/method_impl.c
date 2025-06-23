#include "../../include/parser.h"
#include "../../include/token.h"
#include "../../include/ast.h"
#include "../../include/error.h"
#include <stdlib.h>
#include <string.h>

/**
 * parse_method_implementation - Парсит реализацию метода в классе.
 *
 * Синтаксис:
 * METHOD <method_name>.
 *   <тело метода>
 * ENDMETHOD.
 *
 * Возвращает AST узел реализации метода.
 */
ASTNode *parse_method_implementation(TokenStream *ts) {
    if (!ts) {
        report_error("TokenStream is NULL in parse_method_implementation");
        return NULL;
    }

    Token *token = token_stream_next(ts);
    if (!token || token->type != TOKEN_KEYWORD_METHOD) {
        report_error("Expected 'METHOD' keyword for method implementation");
        return NULL;
    }

    token = token_stream_next(ts);
    if (!token || token->type != TOKEN_IDENTIFIER) {
        report_error("Expected method name after 'METHOD'");
        return NULL;
    }

    ASTNode *method_impl_node = ast_node_create(AST_METHOD_IMPL);
    if (!method_impl_node) {
        report_error("Failed to create AST node for method implementation");
        return NULL;
    }

    method_impl_node->method_name = strdup(token->text);

    // Ожидается точка после METHOD <name>
    token = token_stream_next(ts);
    if (!token || token->type != TOKEN_DOT) {
        report_error("Expected '.' after METHOD name");
        ast_node_free(method_impl_node);
        return NULL;
    }

    // Парсинг тела метода
    method_impl_node->body = parse_statement_block(ts);
    if (!method_impl_node->body) {
        report_error("Failed to parse method body");
        ast_node_free(method_impl_node);
        return NULL;
    }

    // Завершение — ENDMETHOD
    token = token_stream_next(ts);
    if (!token || token->type != TOKEN_KEYWORD_ENDMETHOD) {
        report_error("Expected 'ENDMETHOD' to close method implementation");
        ast_node_free(method_impl_node);
        return NULL;
    }

    token = token_stream_next(ts);
    if (!token || token->type != TOKEN_DOT) {
        report_error("Expected '.' after ENDMETHOD");
        ast_node_free(method_impl_node);
        return NULL;
    }

    return method_impl_node;
}


///////////////////////////


#include "../../include/parser.h"
#include "../../include/token.h"
#include "../../include/ast.h"
#include "../../include/error.h"
#include <stdlib.h>
#include <string.h>

/**
 * parse_method_implementation - Парсит тело метода класса.
 *
 * Синтаксис:
 * METHOD <method_name>.
 *   <тело метода — последовательность операторов и выражений>
 * ENDMETHOD.
 *
 * Возвращает AST узел с телом метода.
 */
ASTNode *parse_method_implementation(TokenStream *ts) {
    if (!ts) {
        report_error("TokenStream is NULL in parse_method_implementation");
        return NULL;
    }

    Token *token = token_stream_next(ts);
    if (!token || token->type != TOKEN_KEYWORD_METHOD) {
        report_error("Expected 'METHOD' keyword");
        return NULL;
    }

    token = token_stream_next(ts);
    if (!token || token->type != TOKEN_IDENTIFIER) {
        report_error("Expected method name after 'METHOD'");
        return NULL;
    }

    ASTNode *method_impl_node = ast_node_create(AST_METHOD_IMPL);
    if (!method_impl_node) {
        report_error("Failed to allocate AST node for method implementation");
        return NULL;
    }

    method_impl_node->method_name = strdup(token->text);
    method_impl_node->body = ast_node_list_create();
    if (!method_impl_node->body) {
        report_error("Failed to create method body list");
        ast_node_free(method_impl_node);
        return NULL;
    }

    token = token_stream_next(ts);
    if (!token || token->type != TOKEN_DOT) {
        report_error("Expected '.' after method name");
        ast_node_free(method_impl_node);
        return NULL;
    }

    // Парсим тело метода до ENDMETHOD.
    while ((token = token_stream_peek(ts)) != NULL) {
        if (token->type == TOKEN_KEYWORD_ENDMETHOD) {
            token_stream_next(ts); // consume ENDMETHOD
            token = token_stream_next(ts);
            if (!token || token->type != TOKEN_DOT) {
                report_error("Expected '.' after ENDMETHOD");
                ast_node_free(method_impl_node);
                return NULL;
            }
            break;
        }

        // TODO: Полный парсинг операторов и выражений в теле метода.
        // Для примера, просто пропускаем токены тела
        token_stream_next(ts);
    }

    return method_impl_node;
}
