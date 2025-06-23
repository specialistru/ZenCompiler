#include "../../include/parser.h"
#include "../../include/token.h"
#include "../../include/ast.h"
#include "../../include/error.h"
#include <stdlib.h>
#include <string.h>

/**
 * parse_select_where - Парсит условие WHERE для SELECT-запроса.
 *
 * Ожидается, что ключевое слово WHERE уже прочитано.
 * Возвращает AST узел условия WHERE или NULL при ошибке.
 */
ASTNode *parse_select_where(TokenStream *ts) {
    if (!ts) {
        report_error("TokenStream is NULL in parse_select_where");
        return NULL;
    }

    ASTNode *where_node = ast_node_create(AST_WHERE_CONDITION);
    if (!where_node) {
        report_error("Failed to allocate AST node for WHERE condition");
        return NULL;
    }

    // Парсим выражение условия (упрощённо: до конца SELECT или конца блока)
    // Можно расширить до полноценного рекурсивного парсера выражений

    Token *tok = NULL;
    while ((tok = token_stream_peek(ts)) != NULL &&
           tok->type != TOKEN_ENDSELECT && tok->type != TOKEN_SEMICOLON) {
        tok = token_stream_next(ts);

        // Создаём узлы для каждого токена или более сложное дерево условий
        // Для упрощения добавим каждый токен как дочерний узел с текстом

        ASTNode *expr_node = ast_node_create(AST_EXPRESSION_TOKEN);
        if (!expr_node) {
            report_error("Failed to allocate AST node for expression token");
            ast_node_free(where_node);
            return NULL;
        }
        expr_node->string_value = strdup(tok->text);
        if (!expr_node->string_value) {
            report_error("Failed to allocate memory for token text");
            ast_node_free(where_node);
            return NULL;
        }
        ast_node_add_child(where_node, expr_node);
    }

    if (!tok) {
        report_error("Unexpected end of tokens while parsing WHERE condition");
        ast_node_free(where_node);
        return NULL;
    }

    return where_node;
}
