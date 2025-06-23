#include "../../include/parser.h"
#include "../../include/token.h"
#include "../../include/ast.h"
#include "../../include/error.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * parse_bracketed_expression - Парсит выражение, заключённое в круглые скобки.
 *
 * Возвращает AST узел выражения внутри скобок или NULL при ошибке.
 */
ASTNode *parse_bracketed_expression(TokenStream *ts) {
    if (!ts) {
        fprintf(stderr, "TokenStream is NULL\n");
        return NULL;
    }

    Token *token = token_stream_next(ts);
    if (!token || token->type != TOKEN_LPAREN) {
        report_error("Expected '(' at start of bracketed expression");
        return NULL;
    }

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
