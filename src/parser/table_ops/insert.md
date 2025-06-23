Следующий файл — `parser/table_ops/insert.c`.

---

### Задача `insert.c` (table\_ops):

Парсинг операции INSERT для внутренних таблиц ABAP — вставка записи в таблицу с возможностью указания позиции.

---

## Реализация `insert.c`

```c
#include "../../include/parser.h"
#include "../../include/token.h"
#include "../../include/ast.h"
#include "../../include/error.h"
#include <stdlib.h>
#include <string.h>

/**
 * parse_table_ops_insert - Парсит конструкцию INSERT для вставки записи во внутреннюю таблицу.
 *
 * Ожидается, что ключевое слово INSERT уже прочитано.
 * Возвращает AST узел INSERT или NULL при ошибке.
 */
ASTNode *parse_table_ops_insert(TokenStream *ts) {
    if (!ts) {
        report_error("TokenStream is NULL in parse_table_ops_insert");
        return NULL;
    }

    ASTNode *insert_node = ast_node_create(AST_INSERT);
    if (!insert_node) {
        report_error("Failed to allocate AST node for INSERT");
        return NULL;
    }

    // Ожидаем запись или переменную, которую нужно вставить
    Token *record_tok = token_stream_next(ts);
    if (!record_tok) {
        report_error("Expected record identifier after INSERT");
        ast_node_free(insert_node);
        return NULL;
    }

    if (record_tok->type != TOKEN_IDENTIFIER) {
        report_error("Expected identifier after INSERT");
        ast_node_free(insert_node);
        return NULL;
    }

    insert_node->string_value = strdup(record_tok->text);
    if (!insert_node->string_value) {
        report_error("Failed to allocate memory for INSERT record identifier");
        ast_node_free(insert_node);
        return NULL;
    }

    // Опционально: ожидаем ключевые слова TO, INDEX, или POSITION, указывающие место вставки
    Token *next_tok = token_stream_peek(ts);
    if (next_tok && (next_tok->type == TOKEN_TO || next_tok->type == TOKEN_INDEX || next_tok->type == TOKEN_POSITION)) {
        token_stream_next(ts);  // consume TO/INDEX/POSITION

        // Ожидаем либо имя таблицы, либо индекс
        Token *pos_tok = token_stream_next(ts);
        if (!pos_tok) {
            report_error("Expected position or table identifier after TO/INDEX/POSITION in INSERT");
            ast_node_free(insert_node);
            return NULL;
        }

        ASTNode *pos_node = ast_node_create(AST_INSERT_POSITION);
        if (!pos_node) {
            report_error("Failed to allocate AST node for INSERT position");
            ast_node_free(insert_node);
            return NULL;
        }

        pos_node->string_value = strdup(pos_tok->text);
        if (!pos_node->string_value) {
            report_error("Failed to allocate memory for INSERT position");
            ast_node_free(insert_node);
            ast_node_free(pos_node);
            return NULL;
        }

        ast_node_add_child(insert_node, pos_node);
    }

    return insert_node;
}
```

---

### Объяснение:

* Парсит инструкцию INSERT с записью и (опционально) позицией вставки.
* Поддерживает разные варианты синтаксиса: `INSERT <record> TO <table>`, `INSERT <record> INDEX <n>`, `INSERT <record> POSITION <pos>`.
* Создаёт AST узлы для записи и позиции.

---

Готов продолжать с следующим файлом!
