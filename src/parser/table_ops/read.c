#include "../../include/parser.h"
#include "../../include/token.h"
#include "../../include/ast.h"
#include "../../include/error.h"
#include <stdlib.h>
#include <string.h>

/**
 * parse_table_ops_read - Парсит инструкцию READ TABLE для чтения строки из внутренней таблицы.
 *
 * Ожидается, что ключевые слова READ TABLE уже были прочитаны.
 * Возвращает AST узел READ или NULL при ошибке.
 */
ASTNode *parse_table_ops_read(TokenStream *ts) {
    if (!ts) {
        report_error("TokenStream is NULL in parse_table_ops_read");
        return NULL;
    }

    ASTNode *read_node = ast_node_create(AST_READ);
    if (!read_node) {
        report_error("Failed to allocate AST node for READ");
        return NULL;
    }

    // Ожидается имя таблицы
    Token *table_tok = token_stream_next(ts);
    if (!table_tok || table_tok->type != TOKEN_IDENTIFIER) {
        report_error("Expected table name after READ TABLE");
        ast_node_free(read_node);
        return NULL;
    }

    read_node->string_value = strdup(table_tok->text);
    if (!read_node->string_value) {
        report_error("Failed to allocate memory for table name in READ");
        ast_node_free(read_node);
        return NULL;
    }

    // Опционально: WITH KEY <условие>
    Token *next_tok = token_stream_peek(ts);
    if (next_tok && next_tok->type == TOKEN_WITH) {
        token_stream_next(ts); // consume WITH

        Token *key_tok = token_stream_next(ts);
        if (!key_tok || key_tok->type != TOKEN_KEY) {
            report_error("Expected KEY after WITH in READ TABLE");
            ast_node_free(read_node);
            return NULL;
        }

        ASTNode *key_node = ast_node_create(AST_KEY);
        if (!key_node) {
            report_error("Failed to allocate AST node for READ KEY");
            ast_node_free(read_node);
            return NULL;
        }

        // Читаем выражение ключа до конца строки или до слова INTO
        Token *expr_tok;
        while ((expr_tok = token_stream_peek(ts)) &&
               expr_tok->type != TOKEN_INTO &&
               expr_tok->type != TOKEN_SEMICOLON &&
               expr_tok->type != TOKEN_END) {
            expr_tok = token_stream_next(ts);

            ASTNode *expr_node = ast_node_create(AST_EXPRESSION_TOKEN);
            if (!expr_node) {
                report_error("Failed to allocate AST node for expression token in READ KEY");
                ast_node_free(read_node);
                ast_node_free(key_node);
                return NULL;
            }

            expr_node->string_value = strdup(expr_tok->text);
            if (!expr_node->string_value) {
                report_error("Failed to allocate memory for expression token");
                ast_node_free(read_node);
                ast_node_free(key_node);
                ast_node_free(expr_node);
                return NULL;
            }

            ast_node_add_child(key_node, expr_node);
        }

        ast_node_add_child(read_node, key_node);
    }

    // Опционально: INTO <переменная>
    next_tok = token_stream_peek(ts);
    if (next_tok && next_tok->type == TOKEN_INTO) {
        token_stream_next(ts); // consume INTO

        Token *var_tok = token_stream_next(ts);
        if (!var_tok || var_tok->type != TOKEN_IDENTIFIER) {
            report_error("Expected identifier after INTO in READ TABLE");
            ast_node_free(read_node);
            return NULL;
        }

        ASTNode *into_node = ast_node_create(AST_INTO);
        if (!into_node) {
            report_error("Failed to allocate AST node for INTO in READ");
            ast_node_free(read_node);
            return NULL;
        }

        into_node->string_value = strdup(var_tok->text);
        if (!into_node->string_value) {
            report_error("Failed to allocate memory for INTO variable");
            ast_node_free(read_node);
            ast_node_free(into_node);
            return NULL;
        }

        ast_node_add_child(read_node, into_node);
    }

    return read_node;
}
