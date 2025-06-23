#include "../../include/parser.h"
#include "../../include/token.h"
#include "../../include/ast.h"
#include "../../include/error.h"
#include <stdlib.h>

/**
 * parse_call_function_dynamic - Парсит динамический вызов функции.
 *
 * Синтаксис:
 * CALL FUNCTION (variable_name)
 *   [EXPORTING ...]
 *   [IMPORTING ...]
 *   [CHANGING ...]
 *
 * Возвращает AST узел вызова или NULL при ошибке.
 */
ASTNode *parse_call_function_dynamic(TokenStream *ts) {
    if (!ts) {
        report_error("TokenStream is NULL in parse_call_function_dynamic");
        return NULL;
    }

    // CALL
    Token *token = token_stream_next(ts);
    if (!token || token->type != TOKEN_KEYWORD_CALL) {
        report_error("Expected CALL keyword");
        return NULL;
    }

    // FUNCTION
    token = token_stream_next(ts);
    if (!token || token->type != TOKEN_KEYWORD_FUNCTION) {
        report_error("Expected FUNCTION keyword after CALL");
        return NULL;
    }

    // (
    token = token_stream_next(ts);
    if (!token || token->type != TOKEN_LPAREN) {
        report_error("Expected '(' for dynamic function name");
        return NULL;
    }

    // variable or expression inside ()
    ASTNode *dynamic_name_expr = parse_expression(ts);
    if (!dynamic_name_expr) {
        report_error("Failed to parse dynamic function name expression");
        return NULL;
    }

    // )
    token = token_stream_next(ts);
    if (!token || token->type != TOKEN_RPAREN) {
        report_error("Expected ')' after dynamic function name");
        ast_node_free(dynamic_name_expr);
        return NULL;
    }

    ASTNode *call_node = ast_node_create(AST_CALL_FUNCTION_DYNAMIC);
    if (!call_node) {
        report_error("Failed to create AST node for dynamic CALL FUNCTION");
        ast_node_free(dynamic_name_expr);
        return NULL;
    }

    call_node->dynamic_function_expr = dynamic_name_expr;

    // Обработка параметров
    while (1) {
        token = token_stream_peek(ts);
        if (!token) break;

        if (token->type == TOKEN_KEYWORD_EXPORTING) {
            token_stream_next(ts);
            ASTNodeList *exporting = parse_parameter_list(ts);
            if (!exporting) {
                report_error("Failed to parse EXPORTING parameters");
                ast_node_free(call_node);
                return NULL;
            }
            call_node->exporting_params = exporting;
        } else if (token->type == TOKEN_KEYWORD_IMPORTING) {
            token_stream_next(ts);
            ASTNodeList *importing = parse_parameter_list(ts);
            if (!importing) {
                report_error("Failed to parse IMPORTING parameters");
                ast_node_free(call_node);
                return NULL;
            }
            call_node->importing_params = importing;
        } else if (token->type == TOKEN_KEYWORD_CHANGING) {
            token_stream_next(ts);
            ASTNodeList *changing = parse_parameter_list(ts);
            if (!changing) {
                report_error("Failed to parse CHANGING parameters");
                ast_node_free(call_node);
                return NULL;
            }
            call_node->changing_params = changing;
        } else {
            break;
        }
    }

    return call_node;
}
