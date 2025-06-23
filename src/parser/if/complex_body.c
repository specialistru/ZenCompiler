#include "../../include/parser.h"
#include "../../include/token.h"
#include "../../include/ast.h"
#include "../../include/error.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * parse_complex_body - Парсит сложное тело IF, включая вложенные IF, циклы, операторы.
 *
 * Возвращает список AST узлов или NULL при ошибке.
 */
ASTNodeList *parse_complex_body(TokenStream *ts) {
    if (!ts) {
        report_error("TokenStream is NULL in parse_complex_body");
        return NULL;
    }

    ASTNodeList *body_list = ast_node_list_create();
    if (!body_list) {
        report_error("Failed to create ASTNodeList for complex body");
        return NULL;
    }

    while (1) {
        Token *token = token_stream_peek(ts);
        if (!token) {
            report_error("Unexpected end of input inside complex body");
            ast_node_list_free(body_list);
            return NULL;
        }

        if (token->type == TOKEN_KEYWORD_ELSE ||
            token->type == TOKEN_KEYWORD_ELSEIF ||
            token->type == TOKEN_KEYWORD_ENDIF) {
            // Конец текущего тела
            break;
        }

        // Выбор парсера по ключевому слову
        ASTNode *stmt = NULL;
        switch (token->type) {
            case TOKEN_KEYWORD_IF:
                stmt = parse_if_multilevel(ts);
                break;
            case TOKEN_KEYWORD_LOOP:
                stmt = parse_loop(ts);
                break;
            case TOKEN_KEYWORD_PERFORM:
                stmt = parse_perform(ts);
                break;
            case TOKEN_KEYWORD_CALL_FUNCTION:
                stmt = parse_call_function(ts);
                break;
            default:
                stmt = parse_statement(ts);
                break;
        }

        if (!stmt) {
            report_error("Failed to parse statement inside complex body");
            ast_node_list_free(body_list);
            return NULL;
        }

        ast_node_list_append(body_list, stmt);
    }

    return body_list;
}
