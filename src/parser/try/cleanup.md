Следующий файл — `parser/try/cleanup.c`.

---

### Задача `cleanup.c` (try):

Обработка блока CLEANUP в конструкции TRY...CLEANUP...ENDTRY в ABAP, предназначенного для выполнения кода очистки независимо от результата TRY.

---

## Реализация `cleanup.c`

```c
#include "../../include/parser.h"
#include "../../include/token.h"
#include "../../include/ast.h"
#include "../../include/error.h"
#include <stdlib.h>

/**
 * parse_cleanup_block - Парсит блок CLEANUP внутри конструкции TRY...ENDTRY.
 *
 * Ожидается, что ключевое слово CLEANUP уже было прочитано.
 * Возвращает AST узел CLEANUP с телом или NULL при ошибке.
 */

ASTNode *parse_cleanup_block(TokenStream *ts) {
    if (!ts) {
        report_error("TokenStream is NULL in parse_cleanup_block");
        return NULL;
    }

    ASTNode *cleanup_node = ast_node_create(AST_CLEANUP);
    if (!cleanup_node) {
        report_error("Failed to allocate AST node for CLEANUP");
        return NULL;
    }

    // Парсим тело CLEANUP до ENDTRY
    while (1) {
        Token *peek = token_stream_peek(ts);
        if (!peek) {
            report_error("Unexpected end of tokens inside CLEANUP block");
            ast_node_free(cleanup_node);
            return NULL;
        }

        if (peek->type == TOKEN_ENDTRY) {
            break;
        }

        ASTNode *stmt = parse_statement(ts);
        if (!stmt) {
            report_error("Failed to parse statement inside CLEANUP block");
            ast_node_free(cleanup_node);
            return NULL;
        }

        ast_node_add_child(cleanup_node, stmt);
    }

    return cleanup_node;
}
```

---

### Объяснение:

* Предполагается, что CLEANUP уже считан.
* Парсит все вложенные инструкции до ENDTRY.
* Возвращает AST узел CLEANUP с дочерними узлами.

---

Если хотите, могу перейти к следующему файлу.
