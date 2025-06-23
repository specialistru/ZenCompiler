#include "../../include/error.h"
#include "../../include/token.h"
#include <stdio.h>

/**
 * report_form_syntax_error - Выводит подробное сообщение об ошибке синтаксиса
 * при парсинге конструкции FORM.
 *
 * @param token   - Токен, на котором обнаружена ошибка.
 * @param message - Сообщение с описанием ошибки.
 *
 * Функция логирует ошибку с указанием позиции и контекста.
 */

void report_form_syntax_error(const Token *token, const char *message) {
    if (!token || !message) {
        fprintf(stderr, "Syntax error in FORM: unknown location - %s\n", message ? message : "no message");
        return;
    }

    fprintf(stderr, "Syntax error in FORM at line %d, column %d: %s\n",
            token->line, token->column, message);
}
