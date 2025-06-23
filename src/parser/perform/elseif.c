#include "../../include/parser.h"
#include "../../include/token.h"
#include "../../include/ast.h"
#include "../../include/error.h"
#include <stdlib.h>

/**
 * parse_perform_elseif - Парсит конструкцию ELSEIF внутри PERFORM.
 *
 * Синтаксис:
 * ELSEIF <логическое выражение>.
 *
 * Возвращает AST узел ELSEIF или NULL при ошибке.
 */
ASTNode *parse_perform_elseif(TokenStream *ts) {
    if (!ts) {
        report_error("TokenStream is NULL in parse_perform_elseif");
        return NULL;
    }

    Token *token = token_stream_next(ts);
    if (!token || token->type != TOKEN_KEYWORD_ELSEIF) {
        report_error("Expected ELSEIF keyword");
        return NULL;
    }

    // Парсим условие после ELSEIF
    ASTNode *condition = parse_logical_expression(ts);
    if (!condition) {
        report_error("Failed to parse condition in ELSEIF");
        return NULL;
    }

    // Создаем AST узел для ELSEIF
    ASTNode *elseif_node = ast_node_create(AST_ELSEIF);
    if (!elseif_node) {
        report_error("Failed to create AST node for ELSEIF");
        ast_node_free(condition);
        return NULL;
    }

    elseif_node->condition = condition;

    // Парсим тело ELSEIF, ожидается до следующего ключевого слова или ENDIF
    ASTNodeList *body = parse_statement_block(ts);
    if (!body) {
        report_error("Failed to parse body of ELSEIF");
        ast_node_free(elseif_node);
        return NULL;
    }
    elseif_node->body = body;

    return elseif_node;
}
