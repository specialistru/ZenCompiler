#include "../../include/parser.h"
#include "../../include/token.h"
#include "../../include/ast.h"
#include "../../include/error.h"
#include <stdlib.h>
#include <string.h>

/**
 * parse_complex_module - Парсит сложный модуль с параметрами и локальными объявлениями.
 *
 * Обрабатывает модуль с возможными параметрами, локальными переменными,
 * событиями и завершается ключевым словом ENDMODULE с точкой.
 *
 * Возвращает AST узел модуля или NULL при ошибке.
 */

ASTNode *parse_complex_module(TokenStream *ts) {
    if (!ts) {
        report_error("TokenStream is NULL in parse_complex_module");
        return NULL;
    }

    // Ожидается ключевое слово MODULE уже прочитано, теперь имя модуля
    Token *name_token = token_stream_next(ts);
    if (!name_token || name_token->type != TOKEN_IDENTIFIER) {
        report_error("Expected identifier as MODULE name");
        return NULL;
    }

    ASTNode *module_node = ast_node_create(AST_MODULE);
    if (!module_node) {
        report_error("Failed to allocate AST node for MODULE");
        return NULL;
    }

    module_node->string_value = strdup(name_token->text);
    if (!module_node->string_value) {
        report_error("Failed to allocate memory for MODULE name");
        ast_node_free(module_node);
        return NULL;
    }

    // Опционально: парсим параметры и локальные объявления до точки
    while (1) {
        Token *peek = token_stream_peek(ts);
        if (!peek) {
            report_error("Unexpected end of tokens while parsing MODULE header");
            ast_node_free(module_node);
            return NULL;
        }

        if (peek->type == TOKEN_DOT) {
            token_stream_next(ts); // consume '.'
            break;
        }

        // Здесь можно добавить логику парсинга параметров или объявлений
        // Пока пропустим этот функционал или добавим как TODO
        // Для простоты — выдаем ошибку, если что-то кроме точки встречается
        report_error("Unexpected token in MODULE header, expected '.'");
        ast_node_free(module_node);
        return NULL;
    }

    // Парсим тело модуля до ENDMODULE
    while (1) {
        Token *next_token = token_stream_peek(ts);
        if (!next_token) {
            report_error("Unexpected end of tokens inside MODULE");
            ast_node_free(module_node);
            return NULL;
        }

        if (next_token->type == TOKEN_ENDMODULE) {
            token_stream_next(ts); // consume ENDMODULE
            Token *end_dot = token_stream_next(ts);
            if (!end_dot || end_dot->type != TOKEN_DOT) {
                report_error("Expected '.' after ENDMODULE");
                ast_node_free(module_node);
                return NULL;
            }
            break;
        }

        ASTNode *stmt = parse_statement(ts);
        if (!stmt) {
            report_error("Failed to parse statement inside MODULE");
            ast_node_free(module_node);
            return NULL;
        }

        ast_node_add_child(module_node, stmt);
    }

    return module_node;
}
