Конечно! Следующий файл — `parser/if/not.c`.

---

### Задача `not.c`:

Реализовать парсер логического оператора NOT в условиях IF.

---

## Реализация `not.c`

```c
#include "../../include/parser.h"
#include "../../include/token.h"
#include "../../include/ast.h"
#include "../../include/error.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * parse_not_expression - Парсит логический оператор NOT.
 *
 * Возвращает AST узел NOT с подвыражением, или NULL при ошибке.
 */
ASTNode *parse_not_expression(TokenStream *ts) {
    if (!ts) {
        fprintf(stderr, "TokenStream is NULL\n");
        return NULL;
    }

    Token *token = token_stream_peek(ts);
    if (!token || token->type != TOKEN_KEYWORD_NOT) {
        report_error("Expected NOT keyword");
        return NULL;
    }
    token_stream_next(ts); // съесть NOT

    ASTNode *not_node = ast_node_create(AST_NOT_OP);
    if (!not_node) {
        report_error("Failed to create AST node for NOT operator");
        return NULL;
    }

    ASTNode *expr = parse_expression(ts);
    if (!expr) {
        report_error("Failed to parse expression after NOT");
        ast_node_free(not_node);
        return NULL;
    }

    not_node->child = expr;

    return not_node;
}
```

---

### Объяснение:

* Проверяем наличие ключевого слова NOT.
* Создаем AST узел NOT.
* Парсим выражение, к которому применяется NOT.
* Возвращаем узел.

---

Если нужно — могу продолжить дальше!
