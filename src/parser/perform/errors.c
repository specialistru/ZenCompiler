#include "../../include/error.h"
#include "../../include/token.h"
#include "../../include/parser.h"
#include <stdio.h>

/**
 * report_perform_error - Регистрирует ошибку при парсинге PERFORM.
 * 
 * Ведёт журнал с описанием ошибки и положением в исходном тексте.
 *
 * @param message - Текст сообщения об ошибке.
 * @param token - Токен, при котором произошла ошибка (может быть NULL).
 */
void report_perform_error(const char *message, const Token *token) {
    if (token) {
        fprintf(stderr, "Perform parse error at line %d, column %d: %s (token: '%s')\n",
            token->line, token->column, message, token->text ? token->text : "<no text>");
    } else {
        fprintf(stderr, "Perform parse error: %s\n", message);
    }
}

/**
 * check_perform_syntax - Проверяет синтаксис PERFORM после основного парсинга,
 * например, наличие обязательных параметров, корректность блоков.
 *
 * @param node - AST узел PERFORM.
 * @return 1 если ошибки не обнаружены, 0 иначе.
 */
int check_perform_syntax(const ASTNode *node) {
    if (!node || node->type != AST_PERFORM) {
        fprintf(stderr, "Invalid AST node passed to check_perform_syntax\n");
        return 0;
    }

    if (!node->form_name || node->form_name[0] == '\0') {
        fprintf(stderr, "PERFORM statement missing form name\n");
        return 0;
    }

    // Дополнительные проверки можно добавить здесь.

    return 1; // синтаксис корректен
}
