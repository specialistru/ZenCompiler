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

/////////////////////////////////

Следующий файл — `parser/class/attributes.c`.

---

### Задача `attributes.c` для class:

Парсинг объявления атрибутов класса, включая модификаторы видимости и типы.

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
 * parse_class_attributes - Парсит секцию атрибутов класса.
 *
 * Синтаксис:
 * [PUBLIC|PRIVATE|PROTECTED] SECTION.
 *   DATA <attr_name> TYPE <type>.
 *   CONSTANTS <const_name> TYPE <type> VALUE <value>.
 *   ...
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
        report_error("Failed to create attribute list");
        return NULL;
    }

    while (1) {
        Token *token = token_stream_peek(ts);
        if (!token) break;

        // Проверяем на конец секции или класса
        if (token->type == TOKEN_KEYWORD_ENDCLASS ||
            token->type == TOKEN_KEYWORD_PUBLIC ||
            token->type == TOKEN_KEYWORD_PRIVATE ||
            token->type == TOKEN_KEYWORD_PROTECTED) {
            // Завершение секции
            break;
        }

        token = token_stream_next(ts);

        if (token->type == TOKEN_KEYWORD_DATA) {
            // Парсим DATA <name> TYPE <type>.
            Token *name_token = token_stream_next(ts);
            if (!name_token || name_token->type != TOKEN_IDENTIFIER) {
                report_error("Expected attribute name after DATA");
                ast_node_list_free(attributes);
                return NULL;
            }

            Token *type_token = token_stream_next(ts);
            if (!type_token || type_token->type != TOKEN_KEYWORD_TYPE) {
                report_error("Expected TYPE keyword after attribute name");
                ast_node_list_free(attributes);
                return NULL;
            }

            Token *type_name = token_stream_next(ts);
            if (!type_name || type_name->type != TOKEN_IDENTIFIER) {
                report_error("Expected type identifier after TYPE");
                ast_node_list_free(attributes);
                return NULL;
            }

            // Точка после определения атрибута
            Token *dot = token_stream_next(ts);
            if (!dot || dot->type != TOKEN_DOT) {
                report_error("Expected '.' after attribute declaration");
                ast_node_list_free(attributes);
                return NULL;
            }

            ASTNode *attr_node = ast_node_create(AST_CLASS_ATTRIBUTE);
            if (!attr_node) {
                report_error("Failed to allocate attribute node");
                ast_node_list_free(attributes);
                return NULL;
            }

            attr_node->attribute_name = strdup(name_token->text);
            attr_node->attribute_type = strdup(type_name->text);
            attr_node->visibility = VISIBILITY_DEFAULT; // Можно позже расширить

            ast_node_list_append(attributes, attr_node);

        } else if (token->type == TOKEN_KEYWORD_CONSTANTS) {
            // Парсинг CONSTANTS <name> TYPE <type> VALUE <value>.
            Token *const_name = token_stream_next(ts);
            if (!const_name || const_name->type != TOKEN_IDENTIFIER) {
                report_error("Expected constant name after CONSTANTS");
                ast_node_list_free(attributes);
                return NULL;
            }

            Token *type_token = token_stream_next(ts);
            if (!type_token || type_token->type != TOKEN_KEYWORD_TYPE) {
                report_error("Expected TYPE keyword after constant name");
                ast_node_list_free(attributes);
                return NULL;
            }

            Token *type_name = token_stream_next(ts);
            if (!type_name || type_name->type != TOKEN_IDENTIFIER) {
                report_error("Expected type identifier after TYPE");
                ast_node_list_free(attributes);
                return NULL;
            }

            Token *value_token = token_stream_next(ts);
            if (!value_token || value_token->type != TOKEN_KEYWORD_VALUE) {
                report_error("Expected VALUE keyword after TYPE in constant");
                ast_node_list_free(attributes);
                return NULL;
            }

            Token *value = token_stream_next(ts);
            if (!value || (value->type != TOKEN_LITERAL_STRING && value->type != TOKEN_LITERAL_NUMBER)) {
                report_error("Expected literal value after VALUE");
                ast_node_list_free(attributes);
                return NULL;
            }

            Token *dot = token_stream_next(ts);
            if (!dot || dot->type != TOKEN_DOT) {
                report_error("Expected '.' after constant declaration");
                ast_node_list_free(attributes);
                return NULL;
            }

            ASTNode *const_node = ast_node_create(AST_CLASS_CONSTANT);
            if (!const_node) {
                report_error("Failed to allocate constant node");
                ast_node_list_free(attributes);
                return NULL;
            }

            const_node->constant_name = strdup(const_name->text);
            const_node->constant_type = strdup(type_name->text);
            const_node->constant_value = strdup(value->text);
            const_node->visibility = VISIBILITY_DEFAULT; // Можно расширить

            ast_node_list_append(attributes, const_node);

        } else {
            report_error_token("Unexpected token in class attributes section", token);
            ast_node_list_free(attributes);
            return NULL;
        }
    }

    return attributes;
}
```

---

### Объяснение:

* Обрабатывает объявление атрибутов `DATA` и констант `CONSTANTS` внутри секций класса.
* Поддерживает базовую структуру с типами и видимостью (пока по умолчанию).
* Формирует список AST узлов с атрибутами класса.

---

Готов к следующему файлу!

