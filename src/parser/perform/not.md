Следующий файл — `parser/perform/not.c`.

---

### Задача `not.c` для perform:

Парсинг оператора логического отрицания NOT в контексте PERFORM.

---

## Реализация `not.c`

```c
#include "../../include/parser.h"
#include "../../include/token.h"
#include "../../include/ast.h"
#include "../../include/error.h"
#include <stdlib.h>

/**
 * parse_perform_not - Парсит логическую операцию NOT.
 *
 * Синтаксис:
 * NOT <логическое выражение>
 *
 * Возвращает AST узел или NULL при ошибке.
 */
ASTNode *parse_perform_not(TokenStream *ts) {
    if (!ts) {
        report_error("TokenStream is NULL in parse_perform_not");
        return NULL;
    }

    Token *token = token_stream_next(ts);
    if (!token || token->type != TOKEN_KEYWORD_NOT) {
        report_error("Expected NOT keyword");
        return NULL;
    }

    ASTNode *operand = parse_simple_expression(ts);
    if (!operand) {
        report_error("Failed to parse operand after NOT");
        return NULL;
    }

    ASTNode *not_node = ast_node_create(AST_LOGICAL_NOT);
    if (!not_node) {
        report_error("Failed to create AST node for NOT operation");
        ast_node_free(operand);
        return NULL;
    }

    not_node->operand = operand;
    return not_node;
}
```

---

### Объяснение:

* Проверяет наличие ключевого слова NOT.
* Парсит за ним логическое выражение.
* Создаёт AST узел логического отрицания.
* Возвращает узел или NULL при ошибках.

---

Готов к следующему файлу!
