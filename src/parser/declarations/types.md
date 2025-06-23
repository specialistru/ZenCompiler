Следующий файл — `parser/declarations/types.c`.

---

### Задача `types.c` (declarations):

Парсинг инструкции `TYPES` в ABAP — определение пользовательских типов данных: скаляры, структуры, таблицы и пр.

---

## Реализация `types.c`

```c
#include "../../include/parser.h"
#include "../../include/token.h"
#include "../../include/ast.h"
#include "../../include/error.h"
#include <stdlib.h>
#include <string.h>

/**
 * parse_declaration_types - Парсит инструкцию TYPES.
 *
 * Поддерживает:
 * - TYPES name TYPE i.
 * - TYPES name TYPE string LENGTH 20.
 * - TYPES name TYPE p DECIMALS 2.
 * - TYPES BEGIN OF struct ... END OF struct.
 *
 * Возвращает AST узел типа или NULL при ошибке.
 */
ASTNode *parse_declaration_types(TokenStream *ts) {
    if (!ts) {
        report_error("TokenStream is NULL in parse_declaration_types");
        return NULL;
    }

    ASTNode *types_node = ast_node_create(AST_TYPES_DECL);
    if (!types_node) {
        report_error("Failed to allocate AST node for TYPES declaration");
        return NULL;
    }

    Token *next = token_stream_peek(ts);
    if (!next) {
        report_error("Unexpected end of input in TYPES");
        ast_node_free(types_node);
        return NULL;
    }

    // Проверяем, начинается ли структура
    if (next->type == TOKEN_BEGIN) {
        // Парсим структуру: TYPES BEGIN OF <name> ... END OF <name>.
        token_stream_next(ts); // consume BEGIN
        Token *of_tok = token_stream_next(ts);
        if (!of_tok || of_tok->type != TOKEN_OF) {
            report_error("Expected 'OF' after 'BEGIN' in TYPES");
            ast_node_free(types_node);
            return NULL;
        }

        Token *struct_name = token_stream_next(ts);
        if (!struct_name || struct_name->type != TOKEN_IDENTIFIER) {
            report_error("Expected structure name after 'BEGIN OF' in TYPES");
            ast_node_free(types_node);
            return NULL;
        }

        types_node->string_value = strdup(struct_name->text);
        if (!types_node->string_value) {
            report_error("Failed to store structure name in TYPES");
            ast_node_free(types_node);
            return NULL;
        }

        // Парсим поля структуры
        while ((next = token_stream_peek(ts)) && next->type != TOKEN_END) {
            ASTNode *field_decl = parse_declaration_types(ts); // рекурсивный вызов
            if (!field_decl) {
                report_error("Invalid structure field in TYPES");
                ast_node_free(types_node);
                return NULL;
            }
            ast_node_add_child(types_node, field_decl);
        }

        // Закрытие структуры: END OF <name>.
        token_stream_next(ts); // consume END
        Token *of2 = token_stream_next(ts);
        if (!of2 || of2->type != TOKEN_OF) {
            report_error("Expected 'OF' after 'END' in TYPES structure");
            ast_node_free(types_node);
            return NULL;
        }

        Token *end_name = token_stream_next(ts);
        if (!end_name || strcmp(end_name->text, struct_name->text) != 0) {
            report_error("Structure name mismatch in END OF <name>");
            ast_node_free(types_node);
            return NULL;
        }

        return types_node;
    }

    // Обычное определение типа
    // TYPES var_name TYPE i [LENGTH x] [DECIMALS y].
    Token *type_name_tok = token_stream_next(ts);
    if (!type_name_tok || type_name_tok->type != TOKEN_IDENTIFIER) {
        report_error("Expected type name identifier in TYPES");
        ast_node_free(types_node);
        return NULL;
    }

    types_node->string_value = strdup(type_name_tok->text);
    if (!types_node->string_value) {
        report_error("Failed to store type name");
        ast_node_free(types_node);
        return NULL;
    }

    Token *type_kw = token_stream_next(ts);
    if (!type_kw || type_kw->type != TOKEN_TYPE) {
        report_error("Expected TYPE keyword after type name");
        ast_node_free(types_node);
        return NULL;
    }

    Token *base_type = token_stream_next(ts);
    if (!base_type || base_type->type != TOKEN_IDENTIFIER) {
        report_error("Expected base type name in TYPES");
        ast_node_free(types_node);
        return NULL;
    }

    ASTNode *base_node = ast_node_create(AST_TYPE_SPEC);
    base_node->string_value = strdup(base_type->text);
    ast_node_add_child(types_node, base_node);

    // Опционально LENGTH и DECIMALS
    Token *tok;
    while ((tok = token_stream_peek(ts)) &&
           (tok->type == TOKEN_LENGTH || tok->type == TOKEN_DECIMALS)) {
        Token *kw = token_stream_next(ts); // consume LENGTH or DECIMALS
        Token *val = token_stream_next(ts);

        if (!val || val->type != TOKEN_NUMBER) {
            report_error("Expected numeric value after LENGTH/DECIMALS");
            ast_node_free(types_node);
            return NULL;
        }

        ASTNode *spec = ast_node_create(
            kw->type == TOKEN_LENGTH ? AST_LENGTH_SPEC : AST_DECIMALS_SPEC);
        spec->string_value = strdup(val->text);
        ast_node_add_child(types_node, spec);
    }

    return types_node;
}
```

---

### Объяснение:

Поддерживает:

* Простые объявления:

  ```abap
  TYPES amount TYPE p LENGTH 8 DECIMALS 2.
  TYPES flag TYPE c.
  ```
* Структуры:

  ```abap
  TYPES: BEGIN OF employee,
           id TYPE i,
           name TYPE c LENGTH 30,
         END OF employee.
  ```

Используемые AST-узлы:

* `AST_TYPES_DECL` — главный узел.
* `AST_TYPE_SPEC` — тип.
* `AST_LENGTH_SPEC`, `AST_DECIMALS_SPEC` — уточнения.
* Рекурсивно собирает поля структуры как дочерние узлы.

---

Готов перейти к следующему файлу!
