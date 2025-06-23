#include "../../include/parser.h"
#include "../../include/token.h"
#include "../../include/ast.h"
#include "../../include/error.h"
#include <stdlib.h>
#include <string.h>

/**
 * parse_declaration_select_options - Парсит конструкцию SELECT-OPTIONS.
 *
 * Примеры:
 *   SELECT-OPTIONS: s_date FOR sy-datum.
 *   SELECT-OPTIONS: s_matnr FOR mara-matnr OBLIGATORY.
 *   SELECT-OPTIONS: s_name FOR zemployee-name NO INTERVALS.
 *
 * Возвращает AST узел SELECT_OPTIONS.
 */
ASTNode *parse_declaration_select_options(TokenStream *ts) {
    if (!ts) {
        report_error("TokenStream is NULL in parse_declaration_select_options");
        return NULL;
    }

    ASTNode *selopt_node = ast_node_create(AST_SELECT_OPTIONS_DECL);
    if (!selopt_node) {
        report_error("Failed to create AST node for SELECT-OPTIONS");
        return NULL;
    }

    // Имя диапазона (обычно начинается с "s_")
    Token *id_tok = token_stream_next(ts);
    if (!id_tok || id_tok->type != TOKEN_IDENTIFIER) {
        report_error("Expected identifier after SELECT-OPTIONS");
        ast_node_free(selopt_node);
        return NULL;
    }

    selopt_node->string_value = strdup(id_tok->text);
    if (!selopt_node->string_value) {
        report_error("Failed to store SELECT-OPTIONS name");
        ast_node_free(selopt_node);
        return NULL;
    }

    // Ожидаем "FOR"
    Token *for_tok = token_stream_next(ts);
    if (!for_tok || for_tok->type != TOKEN_FOR) {
        report_error("Expected FOR keyword in SELECT-OPTIONS");
        ast_node_free(selopt_node);
        return NULL;
    }

    // Поле, к которому относится select-option
    Token *target_tok = token_stream_next(ts);
    if (!target_tok || target_tok->type != TOKEN_IDENTIFIER) {
        report_error("Expected field name after FOR in SELECT-OPTIONS");
        ast_node_free(selopt_node);
        return NULL;
    }

    ASTNode *target_node = ast_node_create(AST_FOR_TARGET);
    if (!target_node) {
        report_error("Failed to allocate AST node for FOR target");
        ast_node_free(selopt_node);
        return NULL;
    }

    target_node->string_value = strdup(target_tok->text);
    ast_node_add_child(selopt_node, target_node);

    // Обработка дополнительных модификаторов: OBLIGATORY, NO INTERVALS, etc.
    Token *tok;
    while ((tok = token_stream_peek(ts)) != NULL &&
           (tok->type == TOKEN_OBLIGATORY || tok->type == TOKEN_NO)) {
        tok = token_stream_next(ts); // consume

        if (tok->type == TOKEN_OBLIGATORY) {
            ASTNode *obl = ast_node_create(AST_OBLIGATORY_SPEC);
            ast_node_add_child(selopt_node, obl);
        } else if (tok->type == TOKEN_NO) {
            Token *next = token_stream_next(ts);
            if (!next || next->type != TOKEN_INTERVALS) {
                report_error("Expected 'INTERVALS' after 'NO' in SELECT-OPTIONS");
                ast_node_free(selopt_node);
                return NULL;
            }
            ASTNode *no_int = ast_node_create(AST_NO_INTERVALS_SPEC);
            ast_node_add_child(selopt_node, no_int);
        }
    }

    return selopt_node;
}
