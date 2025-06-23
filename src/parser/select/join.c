#include "../../include/parser.h"
#include "../../include/token.h"
#include "../../include/ast.h"
#include "../../include/error.h"
#include <stdlib.h>
#include <string.h>

/**
 * parse_select_join - Парсит SELECT с JOIN конструкцией.
 *
 * Ожидается, что ключевое слово SELECT уже прочитано.
 * Возвращает AST узел SELECT с JOIN или NULL при ошибке.
 */
ASTNode *parse_select_join(TokenStream *ts) {
    if (!ts) {
        report_error("TokenStream is NULL in parse_select_join");
        return NULL;
    }

    ASTNode *select_node = ast_node_create(AST_SELECT);
    if (!select_node) {
        report_error("Failed to allocate AST node for SELECT");
        return NULL;
    }

    // Парсим список полей до ключевого слова FROM
    ASTNode *fields_node = ast_node_create(AST_FIELD_LIST);
    if (!fields_node) {
        report_error("Failed to allocate AST node for field list");
        ast_node_free(select_node);
        return NULL;
    }

    Token *tok = NULL;
    while ((tok = token_stream_peek(ts)) != NULL && tok->type != TOKEN_FROM) {
        tok = token_stream_next(ts);
        if (tok->type == TOKEN_COMMA) {
            continue;
        }
        if (tok->type != TOKEN_IDENTIFIER) {
            report_error("Expected field identifier in SELECT statement");
            ast_node_free(select_node);
            ast_node_free(fields_node);
            return NULL;
        }

        ASTNode *field_node = ast_node_create(AST_FIELD);
        if (!field_node) {
            report_error("Failed to allocate AST node for field");
            ast_node_free(select_node);
            ast_node_free(fields_node);
            return NULL;
        }
        field_node->string_value = strdup(tok->text);
        if (!field_node->string_value) {
            report_error("Failed to allocate memory for field name");
            ast_node_free(select_node);
            ast_node_free(fields_node);
            return NULL;
        }
        ast_node_add_child(fields_node, field_node);
    }

    if (!tok || tok->type != TOKEN_FROM) {
        report_error("Expected FROM keyword in SELECT statement");
        ast_node_free(select_node);
        ast_node_free(fields_node);
        return NULL;
    }
    token_stream_next(ts); // consume FROM

    // Парсим первую таблицу (левая таблица в JOIN)
    Token *table_tok = token_stream_next(ts);
    if (!table_tok || table_tok->type != TOKEN_IDENTIFIER) {
        report_error("Expected table name after FROM");
        ast_node_free(select_node);
        ast_node_free(fields_node);
        return NULL;
    }
    ASTNode *left_table_node = ast_node_create(AST_TABLE);
    if (!left_table_node) {
        report_error("Failed to allocate AST node for left table");
        ast_node_free(select_node);
        ast_node_free(fields_node);
        return NULL;
    }
    left_table_node->string_value = strdup(table_tok->text);
    if (!left_table_node->string_value) {
        report_error("Failed to allocate memory for left table name");
        ast_node_free(select_node);
        ast_node_free(fields_node);
        ast_node_free(left_table_node);
        return NULL;
    }

    // Проверяем наличие JOIN
    tok = token_stream_peek(ts);
    if (!tok || (tok->type != TOKEN_JOIN && tok->type != TOKEN_INNER_JOIN &&
                 tok->type != TOKEN_LEFT_JOIN && tok->type != TOKEN_RIGHT_JOIN)) {
        report_error("Expected JOIN keyword after first table");
        ast_node_free(select_node);
        ast_node_free(fields_node);
        ast_node_free(left_table_node);
        return NULL;
    }
    Token *join_tok = token_stream_next(ts);

    // Парсим вторую таблицу
    Token *right_table_tok = token_stream_next(ts);
    if (!right_table_tok || right_table_tok->type != TOKEN_IDENTIFIER) {
        report_error("Expected table name after JOIN");
        ast_node_free(select_node);
        ast_node_free(fields_node);
        ast_node_free(left_table_node);
        return NULL;
    }
    ASTNode *right_table_node = ast_node_create(AST_TABLE);
    if (!right_table_node) {
        report_error("Failed to allocate AST node for right table");
        ast_node_free(select_node);
        ast_node_free(fields_node);
        ast_node_free(left_table_node);
        return NULL;
    }
    right_table_node->string_value = strdup(right_table_tok->text);
    if (!right_table_node->string_value) {
        report_error("Failed to allocate memory for right table name");
        ast_node_free(select_node);
        ast_node_free(fields_node);
        ast_node_free(left_table_node);
        ast_node_free(right_table_node);
        return NULL;
    }

    // Ожидается ON для условия соединения
    tok = token_stream_next(ts);
    if (!tok || tok->type != TOKEN_ON) {
        report_error("Expected ON keyword for JOIN condition");
        ast_node_free(select_node);
        ast_node_free(fields_node);
        ast_node_free(left_table_node);
        ast_node_free(right_table_node);
        return NULL;
    }

    // Парсим условие ON (упрощённо — пропускаем и парсим до WHERE или конца)
    ASTNode *on_condition = ast_node_create(AST_JOIN_CONDITION);
    if (!on_condition) {
        report_error("Failed to allocate AST node for JOIN condition");
        ast_node_free(select_node);
        ast_node_free(fields_node);
        ast_node_free(left_table_node);
        ast_node_free(right_table_node);
        return NULL;
    }

    // Для упрощения возьмём все токены до WHERE или конца
    while ((tok = token_stream_peek(ts)) != NULL && tok->type != TOKEN_WHERE) {
        token_stream_next(ts);
        // Можно расширить и построить полноценное дерево условий
    }

    // Составляем дерево
    ast_node_add_child(select_node, fields_node);
    ast_node_add_child(select_node, left_table_node);

    ASTNode *join_node = ast_node_create(AST_JOIN);
    if (!join_node) {
        report_error("Failed to allocate AST node for JOIN");
        ast_node_free(select_node);
        ast_node_free(fields_node);
        ast_node_free(left_table_node);
        ast_node_free(right_table_node);
        ast_node_free(on_condition);
        return NULL;
    }

    join_node->string_value = strdup(join_tok->text); // тип JOIN (INNER, LEFT, etc.)
    ast_node_add_child(join_node, right_table_node);
    ast_node_add_child(join_node, on_condition);

    ast_node_add_child(select_node, join_node);


///////////////////////////

    #include "../../include/parser.h"
#include "../../include/token.h"
#include "../../include/ast.h"
#include "../../include/error.h"
#include <stdlib.h>
#include <string.h>

/**
 * parse_select_join - Парсит JOIN-часть SELECT-запроса.
 *
 * Ожидается, что ключевое слово JOIN уже прочитано.
 * Возвращает AST узел JOIN или NULL при ошибке.
 */
ASTNode *parse_select_join(TokenStream *ts) {
    if (!ts) {
        report_error("TokenStream is NULL in parse_select_join");
        return NULL;
    }

    ASTNode *join_node = ast_node_create(AST_JOIN);
    if (!join_node) {
        report_error("Failed to allocate AST node for JOIN");
        return NULL;
    }

    // Ожидаем тип JOIN (INNER, LEFT OUTER, RIGHT OUTER и т.д.) — упрощаем как идентификатор
    Token *join_type_tok = token_stream_next(ts);
    if (!join_type_tok || join_type_tok->type != TOKEN_IDENTIFIER) {
        report_error("Expected JOIN type identifier");
        ast_node_free(join_node);
        return NULL;
    }
    join_node->string_value = strdup(join_type_tok->text);
    if (!join_node->string_value) {
        report_error("Failed to allocate memory for JOIN type");
        ast_node_free(join_node);
        return NULL;
    }

    // Ожидаем имя таблицы после типа JOIN
    Token *table_tok = token_stream_next(ts);
    if (!table_tok || table_tok->type != TOKEN_IDENTIFIER) {
        report_error("Expected table name after JOIN type");
        ast_node_free(join_node);
        return NULL;
    }

    ASTNode *table_node = ast_node_create(AST_TABLE);
    if (!table_node) {
        report_error("Failed to allocate AST node for JOIN table");
        ast_node_free(join_node);
        return NULL;
    }
    table_node->string_value = strdup(table_tok->text);
    if (!table_node->string_value) {
        report_error("Failed to allocate memory for JOIN table name");
        ast_node_free(join_node);
        ast_node_free(table_node);
        return NULL;
    }
    ast_node_add_child(join_node, table_node);

    // Ожидаем ON ключевое слово
    Token *on_tok = token_stream_next(ts);
    if (!on_tok || on_tok->type != TOKEN_ON) {
        report_error("Expected ON keyword in JOIN clause");
        ast_node_free(join_node);
        return NULL;
    }

    // Парсим условие соединения — упрощённо как последовательность токенов до следующего ключевого слова
    ASTNode *on_condition = ast_node_create(AST_ON_CONDITION);
    if (!on_condition) {
        report_error("Failed to allocate AST node for ON condition");
        ast_node_free(join_node);
        return NULL;
    }

    Token *tok = NULL;
    while ((tok = token_stream_peek(ts)) != NULL &&
           tok->type != TOKEN_WHERE &&
           tok->type != TOKEN_FROM &&
           tok->type != TOKEN_SEMICOLON &&
           tok->type != TOKEN_JOIN &&
           tok->type != TOKEN_ENDSELECT) {
        tok = token_stream_next(ts);
        ASTNode *expr_node = ast_node_create(AST_EXPRESSION_TOKEN);
        if (!expr_node) {
            report_error("Failed to allocate AST node for expression token");
            ast_node_free(join_node);
            ast_node_free(on_condition);
            return NULL;
        }
        expr_node->string_value = strdup(tok->text);
        if (!expr_node->string_value) {
            report_error("Failed to allocate memory for token text");
            ast_node_free(join_node);
            ast_node_free(on_condition);
            ast_node_free(expr_node);
            return NULL;
        }
        ast_node_add_child(on_condition, expr_node);
    }

    ast_node_add_child(join_node, on_condition);
    return join_node;
}

    // Далее можно добавить обработку WHERE и других опций

    return select_node;
}
