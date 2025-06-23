#include "../../include/parser.h"
#include "../../include/token.h"
#include "../../include/error.h"
#include <string.h>

/**
 * parse_method_visibility - Парсит модификаторы видимости метода.
 *
 * Ожидает ключевые слова PUBLIC, PRIVATE или PROTECTED.
 * Возвращает тип видимости в виде enum или -1 при ошибке.
 */
int parse_method_visibility(TokenStream *ts) {
    if (!ts) {
        report_error("TokenStream is NULL in parse_method_visibility");
        return -1;
    }

    Token *token = token_stream_next(ts);
    if (!token) {
        report_error("Unexpected end of tokens while parsing method visibility");
        return -1;
    }

    if (token->type == TOKEN_KEYWORD_PUBLIC) {
        return VISIBILITY_PUBLIC;
    } else if (token->type == TOKEN_KEYWORD_PRIVATE) {
        return VISIBILITY_PRIVATE;
    } else if (token->type == TOKEN_KEYWORD_PROTECTED) {
        return VISIBILITY_PROTECTED;
    } else {
        report_error("Expected visibility specifier: PUBLIC, PRIVATE or PROTECTED");
        return -1;
    }
}
