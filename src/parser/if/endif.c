#include "../../include/parser.h"
#include "../../include/token.h"
#include "../../include/error.h"
#include <stdio.h>

/**
 * parse_if_end - Проверяет и парсит ключевое слово ENDIF с точкой.
 *
 * Возвращает 0 при успехе, -1 при ошибке.
 */
int parse_if_end(TokenStream *ts) {
    if (!ts) {
        fprintf(stderr, "TokenStream is NULL\n");
        return -1;
    }

    Token *token = token_stream_next(ts);
    if (!token || token->type != TOKEN_KEYWORD_ENDIF) {
        report_error("Expected ENDIF keyword");
        return -1;
    }

    token = token_stream_next(ts);
    if (!token || token->type != TOKEN_DOT) {
        report_error("Expected '.' after ENDIF");
        return -1;
    }

    return 0; // успех
}
