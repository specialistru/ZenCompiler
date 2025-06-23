Следующий файл — `parser/expression/bracket.c`.

---

### Задача `bracket.c` (expression):

Парсинг выражений, заключённых в круглые скобки `( ... )`, для изменения приоритета операций.

---

## Реализация `bracket.c`

```c
#include "../../include/parser.h"
#include "../../include/token.h"
#include "../../include/ast.h"
#include "../../include/error.h"
#include <stdlib.h>

/**
 * parse_bracket_expression - Парсит выражение в круглых скобках.
 *
 * Ожидается, что текущий токен — открывающая скобка '('.
 * После неё парсится вложенное выражение, затем должна быть закрывающая скобка ')'.
 *
 * Возвращает AST узел вложенного выражения или NULL при ошибке.
 */
ASTNode *parse_bracket_expression(TokenStream *ts) {
    if (!ts) {
        report_error("TokenStream is NULL in parse_bracket_expression");
        return NULL;
    }

    Token *token = token_stream_next(ts);
    if (!token || token->type != TOKEN_LPAREN) {
        report_error("Expected '(' at start of bracket expression");
        return NULL;
    }

    // Здесь вызываем функцию парсинга общего выражения (parse_expression)
    // Предположим, она существует и возвращает ASTNode*
    ASTNode *expr_node = parse_expression(ts);
    if (!expr_node) {
        report_error("Failed to parse expression inside brackets");
        return NULL;
    }

    token = token_stream_next(ts);
    if (!token || token->type != TOKEN_RPAREN) {
        report_error("Expected ')' after bracketed expression");
        ast_node_free(expr_node);
        return NULL;
    }

    // Для удобства можно обернуть выражение в специальный AST узел BRACKETED_EXPR,
    // либо возвращать expr_node напрямую, если нет такой необходимости.
    ASTNode *bracket_node = ast_node_create(AST_BRACKETED_EXPR);
    if (!bracket_node) {
        report_error("Failed to allocate AST node for bracketed expression");
        ast_node_free(expr_node);
        return NULL;
    }

    ast_node_add_child(bracket_node, expr_node);
    return bracket_node;
}
```

---

### Объяснение:

* Проверяет наличие открывающей и закрывающей скобок.
* Внутри скобок парсит вложенное выражение (через `parse_expression`).
* Оборачивает результат в специальный AST узел для обозначения скобок.
* Обрабатывает ошибки синтаксиса.

---

Готов перейти к следующему файлу!
