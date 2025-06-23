#include "../../include/parser.h"
#include "../../include/token.h"
#include "../../include/ast.h"
#include "../../include/error.h"

/**
 * parse_endselect - Обрабатывает ключевое слово ENDSELECT, завершая цикл обработки результата SELECT.
 *
 * Возвращает AST узел ENDSELECT или NULL при ошибке.
 */
ASTNode *parse_endselect(TokenStream *ts) {
    if (!ts) {
        report_error("TokenStream is NULL in parse_endselect");
        return NULL;
    }

    // Просто создаём узел AST с типом ENDSELECT, поскольку это маркер конца
    ASTNode *endselect_node = ast_node_create(AST_ENDSELECT);
    if (!endselect_node) {
        report_error("Failed to allocate AST node for ENDSELECT");
        return NULL;
    }

    return endselect_node;
}
