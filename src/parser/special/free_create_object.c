#include "../../include/parser.h"
#include "../../include/token.h"
#include "../../include/ast.h"
#include "../../include/error.h"

/**
 * parse_special_free_create_object - Парсит операторы FREE и CREATE OBJECT.
 *
 * FREE <variable> - освобождение памяти переменной.
 * CREATE OBJECT <object> [EXPORTING ...] - создание экземпляра объекта.
 *
 * Возвращает AST узел FREE или CREATE_OBJECT с соответствующими дочерними узлами.
 */
ASTNode *parse_special_free_create_object(TokenStream *ts) {
    if (!ts) {
        report_error("TokenStream is NULL in parse_special_free_create_object");
        return NULL;
    }

    Token *tok = token_stream_peek(ts);
    if (!tok) {
        report_error("Unexpected end of tokens in free_create_object");
        return NULL;
    }

    ASTNode *node = NULL;

    if (tok->type == TOKEN_FREE) {
        token_stream_next(ts); // consume FREE
        node = ast_node_create(AST_SPECIAL_FREE);
        if (!node) {
            report_error("Failed to create AST node for FREE");
            return NULL;
        }

        Token *var_tok = token_stream_next(ts);
        if (!var_tok || var_tok->type != TOKEN_IDENTIFIER) {
            report_error("Expected identifier after FREE");
            ast_node_free(node);
            return NULL;
        }

        ASTNode *var_node = ast_node_create(AST_IDENTIFIER);
        var_node->string_value = strdup(var_tok->text);
        ast_node_add_child(node, var_node);

    } else if (tok->type == TOKEN_CREATE) {
        token_stream_next(ts); // consume CREATE

        Token *object_tok = token_stream_next(ts);
        if (!object_tok || object_tok->type != TOKEN_OBJECT) {
            report_error("Expected OBJECT after CREATE");
            return NULL;
        }

        node = ast_node_create(AST_SPECIAL_CREATE_OBJECT);
        if (!node) {
            report_error("Failed to create AST node for CREATE OBJECT");
            return NULL;
        }

        Token *obj_name_tok = token_stream_next(ts);
        if (!obj_name_tok || obj_name_tok->type != TOKEN_IDENTIFIER) {
            report_error("Expected identifier after CREATE OBJECT");
            ast_node_free(node);
            return NULL;
        }

        ASTNode *obj_name_node = ast_node_create(AST_IDENTIFIER);
        obj_name_node->string_value = strdup(obj_name_tok->text);
        ast_node_add_child(node, obj_name_node);

        // Можно добавить парсинг опциональных параметров EXPORTING ...
        // Для упрощения пока пропустим.

    } else {
        report_error("Expected FREE or CREATE token");
        return NULL;
    }

    return node;
}
