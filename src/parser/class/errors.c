#include "../../include/error.h"
#include "../../include/token.h"
#include <stdio.h>

/**
 * report_class_error - Выводит ошибку, связанную с парсингом класса.
 *
 * @param message - Сообщение об ошибке.
 * @param token - Токен, на котором обнаружена ошибка (может быть NULL).
 */
void report_class_error(const char *message, const Token *token) {
    if (token) {
        fprintf(stderr, "Class parser error at line %d, column %d: %s (token: '%s')\n",
                token->line, token->column, message, token->text);
    } else {
        fprintf(stderr, "Class parser error: %s\n", message);
    }
}
