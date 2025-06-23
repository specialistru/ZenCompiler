#include "../../include/parser.h"
#include "../../include/token.h"
#include "../../include/ast.h"
#include "../../include/error.h"
#include <stdlib.h>

/**
 * parse_function_call - Парсит вызов функции.
 *
 * Ожидается идентификатор (имя функции), за которым следует
 * открывающая скобка, список аргументов (выражений) через запятую и закрывающая скобка.
 *
 * Возвращает AST узел вызова функции или NULL при ошибке.
 */
ASTNode *parse_function_call(TokenStream *ts) {
    if (!ts) {
        report_error("TokenStream is NULL in parse_function_call");
        return NULL;
    }

    Token *token = token_stream_next(ts);
    if (!token || token->type != TOKEN_IDENTIFIER) {
        report_error("Expected function name identifier");
        return NULL;
    }

    ASTNode *func_call_node = ast_node_create(AST_FUNCTION_CALL);
    if (!func_call_node) {
        report_error("Failed to allocate AST node for function call");
        return NULL;
    }

    func_call_node->string_value = strdup(token->text);
    if (!func_call_node->string_value) {
        report_error("Failed to allocate memory for function name");
        ast_node_free(func_call_node);
        return NULL;
    }

    token = token_stream_next(ts);
    if (!token || token->type != TOKEN_LPAREN) {
        report_error("Expected '(' after function name");
        ast_node_free(func_call_node);
        return NULL;
    }

    // Парсим аргументы функции (возможно 0 и более)
    while (true) {
        token = token_stream_peek(ts);
        if (!token) {
            report_error("Unexpected end of tokens in function arguments");
            ast_node_free(func_call_node);
            return NULL;
        }

        if (token->type == TOKEN_RPAREN) {
            token_stream_next(ts); // consume ')'
            break;
        }

        // Парсим выражение аргумента
        ASTNode *arg = parse_expression(ts);
        if (!arg) {
            report_error("Failed to parse function argument");
            ast_node_free(func_call_node);
            return NULL;
        }
        ast_node_add_child(func_call_node, arg);

        token = token_stream_peek(ts);
        if (!token) {
            report_error("Unexpected end of tokens in function arguments");
            ast_node_free(func_call_node);
            return NULL;
        }

        if (token->type == TOKEN_COMMA) {
            token_stream_next(ts); // consume ','
            continue;
        } else if (token->type == TOKEN_RPAREN) {
            token_stream_next(ts); // consume ')'
            break;
        } else {
            report_error("Expected ',' or ')' in function arguments");
            ast_node_free(func_call_node);
            return NULL;
        }
    }

    return func_call_node;
}
