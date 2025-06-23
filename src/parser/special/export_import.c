#include "../../include/parser.h"
#include "../../include/token.h"
#include "../../include/ast.h"
#include "../../include/error.h"

/**
 * parse_special_export_import - Парсит операторы EXPORT и IMPORT.
 *
 * Операторы EXPORT и IMPORT позволяют сохранять и загружать данные
 * в память или файлы для передачи между программами.
 * Синтаксис:
 *   EXPORT <data> TO MEMORY ID <id>.
 *   IMPORT <data> FROM MEMORY ID <id>.
 *
 * Возвращает AST узел EXPORT или IMPORT с параметрами.
 */
ASTNode *parse_special_export_import(TokenStream *ts) {
    if (!ts) {
        report_error("TokenStream is NULL in parse_special_export_import");
        return NULL;
    }

    Token *tok = token_stream_peek(ts);
    if (!tok) {
        report_error("Unexpected end of tokens in export_import");
        return NULL;
    }

    ASTNode *node = NULL;

    if (tok->type == TOKEN_EXPORT) {
        token_stream_next(ts); // consume EXPORT
        node = ast_node_create(AST_SPECIAL_EXPORT);
        if (!node) {
            report_error("Failed to create AST node for EXPORT");
            return NULL;
        }

        // Пропустим парсинг сложных выражений для data — для простоты сделаем идентификатор
        Token *data_tok = token_stream_next(ts);
        if (!data_tok || data_tok->type != TOKEN_IDENTIFIER) {
            report_error("Expected identifier after EXPORT");
            ast_node_free(node);
            return NULL;
        }

        ASTNode *data_node = ast_node_create(AST_IDENTIFIER);
        data_node->string_value = strdup(data_tok->text);
        ast_node_add_child(node, data_node);

        // Ожидаем TO MEMORY ID <id>
        Token *to_tok = token_stream_next(ts);
        if (!to_tok || to_tok->type != TOKEN_TO) {
            report_error("Expected TO after EXPORT <data>");
            ast_node_free(node);
            return NULL;
        }

        Token *memory_tok = token_stream_next(ts);
        if (!memory_tok || memory_tok->type != TOKEN_MEMORY) {
            report_error("Expected MEMORY after TO");
            ast_node_free(node);
            return NULL;
        }

        Token *id_tok = token_stream_next(ts);
        if (!id_tok || id_tok->type != TOKEN_ID) {
            report_error("Expected ID after MEMORY");
            ast_node_free(node);
            return NULL;
        }

        Token *memid_tok = token_stream_next(ts);
        if (!memid_tok || memid_tok->type != TOKEN_IDENTIFIER) {
            report_error("Expected identifier after ID");
            ast_node_free(node);
            return NULL;
        }

        ASTNode *memid_node = ast_node_create(AST_IDENTIFIER);
        memid_node->string_value = strdup(memid_tok->text);
        ast_node_add_child(node, memid_node);

    } else if (tok->type == TOKEN_IMPORT) {
        token_stream_next(ts); // consume IMPORT
        node = ast_node_create(AST_SPECIAL_IMPORT);
        if (!node) {
            report_error("Failed to create AST node for IMPORT");
            return NULL;
        }

        // Аналогично, парсим идентификатор данных
        Token *data_tok = token_stream_next(ts);
        if (!data_tok || data_tok->type != TOKEN_IDENTIFIER) {
            report_error("Expected identifier after IMPORT");
            ast_node_free(node);
            return NULL;
        }

        ASTNode *data_node = ast_node_create(AST_IDENTIFIER);
        data_node->string_value = strdup(data_tok->text);
        ast_node_add_child(node, data_node);

        // Ожидаем FROM MEMORY ID <id>
        Token *from_tok = token_stream_next(ts);
        if (!from_tok || from_tok->type != TOKEN_FROM) {
            report_error("Expected FROM after IMPORT <data>");
            ast_node_free(node);
            return NULL;
        }

        Token *memory_tok = token_stream_next(ts);
        if (!memory_tok || memory_tok->type != TOKEN_MEMORY) {
            report_error("Expected MEMORY after FROM");
            ast_node_free(node);
            return NULL;
        }

        Token *id_tok = token_stream_next(ts);
        if (!id_tok || id_tok->type != TOKEN_ID) {
            report_error("Expected ID after MEMORY");
            ast_node_free(node);
            return NULL;
        }

        Token *memid_tok = token_stream_next(ts);
        if (!memid_tok || memid_tok->type != TOKEN_IDENTIFIER) {
            report_error("Expected identifier after ID");
            ast_node_free(node);
            return NULL;
        }

        ASTNode *memid_node = ast_node_create(AST_IDENTIFIER);
        memid_node->string_value = strdup(memid_tok->text);
        ast_node_add_child(node, memid_node);

    } else {
        report_error("Expected EXPORT or IMPORT token");
        return NULL;
    }

    return node;
}


