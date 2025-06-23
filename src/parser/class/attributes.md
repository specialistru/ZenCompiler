Следующий файл — `parser/class/attributes.c`.

---

### Задача `attributes.c` для class:

Парсинг секции объявления атрибутов класса — переменных, констант и полей внутри блока DEFINITION класса.

---

## Реализация `attributes.c`

```c
#include "../../include/parser.h"
#include "../../include/token.h"
#include "../../include/ast.h"
#include "../../include/error.h"
#include <stdlib.h>
#include <string.h>

/**
 * parse_class_attributes - Парсит атрибуты класса внутри DEFINITION.
 *
 * Синтаксис:
 *   PUBLIC SECTION.
 *     DATA var1 TYPE i.
 *     CONSTANTS const1 TYPE string VALUE 'value'.
 *   PRIVATE SECTION.
 *     ...
 *
 * Возвращает список AST узлов атрибутов.
 */
ASTNodeList *parse_class_attributes(TokenStream *ts) {
    if (!ts) {
        report_error("TokenStream is NULL in parse_class_attributes");
        return NULL;
    }

    ASTNodeList *attributes = ast_node_list_create();
    if (!attributes) {
        report_error("Failed to allocate ASTNodeList for class attributes");
        return NULL;
    }

    while (1) {
        Token *token = token_stream_peek(ts);
        if (!token) break;

        if (token->type == TOKEN_KEYWORD_PUBLIC ||
            token->type == TOKEN_KEYWORD_PRIVATE ||
            token->type == TOKEN_KEYWORD_PROTECTED) {
            // Секция видимости — можно сохранить как отдельный узел или флаг
            token = token_stream_next(ts);  // съедаем PUBLIC/PRIVATE/...
            Token *section_token = token_stream_next(ts);  // ожидаем SECTION
            if (!section_token || section_token->type != TOKEN_KEYWORD_SECTION) {
                report_error("Expected 'SECTION' after visibility keyword");
                ast_node_list_free(attributes);
                return NULL;
            }
            continue; // пока пропускаем, можно сохранить в будущем как видимость
        }

        if (token->type == TOKEN_KEYWORD_DATA) {
            ASTNode *data_decl = parse_data_declaration(ts);
            if (!data_decl) {
                report_error("Failed to parse DATA declaration");
                ast_node_list_free(attributes);
                return NULL;
            }
            ast_node_list_append(attributes, data_decl);
        } else if (token->type == TOKEN_KEYWORD_CONSTANTS) {
            ASTNode *const_decl = parse_constant_declaration(ts);
            if (!const_decl) {
                report_error("Failed to parse CONSTANTS declaration");
                ast_node_list_free(attributes);
                return NULL;
            }
            ast_node_list_append(attributes, const_decl);
        } else {
            // завершение блока атрибутов
            break;
        }
    }

    return attributes;
}
```

---

### Объяснение:

* Поддерживает парсинг видимых атрибутов класса: `DATA`, `CONSTANTS`.
* Распознаёт области `PUBLIC SECTION`, `PRIVATE SECTION`, `PROTECTED SECTION`.
* Возвращает `ASTNodeList` с атрибутами, которые можно вложить в узел класса.

---

Готов к следующему файлу!
