#include "../../include/parser.h"
#include "../../include/token.h"
#include "../../include/ast.h"
#include "../../include/error.h"
#include <stdlib.h>
#include <string.h>

/**
 * parse_method_declaration - Парсит объявление метода.
 *
 * Синтаксис:
 * METHODS <method_name>
 *   [IMPORTING param1 TYPE type1 ...]
 *   [EXPORTING ...]
 *   [CHANGING ...]
 *   [RETURNING VALUE(rv) TYPE type]
 *   [RAISING ...].
 *
 * Возвращает AST узел метода.
 */
ASTNode *parse_method_declaration(TokenStream *ts) {
    if (!ts) {
        report_error("TokenStream is NULL in parse_method_declaration");
        return NULL;
    }

    Token *token = token_stream_next(ts);
    if (!token || token->type != TOKEN_KEYWORD_METHODS) {
        report_error("Expected 'METHODS' keyword");
        return NULL;
    }

    token = token_stream_next(ts);
    if (!token || token->type != TOKEN_IDENTIFIER) {
        report_error("Expected method name after 'METHODS'");
        return NULL;
    }

    ASTNode *method_node = ast_node_create(AST_METHOD_DECL);
    if (!method_node) {
        report_error("Failed to create AST node for method declaration");
        return NULL;
    }

    method_node->method_name = strdup(token->text);

    while ((token = token_stream_peek(ts)) != NULL) {
        if (token->type == TOKEN_KEYWORD_IMPORTING) {
            token_stream_next(ts);
            method_node->importing_params = parse_parameter_list(ts);
        } else if (token->type == TOKEN_KEYWORD_EXPORTING) {
            token_stream_next(ts);
            method_node->exporting_params = parse_parameter_list(ts);
        } else if (token->type == TOKEN_KEYWORD_CHANGING) {
            token_stream_next(ts);
            method_node->changing_params = parse_parameter_list(ts);
        } else if (token->type == TOKEN_KEYWORD_RETURNING) {
            token_stream_next(ts);
            Token *value_tok = token_stream_next(ts);
            if (!value_tok || value_tok->type != TOKEN_KEYWORD_VALUE) {
                report_error("Expected VALUE after RETURNING");
                ast_node_free(method_node);
                return NULL;
            }

            token = token_stream_next(ts);
            if (!token || token->type != TOKEN_LPAREN) {
                report_error("Expected '(' after VALUE in RETURNING");
                ast_node_free(method_node);
                return NULL;
            }

            token = token_stream_next(ts);
            if (!token || token->type != TOKEN_IDENTIFIER) {
                report_error("Expected return variable name");
                ast_node_free(method_node);
                return NULL;
            }

            char *return_var = strdup(token->text);

            token = token_stream_next(ts);
            if (!token || token->type != TOKEN_RPAREN) {
                report_error("Expected ')' after return variable");
                free(return_var);
                ast_node_free(method_node);
                return NULL;
            }

            token = token_stream_next(ts);
            if (!token || token->type != TOKEN_KEYWORD_TYPE) {
                report_error("Expected TYPE after RETURNING VALUE(...)");
                free(return_var);
                ast_node_free(method_node);
                return NULL;
            }

            token = token_stream_next(ts);
            if (!token || token->type != TOKEN_IDENTIFIER) {
                report_error("Expected type after TYPE in RETURNING");
                free(return_var);
                ast_node_free(method_node);
                return NULL;
            }

            ASTNode *ret = ast_node_create(AST_METHOD_RETURN);
            if (!ret) {
                report_error("Failed to create return node");
                free(return_var);
                ast_node_free(method_node);
                return NULL;
            }

            ret->return_variable = return_var;
            ret->return_type = strdup(token->text);
            method_node->returning_param = ret;
        } else if (token->type == TOKEN_KEYWORD_RAISING) {
            token_stream_next(ts);
            method_node->raising_exceptions = parse_identifier_list(ts);
        } else {
            break;  // завершили парсинг объявления
        }
    }

    return method_node;
}


/////////////////////////////

#include "../../include/parser.h"
#include "../../include/token.h"
#include "../../include/ast.h"
#include "../../include/error.h"
#include <stdlib.h>
#include <string.h>

/**
 * parse_method_definition - Парсит объявление метода класса.
 *
 * Синтаксис:
 * METHODS <method_name>
 *   [IMPORTING <params>]
 *   [EXPORTING <params>]
 *   [CHANGING <params>]
 *   [RETURNING <param>]
 *   [RAISING <exceptions>].
 *
 * Возвращает AST узел метода.
 */
