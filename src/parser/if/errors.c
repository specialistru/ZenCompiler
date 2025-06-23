#include "../../include/error.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * report_if_error - Выводит информативное сообщение об ошибке парсинга IF.
 * @msg: Сообщение ошибки.
 * @line: Номер строки, где произошла ошибка.
 */
void report_if_error(const char *msg, int line) {
    if (!msg) return;
    fprintf(stderr, "Parsing error in IF statement at line %d: %s\n", line, msg);
}
