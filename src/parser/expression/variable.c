#include "../../include/parser.h"
#include "../../include/token.h"
#include "../../include/ast.h"
#include "../../include/error.h"
#include <stdlib.h>
#include <string.h>

/**
 * parse_variable - Парсит переменную в выражении.
 *
 * Поддерживаются простые идентификаторы, а также ссылки на поля или элементы,
 * например, <structure>-field или tab[index].
 *
 * Возвращает AST узел переменной или NULL при ошибке.
 */
ASTNode *parse_variable(TokenStream *ts) {
    if (!ts) {
        report_error("TokenStream is NULL in parse_variable");
        return NULL;
    }

    Token *token = token_stream_peek(ts);
    if (!token || token->type != TOKEN_IDENTIFIER) {
        report_error("Expected identifier at start of variable");
        return NULL;
    }

    // Парсим идентификатор переменной
    Token *ident_token = token_stream_next(ts);
    ASTNode *var_node = ast_node_create(AST_VARIABLE);
    if (!var_node) {
        report_error("Failed to allocate AST node for variable");
        return NULL;
    }

    var_node->string_value = strdup(ident_token->text);
    if (!var_node->string_value) {
        report_error("Failed to allocate memory for variable name");
        ast_node_free(var_node);
        return NULL;
    }

    // Здесь можно добавить обработку ссылок на поля, массивы и т.п.
    // Например, если следующий токен '-' и после него идентификатор — поле структуры
    token = token_stream_peek(ts);
    if (token && token->type == TOKEN_MINUS) {
        token_stream_next(ts); // consume '-'

        token = token_stream_next(ts);
        if (!token || token->type != TOKEN_IDENTIFIER) {
            report_error("Expected identifier after '-' in variable field access");
            ast_node_free(var_node);
            return NULL;
        }

        // Создаём дочерний узел для поля
        ASTNode *field_node = ast_node_create(AST_VARIABLE_FIELD);
        if (!field_node) {
            report_error("Failed to allocate AST node for variable field");
            ast_node_free(var_node);
            return NULL;
        }

        field_node->string_value = strdup(token->text);
        if (!field_node->string_value) {
            report_error("Failed to allocate memory for field name");
            ast_node_free(field_node);
            ast_node_free(var_node);
            return NULL;
        }

        // Добавляем поле как дочерний узел
        ast_node_add_child(var_node, field_node);
    }

    return var_node;
}
