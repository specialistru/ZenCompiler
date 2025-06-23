#include "../../include/parser.h"
#include "../../include/token.h"
#include "../../include/ast.h"
#include "../../include/error.h"
#include <stdlib.h>

/**
 * parse_perform_bracketed - Парсит выражение, заключённое в круглые скобки.
 *
 * Синтаксис:
 * '(' <выражение> ')'
 *
 * Возвращает AST узел выражения внутри скобок или NULL при ошибке.
 */
ASTNode *parse_perform_bracketed(TokenStream *ts) {
    if (!ts) {
        report_error("TokenStream is NULL in parse_perform_bracketed");
        return NULL;
    }

    Token *token = token_stream_next(ts);
    if (!token || token->type != TOKEN_LPAREN) {
        report_error("Expected '(' at start of bracketed expression");
        return NULL;
    }

    // Парсим выражение внутри скобок
    ASTNode *expr = parse_expression(ts);
    if (!expr) {
        report_error("Failed to parse expression inside brackets");
        return NULL;
    }

    token = token_stream_next(ts);
    if (!token || token->type != TOKEN_RPAREN) {
        report_error("Expected ')' at end of bracketed expression");
        ast_node_free(expr);
        return NULL;
    }

    return expr;
}
