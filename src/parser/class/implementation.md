Следующий файл — `parser/class/implementation.c`.

---

### Задача `implementation.c` для class:

Обработка основной реализации класса: парсинг секций класса, методов, атрибутов, интерфейсов и пр.

---

## Реализация `implementation.c`

```c
#include "../../include/parser.h"
#include "../../include/token.h"
#include "../../include/ast.h"
#include "../../include/error.h"
#include <stdlib.h>
#include <string.h>

/**
 * parse_class_implementation - Парсит реализацию класса ABAP.
 *
 * Обрабатывает секции класса: ATTRIBUTES, METHODS, INTERFACES, и прочее.
 * Поддерживает разделение на секции видимости (PUBLIC, PRIVATE, PROTECTED).
 * Возвращает AST узел класса с дочерними элементами.
 */
ASTNode *parse_class_implementation(TokenStream *ts) {
    if (!ts) {
        report_error("TokenStream is NULL in parse_class_implementation");
        return NULL;
    }

    ASTNode *class_node = ast_node_create(AST_CLASS_IMPL);
    if (!class_node) {
        report_error("Failed to allocate AST node for class implementation");
        return NULL;
    }

    VisibilityType current_visibility = VISIBILITY_PRIVATE; // По умолчанию PRIVATE

    Token *token;
    while ((token = token_stream_peek(ts)) != NULL) {
        if (token->type == TOKEN_KEYWORD_ENDCLASS) {
            token_stream_next(ts); // consume ENDCLASS
            token = token_stream_next(ts);
            if (!token || token->type != TOKEN_DOT) {
                report_error("Expected '.' after ENDCLASS");
                ast_node_free(class_node);
                return NULL;
            }
            break; // конец класса
        }

        if (token->type == TOKEN_KEYWORD_PUBLIC ||
            token->type == TOKEN_KEYWORD_PROTECTED ||
            token->type == TOKEN_KEYWORD_PRIVATE) {
            current_visibility = parse_visibility(ts);
            if (current_visibility == VISIBILITY_INVALID) {
                ast_node_free(class_node);
                return NULL;
            }
            continue;
        }

        if (token->type == TOKEN_KEYWORD_ATTRIBUTES) {
            token_stream_next(ts); // consume ATTRIBUTES
            if (!parse_attributes(ts, class_node, current_visibility)) {
                report_error("Failed to parse ATTRIBUTES section");
                ast_node_free(class_node);
                return NULL;
            }
            continue;
        }

        if (token->type == TOKEN_KEYWORD_METHODS) {
            // Метод объявлен, возможно определен позже
            ASTNode *method_def = parse_method_definition(ts);
            if (!method_def) {
                ast_node_free(class_node);
                return NULL;
            }
            method_def->visibility = current_visibility;
            ast_node_list_append(class_node->methods, method_def);
            continue;
        }

        if (token->type == TOKEN_KEYWORD_INTERFACES) {
            token_stream_next(ts); // consume INTERFACES
            if (!parse_interfaces(ts, class_node)) {
                report_error("Failed to parse INTERFACES section");
                ast_node_free(class_node);
                return NULL;
            }
            continue;
        }

        // Пропускаем неизвестные или не поддерживаемые конструкции
        token_stream_next(ts);
    }

    return class_node;
}

/**
 * parse_attributes - Парсит секцию ATTRIBUTES класса.
 *
 * Добавляет атрибуты с текущей видимостью в AST узел класса.
 */
static int parse_attributes(TokenStream *ts, ASTNode *class_node, VisibilityType visibility) {
    Token *token;
    while ((token = token_stream_peek(ts)) != NULL) {
        if (token->type == TOKEN_KEYWORD_PUBLIC ||
            token->type == TOKEN_KEYWORD_PROTECTED ||
            token->type == TOKEN_KEYWORD_PRIVATE ||
            token->type == TOKEN_KEYWORD_METHODS ||
            token->type == TOKEN_KEYWORD_INTERFACES ||
            token->type == TOKEN_KEYWORD_ENDCLASS) {
            break; // Конец секции ATTRIBUTES
        }

        token = token_stream_next(ts);
        if (token->type != TOKEN_IDENTIFIER) {
            report_error("Expected attribute identifier");
            return 0;
        }

        ASTNode *attr_node = ast_node_create(AST_CLASS_ATTRIBUTE);
        if (!attr_node) {
            report_error("Failed to allocate attribute AST node");
            return 0;
        }

        attr_node->attribute_name = strdup(token->text);
        attr_node->visibility = visibility;

        // Опционально, парсим тип и начальное значение (если есть)
        Token *next = token_stream_peek(ts);
        if (next && next->type == TOKEN_KEYWORD_TYPE) {
            token_stream_next(ts); // consume TYPE
            Token *type_token = token_stream_next(ts);
            if (!type_token || type_token->type != TOKEN_IDENTIFIER) {
                report_error("Expected type identifier after TYPE");
                ast_node_free(attr_node);
                return 0;
            }
            attr_node->attribute_type = strdup(type_token->text);
        }

        // TODO: парсинг начального значения и других спецификаторов

        ast_node_list_append(class_node->attributes, attr_node);

        // Ожидаем точку после определения атрибута
        Token *dot_token = token_stream_next(ts);
        if (!dot_token || dot_token->type != TOKEN_DOT) {
            report_error("Expected '.' after attribute declaration");
            return 0;
        }
    }
    return 1;
}

/**
 * parse_interfaces - Парсит секцию INTERFACES класса.
 */
static int parse_interfaces(TokenStream *ts, ASTNode *class_node) {
    while (1) {
        Token *token = token_stream_next(ts);
        if (!token || token->type != TOKEN_IDENTIFIER) {
            report_error("Expected interface name in INTERFACES section");
            return 0;
        }

        ASTNode *iface_node = ast_node_create(AST_CLASS_INTERFACE);
        if (!iface_node) {
            report_error("Failed to allocate interface AST node");
            return 0;
        }

        iface_node->interface_name = strdup(token->text);
        ast_node_list_append(class_node->interfaces, iface_node);

        Token *next = token_stream_peek(ts);
        if (!next || next->type != TOKEN_COMMA) {
            break;
        }
        token_stream_next(ts); // consume comma
    }

    Token *dot_token = token_stream_next(ts);
    if (!dot_token || dot_token->type != TOKEN_DOT) {
        report_error("Expected '.' at end of INTERFACES section");
        return 0;
    }
    return 1;
}
```

---

### Объяснение:

* Функция парсит тело класса между `CLASS ...` и `ENDCLASS.`.
* Обрабатывает видимость, атрибуты, методы, интерфейсы.
* Для каждой секции выделены отдельные функции.
* Ошибки вызывают освобождение AST и возвращают NULL/0.

---

Если хочешь, могу перейти к следующему файлу.
