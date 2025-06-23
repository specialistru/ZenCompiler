#include "../../include/parser.h"
#include "../../include/token.h"
#include "../../include/ast.h"
#include "../../include/error.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * parse_if_simple - Парсит простой оператор IF.
 * Формат:
 *    IF <expression>.
 *      <statements>
 *    ENDIF.
 *
 * Возвращает указатель на AST узел типа IF или NULL при ошибке.
 */
ASTNode *parse_if_simple(TokenStream *ts) {
    if (!ts) {
        fprintf(stderr, "TokenStream is NULL\n");
        return NULL;
    }

    Token *token = token_stream_peek(ts);
    if (!token || token->type != TOKEN_KEYWORD_IF) {
        report_error("Expected IF keyword");
        return NULL;
    }
    // Съесть 'IF'
    token_stream_next(ts);

    // Парсим условие (expression)
    ASTNode *condition = parse_expression(ts);
    if (!condition) {
        report_error("Failed to parse IF condition");
        return NULL;
    }

    // Ожидаем точку ('.') после условия
    token = token_stream_next(ts);
    if (!token || token->type != TOKEN_DOT) {
        report_error("Expected '.' after IF condition");
        ast_node_free(condition);
        return NULL;
    }

    // Создаем AST узел IF
    ASTNode *if_node = ast_node_create(AST_IF);
    if (!if_node) {
        report_error("Failed to create AST IF node");
        ast_node_free(condition);
        return NULL;
    }
    if_node->if_condition = condition;

    // Парсим тело IF — блок операторов, пока не встретим ENDIF
    ASTNodeList *body_list = ast_node_list_create();
    if (!body_list) {
        report_error("Failed to create ASTNodeList for IF body");
        ast_node_free(if_node);
        return NULL;
    }

    while (1) {
        token = token_stream_peek(ts);
        if (!token) {
            report_error("Unexpected end of input inside IF body");
            ast_node_list_free(body_list);
            ast_node_free(if_node);
            return NULL;
        }

        if (token->type == TOKEN_KEYWORD_ENDIF) {
            // Съесть ENDIF
            token_stream_next(ts);

            // Ожидаем точку после ENDIF
            token = token_stream_next(ts);
            if (!token || token->type != TOKEN_DOT) {
                report_error("Expected '.' after ENDIF");
                ast_node_list_free(body_list);
                ast_node_free(if_node);
                return NULL;
            }
            break; // Конец IF блока
        }

        // Парсим отдельный оператор в теле
        ASTNode *stmt = parse_statement(ts);
        if (!stmt) {
            report_error("Failed to parse statement inside IF body");
            ast_node_list_free(body_list);
            ast_node_free(if_node);
            return NULL;
        }
        ast_node_list_append(body_list, stmt);
    }

    if_node->if_body = body_list;
    return if_node;
}
