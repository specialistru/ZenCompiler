#include "../../include/parser.h"
#include "../../include/token.h"
#include "../../include/ast.h"
#include "../../include/error.h"
#include <stdlib.h>
#include <string.h>

/**
 * parse_table_ops_modify - Парсит инструкцию MODIFY для изменения записи во внутренней таблице.
 *
 * Ожидается, что ключевое слово MODIFY уже прочитано.
 * Возвращает AST узел MODIFY или NULL при ошибке.
 */
ASTNode *parse_table_ops_modify(TokenStream *ts) {
    if (!ts) {
        report_error("TokenStream is NULL in parse_table_ops_modify");
        return NULL;
    }

    ASTNode *modify_node = ast_node_create(AST_MODIFY);
    if (!modify_node) {
        report_error("Failed to allocate AST node for MODIFY");
        return NULL;
    }

    // Ожидается имя таблицы
    Token *table_tok = token_stream_next(ts);
    if (!table_tok || table_tok->type != TOKEN_IDENTIFIER) {
        report_error("Expected table name after MODIFY");
        ast_node_free(modify_node);
        return NULL;
    }

    modify_node->string_value = strdup(table_tok->text);
    if (!modify_node->string_value) {
        report_error("Failed to allocate memory for MODIFY table name");
        ast_node_free(modify_node);
        return NULL;
    }

    // Опционально: FROM <источник>
    Token *next_tok = token_stream_peek(ts);
    if (next_tok && next_tok->type == TOKEN_FROM) {
        token_stream_next(ts); // consume FROM

        Token *source_tok = token_stream_next(ts);
        if (!source_tok || source_tok->type != TOKEN_IDENTIFIER) {
            report_error("Expected source identifier after FROM in MODIFY");
            ast_node_free(modify_node);
            return NULL;
        }

        ASTNode *from_node = ast_node_create(AST_FROM);
        if (!from_node) {
            report_error("Failed to allocate AST node for FROM in MODIFY");
            ast_node_free(modify_node);
            return NULL;
        }

        from_node->string_value = strdup(source_tok->text);
        if (!from_node->string_value) {
            report_error("Failed to allocate memory for FROM source");
            ast_node_free(modify_node);
            ast_node_free(from_node);
            return NULL;
        }

        ast_node_add_child(modify_node, from_node);
    }

    return modify_node;
}
