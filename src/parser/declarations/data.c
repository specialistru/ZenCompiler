#include "../../include/parser.h"
#include "../../include/token.h"
#include "../../include/ast.h"
#include "../../include/error.h"
#include <stdlib.h>
#include <string.h>

/**
 * parse_declaration_data - Парсит объявление DATA для переменных.
 *
 * Ожидается, что ключевое слово DATA уже прочитано.
 * Возвращает AST узел DATA или NULL при ошибке.
 */
ASTNode *parse_declaration_data(TokenStream *ts) {
    if (!ts) {
        report_error("TokenStream is NULL in parse_declaration_data");
        return NULL;
    }

    ASTNode *data_node = ast_node_create(AST_DATA_DECL);
    if (!data_node) {
        report_error("Failed to allocate AST node for DATA declaration");
        return NULL;
    }

    // Ожидаем имя переменной
    Token *id_tok = token_stream_next(ts);
    if (!id_tok || id_tok->type != TOKEN_IDENTIFIER) {
        report_error("Expected identifier after DATA");
        ast_node_free(data_node);
        return NULL;
    }

    data_node->string_value = strdup(id_tok->text);
    if (!data_node->string_value) {
        report_error("Failed to allocate memory for variable name in DATA");
        ast_node_free(data_node);
        return NULL;
    }

    // Опциональные спецификаторы: TYPE, LIKE, VALUE
    Token *next_tok = token_stream_peek(ts);
    while (next_tok &&
          (next_tok->type == TOKEN_TYPE ||
           next_tok->type == TOKEN_LIKE ||
           next_tok->type == TOKEN_VALUE ||
           next_tok->type == TOKEN_DECIMALS ||
           next_tok->type == TOKEN_LENGTH)) {
        token_stream_next(ts); // consume keyword

        ASTNodeType spec_type;
        switch (next_tok->type) {
            case TOKEN_TYPE:     spec_type = AST_TYPE_SPEC;     break;
            case TOKEN_LIKE:     spec_type = AST_LIKE_SPEC;     break;
            case TOKEN_VALUE:    spec_type = AST_VALUE_SPEC;    break;
            case TOKEN_DECIMALS: spec_type = AST_DECIMALS_SPEC; break;
            case TOKEN_LENGTH:   spec_type = AST_LENGTH_SPEC;   break;
            default:             spec_type = AST_UNKNOWN;        break;
        }

        Token *val_tok = token_stream_next(ts);
        if (!val_tok || val_tok->type != TOKEN_IDENTIFIER) {
            report_error("Expected identifier after type/value specifier in DATA");
            ast_node_free(data_node);
            return NULL;
        }

        ASTNode *spec_node = ast_node_create(spec_type);
        if (!spec_node) {
            report_error("Failed to allocate AST node for DATA specifier");
            ast_node_free(data_node);
            return NULL;
        }

        spec_node->string_value = strdup(val_tok->text);
        if (!spec_node->string_value) {
            report_error("Failed to allocate memory for DATA specifier value");
            ast_node_free(data_node);
            ast_node_free(spec_node);
            return NULL;
        }

        ast_node_add_child(data_node, spec_node);
        next_tok = token_stream_peek(ts);
    }

    return data_node;
}
