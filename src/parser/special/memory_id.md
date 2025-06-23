Следующий файл — `parser/special/memory_id.c`.

---

### Задача `memory_id.c` (special):

Обработка конструкций, связанных с `MEMORY ID` в ABAP, используемых для идентификации блоков памяти при операциях `EXPORT` и `IMPORT`.

---

## Реализация `memory_id.c`

```c
#include "../../include/parser.h"
#include "../../include/token.h"
#include "../../include/ast.h"
#include "../../include/error.h"

/**
 * parse_special_memory_id - Парсит конструкции MEMORY ID <id>.
 *
 * MEMORY ID используется в операторах EXPORT и IMPORT для указания
 * идентификатора памяти, где данные сохраняются или загружаются.
 *
 * Возвращает AST узел MEMORY_ID с идентификатором.
 */
ASTNode *parse_special_memory_id(TokenStream *ts) {
    if (!ts) {
        report_error("TokenStream is NULL in parse_special_memory_id");
        return NULL;
    }

    Token *memory_tok = token_stream_next(ts);
    if (!memory_tok || memory_tok->type != TOKEN_MEMORY) {
        report_error("Expected MEMORY token");
        return NULL;
    }

    Token *id_tok = token_stream_next(ts);
    if (!id_tok || id_tok->type != TOKEN_ID) {
        report_error("Expected ID token after MEMORY");
        return NULL;
    }

    Token *memid_tok = token_stream_next(ts);
    if (!memid_tok || memid_tok->type != TOKEN_IDENTIFIER) {
        report_error("Expected identifier token after ID");
        return NULL;
    }

    ASTNode *memory_id_node = ast_node_create(AST_SPECIAL_MEMORY_ID);
    if (!memory_id_node) {
        report_error("Failed to create AST node for MEMORY ID");
        return NULL;
    }

    memory_id_node->string_value = strdup(memid_tok->text);
    return memory_id_node;
}
```

---

### Пример ABAP:

```abap
EXPORT itab TO MEMORY ID 'MY_MEMID'.
IMPORT itab FROM MEMORY ID 'MY_MEMID'.
```

---

Если хочешь следующий файл — скажи `следующий файл`.
