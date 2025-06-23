Следующий файл — `parser/perform/else.c`.

---

### Задача `else.c` для perform:

Парсинг конструкции ELSE внутри PERFORM, которая является блоком без условия и содержит тело.

---

## Реализация `else.c`

```c
#include "../../include/parser.h"
#include "../../include/token.h"
#include "../../include/ast.h"
#include "../../include/error.h"
#include <stdlib.h>

/**
 * parse_perform_else - Парсит конструкцию ELSE внутри PERFORM.
 *
 * Синтаксис:
 * ELSE
 *   <тело блока>
 *
 * Возвращает AST узел ELSE или NULL при ошибке.
 */
ASTNode *parse_perform_else(TokenStream *ts) {
    if (!ts) {
        report_error("TokenStream is NULL in parse_perform_else");
        return NULL;
    }

    Token *token = token_stream_next(ts);
    if (!token || token->type != TOKEN_KEYWORD_ELSE) {
        report_error("Expected ELSE keyword");
        return NULL;
    }

    // Парсим тело блока ELSE до конца блока (например, до ENDIF или другого ключевого слова)
    ASTNodeList *body = parse_statement_block(ts);
    if (!body) {
        report_error("Failed to parse body of ELSE");
        return NULL;
    }

    ASTNode *else_node = ast_node_create(AST_ELSE);
    if (!else_node) {
        report_error("Failed to create AST node for ELSE");
        ast_node_list_free(body);
        return NULL;
    }

    else_node->body = body;
    return else_node;
}
```

---

### Объяснение:

* Проверяет наличие ключевого слова ELSE.
* Парсит тело блока после ELSE.
* Возвращает AST узел с телом блока ELSE.
* Ожидается, что `parse_statement_block` корректно парсит блок кода.

---

Готов к следующему файлу!
