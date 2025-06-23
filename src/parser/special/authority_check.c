#include "../../include/parser.h"
#include "../../include/token.h"
#include "../../include/ast.h"
#include "../../include/error.h"

/**
 * parse_special_authority_check - Парсит оператор AUTHORITY-CHECK.
 *
 * AUTHORITY-CHECK используется для проверки прав пользователя на объекты системы.
 * Синтаксис может включать несколько параметров, например, OBJECT, ID, FIELD и т.д.
 *
 * Возвращает AST узел AUTHORITY_CHECK с параметрами проверки.
 */
ASTNode *parse_special_authority_check(TokenStream *ts) {
    if (!ts) {
        report_error("TokenStream is NULL in parse_special_authority_check");
        return NULL;
    }

    Token *auth_tok = token_stream_next(ts);
    if (!auth_tok || auth_tok->type != TOKEN_AUTHORITY_CHECK) {
        report_error("Expected AUTHORITY-CHECK token");
        return NULL;
    }

    ASTNode *node = ast_node_create(AST_SPECIAL_AUTHORITY_CHECK);
    if (!node) {
        report_error("Failed to create AST node for AUTHORITY-CHECK");
        return NULL;
    }

    // Парсим параметры AUTHORITY-CHECK, например: OBJECT <obj>, ID <id>, FIELD <field>
    while (1) {
        Token *param_tok = token_stream_peek(ts);
        if (!param_tok) break;

        if (param_tok->type == TOKEN_OBJECT || param_tok->type == TOKEN_ID || param_tok->type == TOKEN_FIELD) {
            token_stream_next(ts); // consume param

            Token *value_tok = token_stream_next(ts);
            if (!value_tok || (value_tok->type != TOKEN_IDENTIFIER && value_tok->type != TOKEN_LITERAL)) {
                report_error("Expected identifier or literal after AUTHORITY-CHECK parameter");
                ast_node_free(node);
                return NULL;
            }

            ASTNode *param_node = ast_node_create(AST_SPECIAL_AUTH_PARAM);
            param_node->string_value = strdup(param_tok->text);

            ASTNode *value_node = ast_node_create(AST_IDENTIFIER);
            value_node->string_value = strdup(value_tok->text);

            ast_node_add_child(param_node, value_node);
            ast_node_add_child(node, param_node);
        } else {
            // Не параметр — заканчиваем парсинг
            break;
        }
    }

    return node;
}
