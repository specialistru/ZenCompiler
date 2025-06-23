#include "../../include/parser.h"
#include "../../include/token.h"
#include "../../include/ast.h"
#include "../../include/error.h"
#include <stdlib.h>
#include <string.h>

/**
 * parse_declaration_constants - Парсит объявление CONSTANTS в ABAP.
 *
 * Ожидается, что ключевое слово CONSTANTS уже прочитано.
 * Возвращает AST узел CONSTANTS или NULL при ошибке.
 */
ASTNode *parse_declaration_constants(TokenStream *ts) {
    if (!ts) {
        report_error("TokenStream is NULL in parse_declaration_constants");
        return NULL;
    }

    ASTNode *const_node = ast_node_create(AST_CONSTANT_DECL);
    if (!const_node) {
        report_error("Failed to allocate AST node for CONSTANTS declaration");
        return NULL;
    }

    // Ожидается идентификатор (имя константы)
    Token *id_tok = token_stream_next(ts);
    if (!id_tok || id_tok->type != TOKEN_IDENTIFIER) {
        report_error("Expected identifier after CONSTANTS");
        ast_node_free(const_node);
        return NULL;
    }

    const_node->string_value = strdup(id_tok->text);
    if (!const_node->string_value) {
        report_error("Failed to allocate memory for constant name");
        ast_node_free(const_node);
        return NULL;
    }

    // Ожидается ключевое слово TYPE
    Token *type_tok = token_stream_next(ts);
    if (!type_tok || type_tok->type != TOKEN_TYPE) {
        report_error("Expected TYPE after constant name");
        ast_node_free(const_node);
        return NULL;
    }

    // Тип значения
    Token *type_val = token_stream_next(ts);
    if (!type_val || type_val->type != TOKEN_IDENTIFIER) {
        report_error("Expected type name after TYPE in CONSTANTS");
        ast_node_free(const_node);
        return NULL;
    }

    ASTNode *type_node = ast_node_create(AST_TYPE_SPEC);
    if (!type_node) {
        report_error("Failed to allocate AST node for CONSTANTS type");
        ast_node_free(const_node);
        return NULL;
    }

    type_node->string_value = strdup(type_val->text);
    if (!type_node->string_value) {
        report_error("Failed to allocate memory for type in CONSTANTS");
        ast_node_free(const_node);
        ast_node_free(type_node);
        return NULL;
    }

    ast_node_add_child(const_node, type_node);

    // Ожидается ключевое слово VALUE
    Token *value_kw = token_stream_next(ts);
    if (!value_kw || value_kw->type != TOKEN_VALUE) {
        report_error("Expected VALUE after TYPE in CONSTANTS");
        ast_node_free(const_node);
        return NULL;
    }

    Token *value_tok = token_stream_next(ts);
    if (!value_tok || (value_tok->type != TOKEN_STRING && value_tok->type != TOKEN_NUMBER)) {
        report_error("Expected literal value after VALUE in CONSTANTS");
        ast_node_free(const_node);
        return NULL;
    }

    ASTNode *value_node = ast_node_create(AST_VALUE_SPEC);
    if (!value_node) {
        report_error("Failed to allocate AST node for constant value");
        ast_node_free(const_node);
        return NULL;
    }

    value_node->string_value = strdup(value_tok->text);
    if (!value_node->string_value) {
        report_error("Failed to allocate memory for constant value");
        ast_node_free(const_node);
        ast_node_free(value_node);
        return NULL;
    }

    ast_node_add_child(const_node, value_node);

    return const_node;
}
