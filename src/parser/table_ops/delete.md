Следующий файл — `parser/table_ops/delete.c`.

---

### Задача `delete.c` (table\_ops):

Парсинг операции `DELETE` для удаления записей из внутренних таблиц в ABAP.

---

## Реализация `delete.c`

```c
#include "../../include/parser.h"
#include "../../include/token.h"
#include "../../include/ast.h"
#include "../../include/error.h"
#include <stdlib.h>
#include <string.h>

/**
 * parse_table_ops_delete - Парсит инструкцию DELETE для удаления записей из внутренней таблицы.
 *
 * Ожидается, что ключевое слово DELETE уже прочитано.
 * Возвращает AST узел DELETE или NULL при ошибке.
 */
ASTNode *parse_table_ops_delete(TokenStream *ts) {
    if (!ts) {
        report_error("TokenStream is NULL in parse_table_ops_delete");
        return NULL;
    }

    ASTNode *delete_node = ast_node_create(AST_DELETE);
    if (!delete_node) {
        report_error("Failed to allocate AST node for DELETE");
        return NULL;
    }

    // Ожидается имя таблицы
    Token *table_tok = token_stream_next(ts);
    if (!table_tok || table_tok->type != TOKEN_IDENTIFIER) {
        report_error("Expected table name after DELETE");
        ast_node_free(delete_node);
        return NULL;
    }

    delete_node->string_value = strdup(table_tok->text);
    if (!delete_node->string_value) {
        report_error("Failed to allocate memory for table name in DELETE");
        ast_node_free(delete_node);
        return NULL;
    }

    // Опционально: WHERE <условие> или INDEX <число>
    Token *next_tok = token_stream_peek(ts);
    if (next_tok && (next_tok->type == TOKEN_WHERE || next_tok->type == TOKEN_INDEX)) {
        token_stream_next(ts); // consume WHERE/INDEX

        Token *cond_tok = token_stream_next(ts);
        if (!cond_tok) {
            report_error("Expected condition or index after WHERE/INDEX");
            ast_node_free(delete_node);
            return NULL;
        }

        ASTNode *cond_node = ast_node_create(
            next_tok->type == TOKEN_WHERE ? AST_WHERE_CONDITION : AST_INDEX
        );
        if (!cond_node) {
            report_error("Failed to allocate AST node for DELETE condition/index");
            ast_node_free(delete_node);
            return NULL;
        }

        cond_node->string_value = strdup(cond_tok->text);
        if (!cond_node->string_value) {
            report_error("Failed to allocate memory for DELETE condition/index value");
            ast_node_free(delete_node);
            ast_node_free(cond_node);
            return NULL;
        }

        ast_node_add_child(delete_node, cond_node);
    }

    return delete_node;
}
```

---

### Объяснение:

* Парсит DELETE <таблица> \[WHERE <условие>] или \[INDEX <номер>].
* Создаёт AST-узел `AST_DELETE` с дочерними узлами, если указано условие или индекс.
* Поддерживает как логическое, так и позиционное удаление.

---

Готов продолжать с следующим файлом!
