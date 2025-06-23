#include "../../include/parser.h"
#include "../../include/token.h"
#include "../../include/ast.h"
#include "../../include/error.h"
#include <stdlib.h>
#include <string.h>

/**
 * parse_complex_form - Парсит сложную форму (FORM ... ENDFORM) с параметрами.
 *
 * Обрабатывает объявление процедуры FORM с именем и параметрами,
 * тело которой парсится как последовательность операторов.
 *
 * Возвращает AST узел формы или NULL при ошибке.
 */

ASTNode *parse_complex_form(TokenStream *ts) {
    if (!ts) {
        report_error("TokenStream is NULL in parse_complex_form");
        return NULL;
    }

    // Ожидаем ключевое слово FORM уже прочитано, теперь имя процедуры
    Token *name_token = token_stream_next(ts);
    if (!name_token || name_token->type != TOKEN_IDENTIFIER) {
        report_error("Expected identifier as FORM name");
        return NULL;
    }

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

    // Парсим параметры после имени до точки
    while (1) {
        Token *next_token = token_stream_peek(ts);
        if (!next_token) {
            report_error("Unexpected end of tokens while parsing FORM parameters");
            ast_node_free(form_node);
            return NULL;
        }

        if (next_token->type == TOKEN_DOT) {
            token_stream_next(ts); // consume '.'
            break;
        }

        // Парсим параметр (используем упрощенно: идентификатор TYPE идентификатор)
        Token *param_name = token_stream_next(ts);
        if (!param_name || param_name->type != TOKEN_IDENTIFIER) {
            report_error("Expected parameter name in FORM");
            ast_node_free(form_node);
            return NULL;
        }

        Token *using_token = token_stream_next(ts);
        if (!using_token || (strcasecmp(using_token->text, "TYPE") != 0 && strcasecmp(using_token->text, "LIKE") != 0)) {
            report_error("Expected TYPE or LIKE keyword in FORM parameter declaration");
            ast_node_free(form_node);
            return NULL;
        }

        Token *param_type = token_stream_next(ts);
        if (!param_type || param_type->type != TOKEN_IDENTIFIER) {
            report_error("Expected parameter type identifier in FORM");
            ast_node_free(form_node);
            return NULL;
        }

        // Создаем узел параметра
        ASTNode *param_node = ast_node_create(AST_FORM_PARAMETER);
        if (!param_node) {
            report_error("Failed to allocate AST node for FORM parameter");
            ast_node_free(form_node);
            return NULL;
        }

        param_node->string_value = strdup(param_name->text);
        param_node->type_value = strdup(param_type->text); // type_value — дополнительное поле для типа, добавьте в структуру ASTNode

        ast_node_add_child(form_node, param_node);
    }

    // Парсим тело формы до ENDFORM
    while (1) {
        Token *next_token = token_stream_peek(ts);
        if (!next_token) {
            report_error("Unexpected end of tokens inside FORM");
            ast_node_free(form_node);
            return NULL;
        }

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
