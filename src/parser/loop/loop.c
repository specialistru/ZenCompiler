#include "../../include/parser.h"
#include "../../include/token.h"
#include "../../include/ast.h"
#include "../../include/error.h"
#include <stdlib.h>

/**
 * parse_loop - Парсит цикл LOOP ... ENDLOOP.
 *
 * Ожидается конструкция:
 * LOOP.
 *   ... тело цикла ...
 * ENDLOOP.
 *
 * Возвращает AST узел цикла или NULL при ошибке.
 */
ASTNode *parse_loop(TokenStream *ts) {
    if (!ts) {
        report_error("TokenStream is NULL in parse_loop");
        return NULL;
    }

    Token *token = token_stream_next(ts);
    if (!token || token->type != TOKEN_KEYWORD_LOOP) {
        report_error("Expected 'LOOP' keyword");
        return NULL;
    }

    Token *dot_token = token_stream_next(ts);
    if (!dot_token || dot_token->type != TOKEN_DOT) {
        report_error("Expected '.' after 'LOOP'");
        return NULL;
    }

    ASTNode *loop_node = ast_node_create(AST_LOOP);
    if (!loop_node) {
        report_error("Failed to allocate AST node for LOOP");
        return NULL;
    }

    // Создаем список операторов тела цикла
    ASTNodeList *body_statements = ast_node_list_create();
    if (!body_statements) {
        report_error("Failed to allocate AST node list for loop body");
        ast_node_free(loop_node);
        return NULL;
    }

    Token *next_token;
    while ((next_token = token_stream_peek(ts)) != NULL) {
        if (next_token->type == TOKEN_KEYWORD_ENDLOOP) {
            token_stream_next(ts); // consume ENDLOOP
            Token *end_dot = token_stream_next(ts);
            if (!end_dot || end_dot->type != TOKEN_DOT) {
                report_error("Expected '.' after 'ENDLOOP'");
                ast_node_list_free(body_statements);
                ast_node_free(loop_node);
                return NULL;
            }
            break;
        }

        // Для упрощения - пропускаем тело, можно добавить реальный парсинг
        token_stream_next(ts);
    }

    loop_node->body = body_statements;

    return loop_node;
}
