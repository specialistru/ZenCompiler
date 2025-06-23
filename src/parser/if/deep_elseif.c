#include "../../include/parser.h"
#include "../../include/token.h"
#include "../../include/ast.h"
#include "../../include/error.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * parse_deep_elseif - Рекурсивный парсер для глубоких вложенных ELSEIF в IF конструкции.
 *
 * Возвращает AST узел или NULL при ошибке.
 */
ASTNode *parse_deep_elseif(TokenStream *ts) {
    if (!ts) {
        fprintf(stderr, "TokenStream is NULL\n");
        return NULL;
    }

    Token *token = token_stream_peek(ts);

    // Если нет ELSEIF — вернуть NULL (нет вложенного ELSEIF)
    if (!token || token->type != TOKEN_KEYWORD_ELSEIF) {
        return NULL;
    }

    // Съесть ELSEIF
    token_stream_next(ts);

    ASTNode *elseif_node = ast_node_create(AST_ELSEIF);
    if (!elseif_node) {
        report_error("Failed to create AST ELSEIF node");
        return NULL;
    }

    // Парсим условие ELSEIF
    ASTNode *cond = parse_expression(ts);
    if (!cond) {
        report_error("Failed to parse ELSEIF condition");
        ast_node_free(elseif_node);
        return NULL;
    }

    token = token_stream_next(ts);
    if (!token || token->type != TOKEN_DOT) {
        report_error("Expected '.' after ELSEIF condition");
        ast_node_free(elseif_node);
        return NULL;
    }

    elseif_node->if_condition = cond;
    elseif_node->if_body = ast_node_list_create();
    if (!elseif_node->if_body) {
        report_error("Failed to create ASTNodeList for ELSEIF body");
        ast_node_free(elseif_node);
        return NULL;
    }

    // Парсим тело ELSEIF до следующего ELSEIF, ELSE или ENDIF
    while (1) {
        token = token_stream_peek(ts);
        if (!token) {
            report_error("Unexpected end of input inside ELSEIF body");
            ast_node_free(elseif_node);
            return NULL;
        }

        if (token->type == TOKEN_KEYWORD_ELSEIF) {
            // Рекурсивно парсим следующий ELSEIF
            ASTNode *next_elseif = parse_deep_elseif(ts);
            if (!next_elseif) {
                report_error("Failed to parse nested ELSEIF");
                ast_node_free(elseif_node);
                return NULL;
            }
            ast_node_list_append(elseif_node->elseif_list, next_elseif);
            break; // обработка следующего ELSEIF завершена
        } else if (token->type == TOKEN_KEYWORD_ELSE || token->type == TOKEN_KEYWORD_ENDIF) {
            break; // конец ELSEIF тела
        } else {
            // Обычная инструкция
            ASTNode *stmt = parse_statement(ts);
            if (!stmt) {
                report_error("Failed to parse statement inside ELSEIF body");
                ast_node_free(elseif_node);
                return NULL;
            }
            ast_node_list_append(elseif_node->if_body, stmt);
        }
    }

    return elseif_node;
}
