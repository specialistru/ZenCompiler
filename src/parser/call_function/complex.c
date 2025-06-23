#include "../../include/parser.h"
#include "../../include/token.h"
#include "../../include/ast.h"
#include "../../include/error.h"
#include <stdlib.h>

/**
 * parse_call_function_complex - Полноценный парсер конструкции CALL FUNCTION.
 *
 * Синтаксис:
 * CALL FUNCTION <name | (expression)>
 *   [EXPORTING ...]
 *   [IMPORTING ...]
 *   [CHANGING ...]
 *   [TABLES ...]
 *   [EXCEPTIONS ...].
 *
 * Возвращает AST узел вызова функции или NULL при ошибке.
 */
ASTNode *parse_call_function_complex(TokenStream *ts) {
    if (!ts) {
        report_error("TokenStream is NULL in parse_call_function_complex");
        return NULL;
    }

    Token *token = token_stream_next(ts);
    if (!token || token->type != TOKEN_KEYWORD_CALL) {
        report_error("Expected CALL keyword");
        return NULL;
    }

    token = token_stream_next(ts);
    if (!token || token->type != TOKEN_KEYWORD_FUNCTION) {
        report_error("Expected FUNCTION keyword");
        return NULL;
    }

    // Статический или динамический вызов
    ASTNode *call_node = NULL;
    token = token_stream_peek(ts);
    if (!token) {
        report_error("Unexpected end of input after CALL FUNCTION");
        return NULL;
    }

    if (token->type == TOKEN_LPAREN) {
        call_node = ast_node_create(AST_CALL_FUNCTION_DYNAMIC);
        token_stream_next(ts);  // '('
        call_node->dynamic_function_expr = parse_expression(ts);
        if (!call_node->dynamic_function_expr) {
            report_error("Failed to parse dynamic function name expression");
            ast_node_free(call_node);
            return NULL;
        }
        token = token_stream_next(ts);
        if (!token || token->type != TOKEN_RPAREN) {
            report_error("Expected ')' after dynamic function name");
            ast_node_free(call_node);
            return NULL;
        }
    } else if (token->type == TOKEN_IDENTIFIER) {
        call_node = ast_node_create(AST_CALL_FUNCTION);
        token = token_stream_next(ts);
        call_node->function_name = strdup(token->text);
    } else {
        report_error("Expected function name or dynamic expression");
        return NULL;
    }

    if (!call_node) {
        report_error("Failed to create AST node for CALL FUNCTION");
        return NULL;
    }

    // Чтение параметров
    while ((token = token_stream_peek(ts)) != NULL) {
        if (token->type == TOKEN_KEYWORD_EXPORTING) {
            token_stream_next(ts);
            call_node->exporting_params = parse_parameter_list(ts);
        } else if (token->type == TOKEN_KEYWORD_IMPORTING) {
            token_stream_next(ts);
            call_node->importing_params = parse_parameter_list(ts);
        } else if (token->type == TOKEN_KEYWORD_CHANGING) {
            token_stream_next(ts);
            call_node->changing_params = parse_parameter_list(ts);
        } else if (token->type == TOKEN_KEYWORD_TABLES) {
            token_stream_next(ts);
            call_node->tables_params = parse_parameter_list(ts);
        } else if (token->type == TOKEN_KEYWORD_EXCEPTIONS) {
            call_node->exceptions = parse_call_function_exceptions(ts);
        } else {
            break;
        }

        if (!call_node->exporting_params && !call_node->importing_params &&
            !call_node->changing_params && !call_node->tables_params && !call_node->exceptions) {
            report_error("Failed to parse parameters or EXCEPTIONS in CALL FUNCTION");
            ast_node_free(call_node);
            return NULL;
        }
    }

    return call_node;
}
