#include "../../include/parser.h"
#include "../../include/token.h"
#include "../../include/ast.h"
#include "../../include/error.h"

/**
 * parse_control_continue - Парсит оператор CONTINUE.
 *
 * Оператор CONTINUE пропускает оставшуюся часть текущей итерации цикла
 * и переходит к следующей итерации.
 * Синтаксис: CONTINUE.
 *
 * Возвращает AST узел оператора CONTINUE.
 */
ASTNode *parse_control_continue(TokenStream *ts) {
    if (!ts) {
        report_error("TokenStream is NULL in parse_control_continue");
        return NULL;
    }

    Token *tok = token_stream_next(ts);
    if (!tok || tok->type != TOKEN_CONTINUE) {
        report_error("Expected CONTINUE token");
        return NULL;
    }

    ASTNode *continue_node = ast_node_create(AST_CONTROL_CONTINUE);
    if (!continue_node) {
        report_error("Failed to create AST node for CONTINUE");
        return NULL;
    }

    return continue_node;
}
