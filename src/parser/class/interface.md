Следующий файл — `parser/class/interface.c`.

---

### Задача `interface.c` для class:

Парсинг объявления интерфейса ABAP — `INTERFACE <name>` и его структуры.

---

## Реализация `interface.c`

```c
#include "../../include/parser.h"
#include "../../include/token.h"
#include "../../include/ast.h"
#include "../../include/error.h"
#include <stdlib.h>
#include <string.h>

/**
 * parse_interface_definition - Парсит объявление интерфейса.
 *
 * Синтаксис:
 * INTERFACE <interface_name>.
 *   METHODS ...
 *   DATA ...
 * ENDINTERFACE.
 *
 * Возвращает AST узел интерфейса или NULL при ошибке.
 */
ASTNode *parse_interface_definition(TokenStream *ts) {
    if (!ts) {
        report_error("TokenStream is NULL in parse_interface_definition");
        return NULL;
    }

    Token *token = token_stream_next(ts);
    if (!token || token->type != TOKEN_KEYWORD_INTERFACE) {
        report_error("Expected 'INTERFACE' keyword");
        return NULL;
    }

    token = token_stream_next(ts);
    if (!token || token->type != TOKEN_IDENTIFIER) {
        report_error("Expected interface name after 'INTERFACE'");
        return NULL;
    }

    ASTNode *interface_node = ast_node_create(AST_INTERFACE_DEF);
    if (!interface_node) {
        report_error("Failed to allocate AST node for INTERFACE");
        return NULL;
    }

    interface_node->interface_name = strdup(token->text);
    interface_node->interface_members = ast_node_list_create();

    if (!interface_node->interface_members) {
        report_error("Failed to create member list for interface");
        ast_node_free(interface_node);
        return NULL;
    }

    // Парсинг содержимого интерфейса
    while ((token = token_stream_peek(ts)) != NULL) {
        if (token->type == TOKEN_KEYWORD_ENDINTERFACE) {
            token_stream_next(ts);  // consume ENDINTERFACE
            break;
        }

        if (token->type == TOKEN_KEYWORD_METHODS) {
            ASTNode *method_decl = parse_method_declaration(ts);
            if (!method_decl) {
                report_error("Failed to parse METHOD declaration in INTERFACE");
                ast_node_free(interface_node);
                return NULL;
            }
            ast_node_list_append(interface_node->interface_members, method_decl);
        } else if (token->type == TOKEN_KEYWORD_DATA) {
            ASTNode *data_decl = parse_data_declaration(ts);
            if (!data_decl) {
                report_error("Failed to parse DATA declaration in INTERFACE");
                ast_node_free(interface_node);
                return NULL;
            }
            ast_node_list_append(interface_node->interface_members, data_decl);
        } else {
            report_error_token("Unexpected token in INTERFACE block", token);
            ast_node_free(interface_node);
            return NULL;
        }
    }

    return interface_node;
}
```

---

### Объяснение:

* Обрабатывает конструкцию `INTERFACE <name> ... ENDINTERFACE`.
* Поддерживает парсинг методов (`METHODS`) и переменных (`DATA`) внутри интерфейса.
* Возвращает AST узел `AST_INTERFACE_DEF` с вложенными членами интерфейса.

---

Готов к следующему файлу!
