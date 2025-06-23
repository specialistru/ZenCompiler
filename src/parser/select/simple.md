Следующий файл — `parser/select/simple.c`.

---

### Задача `simple.c` (select):

Парсинг простейших операторов SELECT из ABAP, поддерживающий выборку из одной таблицы с базовыми условиями.

---

## Реализация `simple.c`

```c
#include "../../include/parser.h"
#include "../../include/token.h"
#include "../../include/ast.h"
#include "../../include/error.h"
#include <stdlib.h>
#include <string.h>

/**
 * parse_simple_select - Парсит простой оператор SELECT в ABAP.
 *
 * Ожидается, что ключевое слово SELECT уже было прочитано.
 * Возвращает AST узел SELECT или NULL при ошибке.
 */

ASTNode *parse_simple_select(TokenStream *ts) {
    if (!ts) {
        report_error("TokenStream is NULL in parse_simple_select");
        return NULL;
    }

    ASTNode *select_node = ast_node_create(AST_SELECT);
    if (!select_node) {
        report_error("Failed to allocate AST node for SELECT");
        return NULL;
    }

    // Парсим список полей до ключевого слова FROM
    ASTNode *fields_node = ast_node_create(AST_FIELD_LIST);
    if (!fields_node) {
        report_error("Failed to allocate AST node for field list");
        ast_node_free(select_node);
        return NULL;
    }

    Token *tok = NULL;
    while ((tok = token_stream_peek(ts)) != NULL && tok->type != TOKEN_FROM) {
        tok = token_stream_next(ts);

        if (tok->type == TOKEN_COMMA) {
            continue; // пропускаем запятые
        }

        if (tok->type != TOKEN_IDENTIFIER) {
            report_error("Expected field identifier in SELECT statement");
            ast_node_free(select_node);
            ast_node_free(fields_node);
            return NULL;
        }

        ASTNode *field_node = ast_node_create(AST_FIELD);
        if (!field_node) {
            report_error("Failed to allocate AST node for field");
            ast_node_free(select_node);
            ast_node_free(fields_node);
            return NULL;
        }

        field_node->string_value = strdup(tok->text);
        if (!field_node->string_value) {
            report_error("Failed to allocate memory for field name");
            ast_node_free(select_node);
            ast_node_free(fields_node);
            return NULL;
        }

        ast_node_add_child(fields_node, field_node);
    }

    if (!tok || tok->type != TOKEN_FROM) {
        report_error("Expected FROM keyword in SELECT statement");
        ast_node_free(select_node);
        ast_node_free(fields_node);
        return NULL;
    }
    token_stream_next(ts); // consume FROM

    // Парсим имя таблицы после FROM
    Token *table_tok = token_stream_next(ts);
    if (!table_tok || table_tok->type != TOKEN_IDENTIFIER) {
        report_error("Expected table name after FROM");
        ast_node_free(select_node);
        ast_node_free(fields_node);
        return NULL;
    }

    ASTNode *table_node = ast_node_create(AST_TABLE);
    if (!table_node) {
        report_error("Failed to allocate AST node for table");
        ast_node_free(select_node);
        ast_node_free(fields_node);
        return NULL;
    }

    table_node->string_value = strdup(table_tok->text);
    if (!table_node->string_value) {
        report_error("Failed to allocate memory for table name");
        ast_node_free(select_node);
        ast_node_free(fields_node);
        return NULL;
    }

    ast_node_add_child(select_node, fields_node);
    ast_node_add_child(select_node, table_node);

    // Опционально, можно расширить поддержку WHERE, INTO и т.п.

    return select_node;
}
```

---

### Объяснение:

* Парсит ключевое слово SELECT (ожидается уже прочитано).
* Собирает список полей до FROM.
* Парсит имя таблицы после FROM.
* Возвращает AST узел SELECT с детьми: список полей и таблица.

---

Если нужен следующий файл — готов продолжить!

////////////////////////////

Следующий файл — `parser/select/simple.c`.

---

### Задача `simple.c` (select):

Парсинг простого SELECT-запроса без JOIN, только выборка полей и FROM.

---

## Реализация `simple.c`

```c
#include "../../include/parser.h"
#include "../../include/token.h"
#include "../../include/ast.h"
#include "../../include/error.h"
#include <stdlib.h>
#include <string.h>

/**
 * parse_select_simple - Парсит простой SELECT-запрос без JOIN.
 *
 * Ожидается, что ключевое слово SELECT уже прочитано.
 * Возвращает AST узел SELECT или NULL при ошибке.
 */
ASTNode *parse_select_simple(TokenStream *ts) {
    if (!ts) {
        report_error("TokenStream is NULL in parse_select_simple");
        return NULL;
    }

    ASTNode *select_node = ast_node_create(AST_SELECT);
    if (!select_node) {
        report_error("Failed to allocate AST node for SELECT");
        return NULL;
    }

    // Парсим список полей до ключевого слова FROM
    ASTNode *fields_node = ast_node_create(AST_FIELD_LIST);
    if (!fields_node) {
        report_error("Failed to allocate AST node for field list");
        ast_node_free(select_node);
        return NULL;
    }

    Token *tok = NULL;
    while ((tok = token_stream_peek(ts)) != NULL && tok->type != TOKEN_FROM) {
        tok = token_stream_next(ts);

        if (tok->type == TOKEN_COMMA) {
            continue;
        }

        if (tok->type != TOKEN_IDENTIFIER) {
            report_error("Expected identifier in SELECT fields");
            ast_node_free(select_node);
            ast_node_free(fields_node);
            return NULL;
        }

        ASTNode *field_node = ast_node_create(AST_FIELD);
        if (!field_node) {
            report_error("Failed to allocate AST node for field");
            ast_node_free(select_node);
            ast_node_free(fields_node);
            return NULL;
        }

        field_node->string_value = strdup(tok->text);
        if (!field_node->string_value) {
            report_error("Failed to allocate memory for field name");
            ast_node_free(select_node);
            ast_node_free(fields_node);
            ast_node_free(field_node);
            return NULL;
        }

        ast_node_add_child(fields_node, field_node);
    }

    if (!tok || tok->type != TOKEN_FROM) {
        report_error("Expected FROM keyword in SELECT statement");
        ast_node_free(select_node);
        ast_node_free(fields_node);
        return NULL;
    }
    token_stream_next(ts); // consume FROM

    // Парсим имя таблицы после FROM
    Token *table_tok = token_stream_next(ts);
    if (!table_tok || table_tok->type != TOKEN_IDENTIFIER) {
        report_error("Expected table name after FROM");
        ast_node_free(select_node);
        ast_node_free(fields_node);
        return NULL;
    }

    ASTNode *table_node = ast_node_create(AST_TABLE);
    if (!table_node) {
        report_error("Failed to allocate AST node for table");
        ast_node_free(select_node);
        ast_node_free(fields_node);
        return NULL;
    }
    table_node->string_value = strdup(table_tok->text);
    if (!table_node->string_value) {
        report_error("Failed to allocate memory for table name");
        ast_node_free(select_node);
        ast_node_free(fields_node);
        ast_node_free(table_node);
        return NULL;
    }

    ast_node_add_child(select_node, fields_node);
    ast_node_add_child(select_node, table_node);

    return select_node;
}
```

---

### Объяснение:

* Парсит базовый SELECT с перечислением полей и таблицы.
* Строит AST с корнем `AST_SELECT`, содержащим узлы полей и таблицы.
* Проверяет корректность синтаксиса (идентификаторы, ключевые слова).

---

Готов продолжать с следующим файлом!

