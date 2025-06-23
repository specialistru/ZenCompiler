#include "../../include/parser.h"
#include "../../include/token.h"
#include "../../include/ast.h"
#include "../../include/error.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * parse_if_body - Парсит тело IF, пока не встретит ELSE, ELSEIF или ENDIF.
 *
 * Возвращает список AST узлов, или NULL при ошибке.
 */
ASTNodeList *parse_if_body(TokenStream *ts) {
    if (!ts) {
        report_error("TokenStream is NULL in parse_if_body");
        return NULL;
    }

    ASTNodeList *body_list = ast_node_list_create();
    if (!body_list) {
        report_error("Failed to create ASTNodeList for IF body");
        return NULL;
    }

    while (1) {
        Token *token = token_stream_peek(ts);
        if (!token) {
            report_error("Unexpected end of input inside IF body");
            ast_node_list_free(body_list);
            return NULL;
        }

        if (token->type == TOKEN_KEYWORD_ELSE ||
            token->type == TOKEN_KEYWORD_ELSEIF ||
            token->type == TOKEN_KEYWORD_ENDIF) {
            // Конец тела IF
            break;
        }

        ASTNode *stmt = parse_statement(ts);
        if (!stmt) {
            report_error("Failed to parse statement inside IF body");
            ast_node_list_free(body_list);
            return NULL;
        }

        ast_node_list_append(body_list, stmt);
    }

    return body_list;
}
