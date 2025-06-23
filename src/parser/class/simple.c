#include "../../include/parser.h"
#include "../../include/token.h"
#include "../../include/ast.h"
#include "../../include/error.h"
#include <stdlib.h>
#include <string.h>

/**
 * parse_simple_class - Парсит простое объявление класса ABAP.
 *
 * Синтаксис:
 * CLASS <class_name> DEFINITION.
 *   <опционально секции: PUBLIC, PROTECTED, PRIVATE>
 * ENDCLASS.
 *
 * Возвращает AST узел класса.
 */
ASTNode *parse_simple_class(TokenStream *ts) {
    if (!ts) {
        report_error("TokenStream is NULL in parse_simple_class");
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
        report_error("Failed to allocate AST node for class");
        return NULL;
    }

    class_node->class_name = strdup(token->text);
    class_node->sections = ast_node_list_create();
    if (!class_node->sections) {
        report_error("Failed to create sections list for class");
        ast_node_free(class_node);
        return NULL;
    }

    // Ожидаем ключевое слово DEFINITION
    token = token_stream_next(ts);
    if (!token || token->type != TOKEN_KEYWORD_DEFINITION) {
        report_error("Expected 'DEFINITION' keyword after class name");
        ast_node_free(class_node);
        return NULL;
    }

    // Точка после DEFINITION
    token = token_stream_next(ts);
    if (!token || token->type != TOKEN_DOT) {
        report_error("Expected '.' after 'DEFINITION'");
        ast_node_free(class_node);
        return NULL;
    }

    // Парсим секции (PUBLIC SECTION, PRIVATE SECTION и т.п.)
    while ((token = token_stream_peek(ts)) != NULL) {
        if (token->type == TOKEN_KEYWORD_ENDCLASS) {
            token_stream_next(ts);  // consume ENDCLASS
            token = token_stream_next(ts);
            if (!token || token->type != TOKEN_DOT) {
                report_error("Expected '.' after ENDCLASS");
                ast_node_free(class_node);
                return NULL;
            }
            break; // класс завершен
        }

        // Здесь можно парсить секции и их содержимое, например:
        // PUBLIC SECTION. PRIVATE SECTION. PROTECTED SECTION.
        // Для простоты пока пропускаем детали.

        // Если не секция, прерываем цикл
        break;
    }

    return class_node;
}