ASTNode *parse_method_definition(TokenStream *ts) {
    if (!ts) {
        report_error("TokenStream is NULL in parse_method_definition");
        return NULL;
    }

    Token *token = token_stream_next(ts);
    if (!token || token->type != TOKEN_KEYWORD_METHODS) {
        report_error("Expected 'METHODS' keyword");
        return NULL;
    }

    token = token_stream_next(ts);
    if (!token || token->type != TOKEN_IDENTIFIER) {
        report_error("Expected method name after 'METHODS'");
        return NULL;
    }

    ASTNode *method_node = ast_node_create(AST_METHOD_DEF);
    if (!method_node) {
        report_error("Failed to allocate AST node for method definition");
        return NULL;
    }

    method_node->method_name = strdup(token->text);
    method_node->parameters = ast_node_list_create();
    method_node->exceptions = ast_node_list_create();
    if (!method_node->parameters || !method_node->exceptions) {
        report_error("Failed to allocate parameters or exceptions list");
        ast_node_free(method_node);
        return NULL;
    }

    // Парсим параметры метода: IMPORTING, EXPORTING, CHANGING, RETURNING, RAISING
    while ((token = token_stream_peek(ts)) != NULL) {
        if (token->type == TOKEN_DOT) {
            token_stream_next(ts); // consume '.'
            break;
        }

        token = token_stream_next(ts);

        if (token->type == TOKEN_KEYWORD_IMPORTING ||
            token->type == TOKEN_KEYWORD_EXPORTING ||
            token->type == TOKEN_KEYWORD_CHANGING) {
            // Парсим список параметров
            // Пример: IMPORTING param TYPE type, param2 TYPE type2
            if (!parse_parameters(ts, method_node->parameters, token->type)) {
                report_error("Failed to parse parameters");
                ast_node_free(method_node);
                return NULL;
            }
        } else if (token->type == TOKEN_KEYWORD_RETURNING) {
            // Возвращаемый параметр
            if (!parse_returning_parameter(ts, method_node)) {
                report_error("Failed to parse RETURNING parameter");
                ast_node_free(method_node);
                return NULL;
            }
        } else if (token->type == TOKEN_KEYWORD_RAISING) {
            // Исключения
            if (!parse_exceptions(ts, method_node->exceptions)) {
                report_error("Failed to parse RAISING exceptions");
                ast_node_free(method_node);
                return NULL;
            }
        } else {
            report_error_token("Unexpected token in method definition", token);
            ast_node_free(method_node);
            return NULL;
        }
    }

    return method_node;
}

/**
 * parse_parameters - Парсит список параметров с их типами.
 *
 * @param ts - поток токенов
 * @param list - список параметров для добавления
 * @param direction - тип параметров (IMPORTING, EXPORTING, CHANGING)
 * @return 1 при успехе, 0 при ошибке
 */
static int parse_parameters(TokenStream *ts, ASTNodeList *list, TokenType direction) {
    while (1) {
        Token *name = token_stream_next(ts);
        if (!name || name->type != TOKEN_IDENTIFIER) {
            report_error("Expected parameter name");
            return 0;
        }

        Token *type_kw = token_stream_next(ts);
        if (!type_kw || type_kw->type != TOKEN_KEYWORD_TYPE) {
            report_error("Expected TYPE keyword after parameter name");
            return 0;
        }

        Token *type_name = token_stream_next(ts);
        if (!type_name || type_name->type != TOKEN_IDENTIFIER) {
            report_error("Expected type name after TYPE");
            return 0;
        }

        ASTNode *param = ast_node_create(AST_METHOD_PARAM);
        if (!param) {
            report_error("Failed to allocate parameter node");
            return 0;
        }

        param->param_name = strdup(name->text);
        param->param_type = strdup(type_name->text);
        param->param_direction = direction;

        ast_node_list_append(list, param);

        Token *next = token_stream_peek(ts);
        if (!next || next->type != TOKEN_COMMA) {
            break;
        }
        token_stream_next(ts); // consume comma
    }
    return 1;
}

/**
 * parse_returning_parameter - Парсит RETURNING параметр метода.
 *
 * @param ts - поток токенов
 * @param method_node - AST узел метода
 * @return 1 при успехе, 0 при ошибке
 */
static int parse_returning_parameter(TokenStream *ts, ASTNode *method_node) {
    Token *param_name = token_stream_next(ts);
    if (!param_name || param_name->type != TOKEN_IDENTIFIER) {
        report_error("Expected parameter name after RETURNING");
        return 0;
    }

    Token *type_kw = token_stream_next(ts);
    if (!type_kw || type_kw->type != TOKEN_KEYWORD_TYPE) {
        report_error("Expected TYPE keyword after RETURNING parameter name");
        return 0;
    }

    Token *type_name = token_stream_next(ts);
    if (!type_name || type_name->type != TOKEN_IDENTIFIER) {
        report_error("Expected type name after TYPE in RETURNING");
        return 0;
    }

    ASTNode *ret_param = ast_node_create(AST_METHOD_PARAM);
    if (!ret_param) {
        report_error("Failed to allocate RETURNING parameter node");
        return 0;
    }

    ret_param->param_name = strdup(param_name->text);
    ret_param->param_type = strdup(type_name->text);
    ret_param->param_direction = PARAM_RETURNING;

    ast_node_list_append(method_node->parameters, ret_param);

    return 1;
}

/**
 * parse_exceptions - Парсит список исключений RAISING.
 *
 * @param ts - поток токенов
 * @param list - список исключений
 * @return 1 при успехе, 0 при ошибке
 */
static int parse_exceptions(TokenStream *ts, ASTNodeList *list) {
    while (1) {
        Token *exc = token_stream_next(ts);
        if (!exc || exc->type != TOKEN_IDENTIFIER) {
            report_error("Expected exception identifier");
            return 0;
        }

        ASTNode *exc_node = ast_node_create(AST_EXCEPTION);
        if (!exc_node) {
            report_error("Failed to allocate exception node");
            return 0;
        }

        exc_node->exception_name = strdup(exc->text);
        ast_node_list_append(list, exc_node);

        Token *next = token_stream_peek(ts);
        if (!next || next->type != TOKEN_COMMA) {
            break;
        }
        token_stream_next(ts); // consume comma
    }
    return 1;
}
