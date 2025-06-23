Следующий файл — `parser/select/into_table.c`.

---

### Задача `into_table.c` (select):

Обработка конструкции SELECT ... INTO TABLE в ABAP, где результат выборки сохраняется во внутреннюю таблицу.

---

## Реализация `into_table.c`

```c
#include "../../include/parser.h"
#include "../../include/token.h"
#include "../../include/ast.h"
#include "../../include/error.h"
#include <stdlib.h>
#include <string.h>

/**
 * parse_select_into_table - Парсит конструкцию SELECT ... INTO TABLE ... FROM ...
 *
 * Ожидается, что ключевое слово SELECT уже было прочитано.
 * Возвращает AST узел SELECT с INTO TABLE или NULL при ошибке.
 */
ASTNode *parse_select_into_table(TokenStream *ts) {
    if (!ts) {
        report_error("TokenStream is NULL in parse_select_into_table");
        return NULL;
    }

    ASTNode *select_node = ast_node_create(AST_SELECT);
    if (!select_node) {
        report_error("Failed to allocate AST node for SELECT");
        return NULL;
    }

    // Парсим список полей до INTO
    ASTNode *fields_node = ast_node_create(AST_FIELD_LIST);
    if (!fields_node) {
        report_error("Failed to allocate AST node for field list");
        ast_node_free(select_node);
        return NULL;
    }

    Token *tok = NULL;
    while ((tok = token_stream_peek(ts)) != NULL) {
        if (tok->type == TOKEN_INTO) {
            break;
        }

        tok = token_stream_next(ts);
        if (tok->type == TOKEN_COMMA) {
            continue;
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

    if (!tok || tok->type != TOKEN_INTO) {
        report_error("Expected INTO keyword in SELECT statement");
        ast_node_free(select_node);
        ast_node_free(fields_node);
        return NULL;
    }
    token_stream_next(ts); // consume INTO

    // Ожидается TABLE после INTO
    tok = token_stream_next(ts);
    if (!tok || tok->type != TOKEN_TABLE) {
        report_error("Expected TABLE keyword after INTO");
        ast_node_free(select_node);
        ast_node_free(fields_node);
        return NULL;
    }

    // Парсим имя внутренней таблицы
    Token *table_var_tok = token_stream_next(ts);
    if (!table_var_tok || table_var_tok->type != TOKEN_IDENTIFIER) {
        report_error("Expected internal table variable name after INTO TABLE");
        ast_node_free(select_node);
        ast_node_free(fields_node);
        return NULL;
    }

    ASTNode *into_table_node = ast_node_create(AST_INTO_TABLE);
    if (!into_table_node) {
        report_error("Failed to allocate AST node for INTO TABLE");
        ast_node_free(select_node);
        ast_node_free(fields_node);
        return NULL;
    }
    into_table_node->string_value = strdup(table_var_tok->text);
    if (!into_table_node->string_value) {
        report_error("Failed to allocate memory for internal table variable name");
        ast_node_free(select_node);
        ast_node_free(fields_node);
        ast_node_free(into_table_node);
        return NULL;
    }

    // Ожидается ключевое слово FROM
    tok = token_stream_next(ts);
    if (!tok || tok->type != TOKEN_FROM) {
        report_error("Expected FROM keyword in SELECT statement");
        ast_node_free(select_node);
        ast_node_free(fields_node);
        ast_node_free(into_table_node);
        return NULL;
    }

    // Парсим имя таблицы
    Token *db_table_tok = token_stream_next(ts);
    if (!db_table_tok || db_table_tok->type != TOKEN_IDENTIFIER) {
        report_error("Expected database table name after FROM");
        ast_node_free(select_node);
        ast_node_free(fields_node);
        ast_node_free(into_table_node);
        return NULL;
    }

    ASTNode *db_table_node = ast_node_create(AST_TABLE);
    if (!db_table_node) {
        report_error("Failed to allocate AST node for table");
        ast_node_free(select_node);
        ast_node_free(fields_node);
        ast_node_free(into_table_node);
        return NULL;
    }
    db_table_node->string_value = strdup(db_table_tok->text);
    if (!db_table_node->string_value) {
        report_error("Failed to allocate memory for table name");
        ast_node_free(select_node);
        ast_node_free(fields_node);
        ast_node_free(into_table_node);
        ast_node_free(db_table_node);
        return NULL;
    }

    ast_node_add_child(select_node, fields_node);
    ast_node_add_child(select_node, into_table_node);
    ast_node_add_child(select_node, db_table_node);

    // Дополнительно можно добавить обработку WHERE и других условий

    return select_node;
}
```

---

### Объяснение:

* Парсит SELECT с INTO TABLE конструкцией.
* Разбирает список полей, внутреннюю таблицу и исходную базу данных.
* Возвращает AST узел с детьми для полей, внутренней таблицы и таблицы данных.

---

Если хотите, могу перейти к следующему файлу!
