#include "../../include/parser.h"
#include "../../include/token.h"
#include "../../include/ast.h"
#include "../../include/error.h"
#include <stdlib.h>

/**
 * parse_do_loop - Парсит цикл DO ... ENDDO.
 *
 * Ожидается конструкция:
 * DO.
 *   ... тело цикла ...
 * ENDDO.
 *
 * Возвращает AST узел цикла или NULL при ошибке.
 */
ASTNode *parse_do_loop(TokenStream *ts) {
    if (!ts) {
        report_error("TokenStream is NULL in parse_do_loop");
        return NULL;
    }

    Token *token = token_stream_next(ts);
    if (!token || token->type != TOKEN_KEYWORD_DO) {
        report_error("Expected 'DO' keyword");
        return NULL;
    }

    Token *dot_token = token_stream_next(ts);
    if (!dot_token || dot_token->type != TOKEN_DOT) {
        report_error("Expected '.' after 'DO'");
        return NULL;
    }

    ASTNode *do_node = ast_node_create(AST_DO_LOOP);
    if (!do_node) {
        report_error("Failed to allocate AST node for DO loop");
        return NULL;
    }

    ASTNodeList *body_statements = ast_node_list_create();
    if (!body_statements) {
        report_error("Failed to allocate AST node list for DO loop body");
        ast_node_free(do_node);
        return NULL;
    }

    Token *next_token;
    while ((next_token = token_stream_peek(ts)) != NULL) {
        if (next_token->type == TOKEN_KEYWORD_ENDDO) {
            token_stream_next(ts); // consume ENDDO
            Token *end_dot = token_stream_next(ts);
            if (!end_dot || end_dot->type != TOKEN_DOT) {
                report_error("Expected '.' after 'ENDDO'");
                ast_node_list_free(body_statements);
                ast_node_free(do_node);
                return NULL;
            }
            break;
        }

        // Пропускаем тело, можно добавить парсинг инструкций
        token_stream_next(ts);
    }

    do_node->body = body_statements;

    return do_node;
}
