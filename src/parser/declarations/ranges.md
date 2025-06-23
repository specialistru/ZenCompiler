Следующий файл — `parser/declarations/ranges.c`.

---

### Задача `ranges.c` (declarations):

Парсинг конструкции `RANGES` в ABAP — объявление таблиц диапазонов для фильтрации данных.

---

## Реализация `ranges.c`

```c
#include "../../include/parser.h"
#include "../../include/token.h"
#include "../../include/ast.h"
#include "../../include/error.h"
#include <stdlib.h>
#include <string.h>

/**
 * parse_declaration_ranges - Парсит объявление RANGES.
 *
 * Пример:
 *   RANGES: r_matnr FOR mara-matnr.
 *
 * Возвращает AST узел RANGES_DECL или NULL при ошибке.
 */
ASTNode *parse_declaration_ranges(TokenStream *ts) {
    if (!ts) {
        report_error("TokenStream is NULL in parse_declaration_ranges");
        return NULL;
    }

    ASTNode *ranges_node = ast_node_create(AST_RANGES_DECL);
    if (!ranges_node) {
        report_error("Failed to create AST node for RANGES");
        return NULL;
    }

    // Имя таблицы диапазонов
    Token *id_tok = token_stream_next(ts);
    if (!id_tok || id_tok->type != TOKEN_IDENTIFIER) {
        report_error("Expected identifier after RANGES");
        ast_node_free(ranges_node);
        return NULL;
    }

    ranges_node->string_value = strdup(id_tok->text);
    if (!ranges_node->string_value) {
        report_error("Failed to store RANGES name");
        ast_node_free(ranges_node);
        return NULL;
    }

    // Ожидаем FOR
    Token *for_tok = token_stream_next(ts);
    if (!for_tok || for_tok->type != TOKEN_FOR) {
        report_error("Expected FOR keyword in RANGES declaration");
        ast_node_free(ranges_node);
        return NULL;
    }

    // Целевое поле
    Token *target_tok = token_stream_next(ts);
    if (!target_tok || target_tok->type != TOKEN_IDENTIFIER) {
        report_error("Expected field name after FOR in RANGES");
        ast_node_free(ranges_node);
        return NULL;
    }

    ASTNode *target_node = ast_node_create(AST_FOR_TARGET);
    if (!target_node) {
        report_error("Failed to create AST node for FOR target");
        ast_node_free(ranges_node);
        return NULL;
    }

    target_node->string_value = strdup(target_tok->text);
    ast_node_add_child(ranges_node, target_node);

    return ranges_node;
}
```

---

### Пример ABAP:

```abap
RANGES: r_date FOR sy-datum.
RANGES: r_matnr FOR mara-matnr.
```

---

### Основные AST узлы:

* `AST_RANGES_DECL` — корневой узел RANGES объявления;
* `AST_FOR_TARGET` — целевое поле диапазонов.

---

Если нужен следующий файл — скажи `следующий файл`.
