Следующий файл — `parser/table_ops/sort.c`.

---

### Задача `sort.c` (table\_ops):

Парсинг инструкции `SORT` в ABAP — сортировка внутренней таблицы, опционально по ключу или полям.

---

## Реализация `sort.c`

```c
#include "../../include/parser.h"
#include "../../include/token.h"
#include "../../include/ast.h"
#include "../../include/error.h"
#include <stdlib.h>
#include <string.h>

/**
 * parse_table_ops_sort - Парсит инструкцию SORT для сортировки внутренней таблицы.
 *
 * Ожидается, что ключевое слово SORT уже прочитано.
 * Возвращает AST узел SORT или NULL при ошибке.
 */
ASTNode *parse_table_ops_sort(TokenStream *ts) {
    if (!ts) {
        report_error("TokenStream is NULL in parse_table_ops_sort");
        return NULL;
    }

    ASTNode *sort_node = ast_node_create(AST_SORT);
    if (!sort_node) {
        report_error("Failed to allocate AST node for SORT");
        return NULL;
    }

    // Ожидается имя таблицы
    Token *table_tok = token_stream_next(ts);
    if (!table_tok || table_tok->type != TOKEN_IDENTIFIER) {
        report_error("Expected table name after SORT");
        ast_node_free(sort_node);
        return NULL;
    }

    sort_node->string_value = strdup(table_tok->text);
    if (!sort_node->string_value) {
        report_error("Failed to allocate memory for table name in SORT");
        ast_node_free(sort_node);
        return NULL;
    }

    // Опционально: BY <field1> <field2> ...
    Token *next_tok = token_stream_peek(ts);
    if (next_tok && next_tok->type == TOKEN_BY) {
        token_stream_next(ts); // consume BY

        ASTNode *by_node = ast_node_create(AST_SORT_BY);
        if (!by_node) {
            report_error("Failed to allocate AST node for SORT BY");
            ast_node_free(sort_node);
            return NULL;
        }

        Token *field_tok;
        while ((field_tok = token_stream_peek(ts)) &&
               field_tok->type == TOKEN_IDENTIFIER) {
            field_tok = token_stream_next(ts);

            ASTNode *field_node = ast_node_create(AST_IDENTIFIER);
            if (!field_node) {
                report_error("Failed to allocate AST node for sort field");
                ast_node_free(sort_node);
                ast_node_free(by_node);
                return NULL;
            }

            field_node->string_value = strdup(field_tok->text);
            if (!field_node->string_value) {
                report_error("Failed to allocate memory for sort field name");
                ast_node_free(sort_node);
                ast_node_free(by_node);
                ast_node_free(field_node);
                return NULL;
            }

            ast_node_add_child(by_node, field_node);
        }

        ast_node_add_child(sort_node, by_node);
    }

    return sort_node;
}
```

---

### Объяснение:

* Парсит инструкцию `SORT <таблица>` или `SORT <таблица> BY <поле1> <поле2> ...`.
* Главный узел AST — `AST_SORT`, с опциональным дочерним `AST_SORT_BY`, содержащим идентификаторы полей.
* Обрабатывает множественные поля сортировки, если они указаны.

---

Готов продолжить с следующим файлом!
