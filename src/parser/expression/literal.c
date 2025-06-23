#include "../../include/parser.h"
#include "../../include/token.h"
#include "../../include/ast.h"
#include "../../include/error.h"
#include <stdlib.h>
#include <string.h>

/**
 * parse_literal - Парсит литеральное значение из токенов.
 *
 * Поддерживаются числовые литералы (целые и с плавающей точкой),
 * строковые литералы и булевы значения (TRUE, FALSE).
 *
 * Возвращает AST узел литерала или NULL при ошибке.
 */
ASTNode *parse_literal(TokenStream *ts) {
    if (!ts) {
        report_error("TokenStream is NULL in parse_literal");
        return NULL;
    }

    Token *token = token_stream_next(ts);
    if (!token) {
        report_error("Unexpected end of tokens while parsing literal");
        return NULL;
    }

    ASTNode *literal_node = ast_node_create(AST_LITERAL);
    if (!literal_node) {
        report_error("Failed to allocate AST node for literal");
        return NULL;
    }

    switch (token->type) {
        case TOKEN_INTEGER_LITERAL:
            literal_node->literal_type = LITERAL_INT;
            literal_node->int_value = atoi(token->text);
            break;
        case TOKEN_FLOAT_LITERAL:
            literal_node->literal_type = LITERAL_FLOAT;
            literal_node->float_value = atof(token->text);
            break;
        case TOKEN_STRING_LITERAL:
            literal_node->literal_type = LITERAL_STRING;
            literal_node->string_value = strdup(token->text);
            if (!literal_node->string_value) {
                report_error("Failed to allocate memory for string literal");
                ast_node_free(literal_node);
                return NULL;
            }
            break;
        case TOKEN_KEYWORD_TRUE:
            literal_node->literal_type = LITERAL_BOOL;
            literal_node->bool_value = 1;
            break;
        case TOKEN_KEYWORD_FALSE:
            literal_node->literal_type = LITERAL_BOOL;
            literal_node->bool_value = 0;
            break;
        default:
            report_error("Unexpected token type for literal");
            ast_node_free(literal_node);
            return NULL;
    }

    return literal_node;
}
