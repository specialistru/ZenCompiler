#include "../../include/parser.h"
#include "../../include/token.h"
#include "../../include/ast.h"
#include "../../include/error.h"
#include <stdlib.h>
#include <string.h>

/**
 * parse_simple_form - Парсит простую форму (FORM ... ENDFORM).
 *
 * Обрабатывает объявление процедуры FORM с именем,
 * тело которой парсится как последовательность операторов.
 *
 * Возвращает AST узел формы или NULL при ошибке.
 */

ASTNode *parse_simple_form(TokenStream *ts) {
    if (!ts) {
        report_error("TokenStream is NULL in parse_simple_form");
        return NULL;
    }

    // Ожидаем ключевое слово FORM уже прочитано, теперь имя процедуры
    Token *name_token = token_stream_next(ts);
    if (!name_token || name_token->type != TOKEN_IDENTIFIER) {
        report_error("Expected identifier as FORM name");
        return NULL;
    }

    // Создаем узел FORM
    ASTNode *form_node = ast_node_create(AST_FORM);
    if (!form_node) {
        report_error("Failed to allocate AST node for FORM");
        return NULL;
    }

    form_node->string_value = strdup(name_token->text);
    if (!form_node->string_value) {
        report_error("Failed to allocate memory for FORM name");
        ast_node_free(form_node);
        return NULL;
    }

    // Ожидаем точку (конец объявления FORM)
    Token *dot_token = token_stream_next(ts);
    if (!dot_token || dot_token->type != TOKEN_DOT) {
        report_error("Expected '.' after FORM name");
        ast_node_free(form_node);
        return NULL;
    }

    // Парсим тело формы - последовательность операторов до ENDFORM
    while (1) {
        Token *next_token = token_stream_peek(ts);
        if (!next_token) {
            report_error("Unexpected end of tokens inside FORM");
            ast_node_free(form_node);
            return NULL;
        }

        // Если ENDFORM — конец процедуры
        if (next_token->type == TOKEN_ENDFORM) {
            token_stream_next(ts); // consume ENDFORM
            Token *end_dot = token_stream_next(ts);
            if (!end_dot || end_dot->type != TOKEN_DOT) {
                report_error("Expected '.' after ENDFORM");
                ast_node_free(form_node);
                return NULL;
            }
            break;
        }

        // Парсим оператор (общий вызов)
        ASTNode *stmt = parse_statement(ts);
        if (!stmt) {
            report_error("Failed to parse statement inside FORM");
            ast_node_free(form_node);
            return NULL;
        }

        ast_node_add_child(form_node, stmt);
    }

    return form_node;
}
