#include "../../include/parser.h"
#include "../../include/token.h"
#include "../../include/error.h"

/**
 * parse_endinterface - Парсит конец определения интерфейса.
 *
 * Ожидает конструкцию:
 * ENDINTERFACE.
 *
 * Возвращает 1 при успехе, 0 при ошибке.
 */
int parse_endinterface(TokenStream *ts) {
    if (!ts) {
        report_error("TokenStream is NULL in parse_endinterface");
        return 0;
    }

    Token *token = token_stream_next(ts);
    if (!token || token->type != TOKEN_KEYWORD_ENDINTERFACE) {
        report_error("Expected 'ENDINTERFACE' keyword");
        return 0;
    }

    token = token_stream_next(ts);
    if (!token || token->type != TOKEN_DOT) {
        report_error("Expected '.' after 'ENDINTERFACE'");
        return 0;
    }

    return 1;
}