///////////////////////


#include "../../include/parser.h"
#include "../../include/token.h"
#include "../../include/ast.h"
#include "../../include/error.h"

/**
 * parse_special_export_import - Парсит операторы EXPORT и IMPORT.
 *
 * EXPORT <data> TO MEMORY <memid>.
 * IMPORT <data> FROM MEMORY <memid>.
 *
 * Поддерживает передачу данных по имени памяти.
 *
 * Возвращает AST узел EXPORT или IMPORT с соответствующими параметрами.
 */
ASTNode *parse_special_export_import(TokenStream *ts) {
    if (!ts) {
        report_error("TokenStream is NULL in parse_special_export_import");
        return NULL;
    }

    Token *cmd_tok = token_stream_next(ts);
    if (!cmd_tok || (cmd_tok->type != TOKEN_EXPORT && cmd_tok->type != TOKEN_IMPORT)) {
        report_error("Expected EXPORT or IMPORT token");
        return NULL;
    }

    ASTNode *node = ast_node_create((cmd_tok->type == TOKEN_EXPORT) ? AST_SPECIAL_EXPORT : AST_SPECIAL_IMPORT);
    if (!node) {
        report_error("Failed to create AST node for EXPORT/IMPORT");
        return NULL;
    }

    // Парсим данные после EXPORT/IMPORT — идентификатор или список идентификаторов
    Token *data_tok = token_stream_next(ts);
    if (!data_tok || data_tok->type != TOKEN_IDENTIFIER) {
        report_error("Expected identifier after EXPORT/IMPORT");
        ast_node_free(node);
        return NULL;
    }

    ASTNode *data_node = ast_node_create(AST_IDENTIFIER);
    data_node->string_value = strdup(data_tok->text);
    ast_node_add_child(node, data_node);

    // Парсим ключевое слово TO или FROM
    Token *dir_tok = token_stream_next(ts);
    if (!dir_tok) {
        report_error("Expected TO or FROM after EXPORT/IMPORT data");
        ast_node_free(node);
        return NULL;
    }

    if ((cmd_tok->type == TOKEN_EXPORT && dir_tok->type != TOKEN_TO) ||
        (cmd_tok->type == TOKEN_IMPORT && dir_tok->type != TOKEN_FROM)) {
        report_error("Expected TO after EXPORT or FROM after IMPORT");
        ast_node_free(node);
        return NULL;
    }

    // Парсим MEMORY
    Token *mem_tok = token_stream_next(ts);
    if (!mem_tok || mem_tok->type != TOKEN_MEMORY) {
        report_error("Expected MEMORY keyword");
        ast_node_free(node);
        return NULL;
    }

    // Парсим идентификатор памяти
    Token *memid_tok = token_stream_next(ts);
    if (!memid_tok || memid_tok->type != TOKEN_IDENTIFIER) {
        report_error("Expected memory ID after MEMORY");
        ast_node_free(node);
        return NULL;
    }

    ASTNode *memid_node = ast_node_create(AST_IDENTIFIER);
    memid_node->string_value = strdup(memid_tok->text);
    ast_node_add_child(node, memid_node);

    return node;
}
