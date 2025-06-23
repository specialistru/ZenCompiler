#include "../../include/parser.h"
#include "../../include/token.h"
#include "../../include/ast.h"
#include "../../include/error.h"
#include <stdlib.h>
#include <string.h>

/**
 * parse_class_definition - Парсит начало объявления класса.
 *
 * Синтаксис:
 * CLASS <name> DEFINITION [PUBLIC | PRIVATE | PROTECTED].
 * 
 * Возвращает AST узел класса или NULL при ошибке.
 */
ASTNode *parse_class_definition(TokenStream *ts) {
    if (!ts) {
        report_error("TokenStream is NULL in parse_class_definition");
        return NULL;
    }

    Token *token = token_stream_next(ts);
    if (!token || token->type != TOKEN_KEYWORD_CLASS) {
        report_error("Expected 'CLASS' keyword");
        return NULL;
    }

    token = token_stream_next(ts);
    if (!token || token->type != TOKEN_IDENTIFIER) {
        report_error("Expected class name after 'CLASS'");
        return NULL;
    }

    ASTNode *class_node = ast_node_create(AST_CLASS_DEF);
    if (!class_node) {
        report_error("Failed to allocate AST node for CLASS");
        return NULL;
    }

    class_node->class_name = strdup(token->text);

    token = token_stream_next(ts);
    if (!token || token->type != TOKEN_KEYWORD_DEFINITION) {
        report_error("Expected 'DEFINITION' after class name");
        ast_node_free(class_node);
        return NULL;
    }

    // Обработка опциональной области видимости
    token = token_stream_peek(ts);
    if (token) {
        if (token->type == TOKEN_KEYWORD_PUBLIC ||
            token->type == TOKEN_KEYWORD_PRIVATE ||
            token->type == TOKEN_KEYWORD_PROTECTED) {
            token = token_stream_next(ts);
            class_node->visibility = strdup(token->text);
        }
    }

    return class_node;
}
