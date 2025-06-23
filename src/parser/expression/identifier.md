Следующий файл — `parser/expression/identifier.c`.

---

### Задача `identifier.c` (expression):

Парсинг идентификаторов (имен переменных, функций, классов и т.п.) в выражениях.

---

## Реализация `identifier.c`

```c
#include "../../include/parser.h"
#include "../../include/token.h"
#include "../../include/ast.h"
#include "../../include/error.h"
#include <stdlib.h>
#include <string.h>

/**
 * parse_identifier - Парсит идентификатор из токенов.
 *
 * Идентификатор — последовательность символов, соответствующая правилам ABAP,
 * обычно начинается с буквы и может содержать буквы, цифры, подчеркивания.
 *
 * Возвращает AST узел идентификатора или NULL при ошибке.
 */
ASTNode *parse_identifier(TokenStream *ts) {
    if (!ts) {
        report_error("TokenStream is NULL in parse_identifier");
        return NULL;
    }

    Token *token = token_stream_next(ts);
    if (!token) {
        report_error("Unexpected end of tokens while parsing identifier");
        return NULL;
    }

    if (token->type != TOKEN_IDENTIFIER) {
        report_error("Expected identifier token");
        return NULL;
    }

    ASTNode *identifier_node = ast_node_create(AST_IDENTIFIER);
    if (!identifier_node) {
        report_error("Failed to allocate AST node for identifier");
        return NULL;
    }

    identifier_node->string_value = strdup(token->text);
    if (!identifier_node->string_value) {
        report_error("Failed to allocate memory for identifier string");
        ast_node_free(identifier_node);
        return NULL;
    }

    return identifier_node;
}
```

---

### Объяснение:

* Получает следующий токен и проверяет, что это идентификатор.
* Создаёт AST узел с именем, скопированным из токена.
* Обрабатывает ошибки памяти и неверные токены.

---

Готов продолжить!
