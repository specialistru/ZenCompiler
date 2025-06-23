#include "../../include/parser.h"
#include "../../include/token.h"
#include "../../include/ast.h"
#include "../../include/error.h"
#include <stdlib.h>
#include <string.h>

/**
 * parse_table_ops_append - Парсит конструкцию APPEND для добавления записи во внутреннюю таблицу.
 *
 * Ожидается, что ключевое слово APPEND уже прочитано.
 * Возвращает AST узел APPEND или NULL при ошибке.
 */
ASTNode *parse_table_ops_append(TokenStream *ts) {
    if (!ts) {
        report_error("TokenStream is NULL in parse_table_ops_append");
        return NULL;
    }

    ASTNode *append_node = ast_node_create(AST_APPEND);
    if (!append_node) {
        report_error("Failed to allocate AST node for APPEND");
        return NULL;
    }

    // Ожидаем запись или переменную, которую нужно добавить в таблицу
    Token *record_tok = token_stream_next(ts);
    if (!record_tok) {
        report_error("Expected record identifier after APPEND");
        ast_node_free(append_node);
        return NULL;
    }

    if (record_tok->type != TOKEN_IDENTIFIER) {
        report_error("Expected identifier after APPEND");
        ast_node_free(append_node);
        return NULL;
    }

    append_node->string_value = strdup(record_tok->text);
    if (!append_node->string_value) {
        report_error("Failed to allocate memory for APPEND record identifier");
        ast_node_free(append_node);
        return NULL;
    }

    // Опционально: ожидаем ключевые слова TO или IN, указывающие таблицу
    Token *next_tok = token_stream_peek(ts);
    if (next_tok && (next_tok->type == TOKEN_TO || next_tok->type == TOKEN_IN)) {
        token_stream_next(ts);  // consume TO/IN

        Token *table_tok = token_stream_next(ts);
        if (!table_tok || table_tok->type != TOKEN_IDENTIFIER) {
            report_error("Expected table identifier after TO/IN in APPEND");
            ast_node_free(append_node);
            return NULL;
        }

        ASTNode *table_node = ast_node_create(AST_TABLE);
        if (!table_node) {
            report_error("Failed to allocate AST node for APPEND target table");
            ast_node_free(append_node);
            return NULL;
        }

        table_node->string_value = strdup(table_tok->text);
        if (!table_node->string_value) {
            report_error("Failed to allocate memory for APPEND target table name");
            ast_node_free(append_node);
            ast_node_free(table_node);
            return NULL;
        }

        ast_node_add_child(append_node, table_node);
    }

    return append_node;
}
