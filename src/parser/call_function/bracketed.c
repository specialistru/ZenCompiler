#include "../../include/parser.h"
#include "../../include/token.h"
#include "../../include/ast.h"
#include "../../include/error.h"
#include <stdlib.h>

/**
 * parse_call_function_bracketed - Парсит вызов функции, где имя задано в скобках.
 *
 * Пример:
 * CALL FUNCTION (lv_func_name)
 *
 * Возвращает AST узел или NULL при ошибке.
 */
ASTNode *parse_call_function_bracketed(TokenStream *ts) {
    if (!ts) {
        report_error("TokenStream is NULL in parse_call_function_bracketed");
        return NULL;
    }

    Token *token = token_stream_next(ts);
    if (!token || token->type != TOKEN_KEYWORD_CALL) {
        report_error("Expected CALL keyword");
        return NULL;
    }

    token = token_stream_next(ts);
    if (!token || token->type != TOKEN_KEYWORD_FUNCTION) {
        report_error("Expected FUNCTION keyword");
        return NULL;
    }

    token = token_stream_next(ts);
    if (!token || token->type != TOKEN_LPAREN) {
        report_error("Expected '(' after FUNCTION keyword");
        return NULL;
    }

    ASTNode *expr = parse_expression(ts);
    if (!expr) {
        report_error("Failed to parse expression inside brackets for function name");
        return NULL;
    }

    token = token_stream_next(ts);
    if (!token || token->type != TOKEN_RPAREN) {
        report_error("Expected ')' after dynamic function expression");
        ast_node_free(expr);
        return NULL;
    }

    ASTNode *call_node = ast_node_create(AST_CALL_FUNCTION_DYNAMIC);
    if (!call_node) {
        report_error("Failed to allocate AST node for bracketed function call");
        ast_node_free(expr);
        return NULL;
    }

    call_node->dynamic_function_expr = expr;

    return call_node;
}
