#include "../../include/parser.h"
#include "../../include/token.h"
#include "../../include/ast.h"
#include "../../include/error.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * parse_perform_nested - Парсит вложенный оператор PERFORM,
 * который может содержать вызовы с параметрами, диапазоны форм и сложные конструкции.
 *
 * Синтаксис поддерживается примерно:
 * PERFORM <form_name> [USING <params>] [CHANGING <params>] [TABLES <tables>] [ON COMMIT <commit_option>].
 *
 * Возвращает AST узел или NULL при ошибке.
 */
ASTNode *parse_perform_nested(TokenStream *ts) {
    if (!ts) {
        report_error("TokenStream is NULL in parse_perform_nested");
        return NULL;
    }

    Token *token = token_stream_next(ts);
    if (!token || token->type != TOKEN_KEYWORD_PERFORM) {
        report_error("Expected PERFORM keyword");
        return NULL;
    }

    token = token_stream_next(ts);
    if (!token || token->type != TOKEN_IDENTIFIER) {
        report_error("Expected form name identifier after PERFORM");
        return NULL;
    }

    ASTNode *perform_node = ast_node_create(AST_PERFORM);
    if (!perform_node) {
        report_error("Failed to create AST node for PERFORM");
        return NULL;
    }
    perform_node->perform_form_name = strdup(token->text);
    if (!perform_node->perform_form_name) {
        report_error("Memory allocation failed for form name");
        ast_node_free(perform_node);
        return NULL;
    }

    // Инициализация списков параметров
    perform_node->perform_using_params = ast_node_list_create();
    perform_node->perform_changing_params = ast_node_list_create();
    perform_node->perform_tables_params = ast_node_list_create();
    if (!perform_node->perform_using_params || !perform_node->perform_changing_params || !perform_node->perform_tables_params) {
        report_error("Failed to create parameter lists");
        ast_node_free(perform_node);
        return NULL;
    }

    // Парсим опциональные части USING, CHANGING, TABLES, ON COMMIT
    while (1) {
        token = token_stream_peek(ts);
        if (!token) {
            report_error("Unexpected end of input in PERFORM nested");
            ast_node_free(perform_node);
            return NULL;
        }

        if (token->type == TOKEN_DOT) {
            token_stream_next(ts); // съесть точку и завершить
            break;
        } else if (token->type == TOKEN_KEYWORD_USING) {
            token_stream_next(ts); // съесть USING
            if (!parse_perform_param_list(ts, perform_node->perform_using_params)) {
                report_error("Failed to parse USING parameters");
                ast_node_free(perform_node);
                return NULL;
            }
        } else if (token->type == TOKEN_KEYWORD_CHANGING) {
            token_stream_next(ts); // съесть CHANGING
            if (!parse_perform_param_list(ts, perform_node->perform_changing_params)) {
                report_error("Failed to parse CHANGING parameters");
                ast_node_free(perform_node);
                return NULL;
            }
        } else if (token->type == TOKEN_KEYWORD_TABLES) {
            token_stream_next(ts); // съесть TABLES
            if (!parse_perform_param_list(ts, perform_node->perform_tables_params)) {
                report_error("Failed to parse TABLES parameters");
                ast_node_free(perform_node);
                return NULL;
            }
        } else if (token->type == TOKEN_KEYWORD_ON) {
            token_stream_next(ts); // съесть ON
            token = token_stream_next(ts);
            if (!token || token->type != TOKEN_KEYWORD_COMMIT) {
                report_error("Expected COMMIT after ON in PERFORM");
                ast_node_free(perform_node);
                return NULL;
            }
            // Обработка ON COMMIT (например, сохраняем флаг в узле)
            perform_node->perform_on_commit = 1;
        } else {
            report_error("Unexpected token in PERFORM nested parameters");
            ast_node_free(perform_node);
            return NULL;
        }
    }

    return perform_node;
}

/**
 * parse_perform_param_list - Парсит список параметров PERFORM (USING, CHANGING, TABLES).
 *
 * Возвращает 1 при успехе, 0 при ошибке.
 */
int parse_perform_param_list(TokenStream *ts, ASTNodeList *param_list) {
    if (!ts || !param_list) return 0;

    while (1) {
        Token *token = token_stream_next(ts);
        if (!token) {
            report_error("Unexpected end of input in perform param list");
            return 0;
        }

        if (token->type == TOKEN_IDENTIFIER) {
            ASTNode *param_node = ast_node_create(AST_VARIABLE);
            if (!param_node) {
                report_error("Failed to create AST variable node for param");
                return 0;
            }
            param_node->var_name = strdup(token->text);
            if (!param_node->var_name) {
                report_error("Memory allocation failed for param var_name");
                ast_node_free(param_node);
                return 0;
            }
            ast_node_list_append(param_list, param_node);

            // Проверка на запятую или окончание списка
            Token *next_token = token_stream_peek(ts);
            if (!next_token) {
                report_error("Unexpected end of input in perform param list");
                return 0;
            }
            if (next_token->type == TOKEN_COMMA) {
                token_stream_next(ts); // съесть запятую и продолжать
                continue;
            } else {
                break; // конец списка параметров
            }
        } else {
            report_error("Expected identifier in perform param list");
            return 0;
        }
    }
    return 1;
}
