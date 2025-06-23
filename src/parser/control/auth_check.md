Следующий файл — `parser/control/auth_check.c`.

---

### Задача `auth_check.c` (control):

Обработка оператора `AUTHORITY-CHECK` в ABAP — проверка прав доступа пользователя.

---

## Реализация `auth_check.c`

```c
#include "../../include/parser.h"
#include "../../include/token.h"
#include "../../include/ast.h"
#include "../../include/error.h"

/**
 * parse_control_auth_check - Парсит оператор AUTHORITY-CHECK.
 *
 * Оператор AUTHORITY-CHECK проверяет наличие у пользователя необходимых прав доступа.
 * Синтаксис: AUTHORITY-CHECK OBJECT <obj> ID <id> FIELD <field>.
 *
 * Возвращает AST узел AUTHORITY_CHECK с параметрами проверки.
 */
ASTNode *parse_control_auth_check(TokenStream *ts) {
    if (!ts) {
        report_error("TokenStream is NULL in parse_control_auth_check");
        return NULL;
    }

    Token *tok = token_stream_next(ts);
    if (!tok || tok->type != TOKEN_AUTHORITY_CHECK) {
        report_error("Expected AUTHORITY-CHECK token");
        return NULL;
    }

    ASTNode *auth_node = ast_node_create(AST_CONTROL_AUTHORITY_CHECK);
    if (!auth_node) {
        report_error("Failed to create AST node for AUTHORITY-CHECK");
        return NULL;
    }

    // Пример простой парсинг параметров OBJECT, ID, FIELD
    // Подробнее можно расширить согласно грамматике ABAP

    while (true) {
        Token *param_tok = token_stream_peek(ts);
        if (!param_tok) break;

        if (param_tok->type == TOKEN_OBJECT) {
            token_stream_next(ts);
            Token *obj_val = token_stream_next(ts);
            if (!obj_val || obj_val->type != TOKEN_IDENTIFIER) {
                report_error("Expected identifier after OBJECT");
                ast_node_free(auth_node);
                return NULL;
            }
            ASTNode *obj_node = ast_node_create(AST_AUTH_CHECK_OBJECT);
            obj_node->string_value = strdup(obj_val->text);
            ast_node_add_child(auth_node, obj_node);
        }
        else if (param_tok->type == TOKEN_ID) {
            token_stream_next(ts);
            Token *id_val = token_stream_next(ts);
            if (!id_val || id_val->type != TOKEN_IDENTIFIER) {
                report_error("Expected identifier after ID");
                ast_node_free(auth_node);
                return NULL;
            }
            ASTNode *id_node = ast_node_create(AST_AUTH_CHECK_ID);
            id_node->string_value = strdup(id_val->text);
            ast_node_add_child(auth_node, id_node);
        }
        else if (param_tok->type == TOKEN_FIELD) {
            token_stream_next(ts);
            Token *field_val = token_stream_next(ts);
            if (!field_val || field_val->type != TOKEN_IDENTIFIER) {
                report_error("Expected identifier after FIELD");
                ast_node_free(auth_node);
                return NULL;
            }
            ASTNode *field_node = ast_node_create(AST_AUTH_CHECK_FIELD);
            field_node->string_value = strdup(field_val->text);
            ast_node_add_child(auth_node, field_node);
        }
        else {
            break;
        }
    }

    return auth_node;
}
```

---

### Пример ABAP:

```abap
AUTHORITY-CHECK OBJECT 'S_TCODE' ID 'ACTVT' FIELD '01'.
```

---

### Основные AST узлы:

* `AST_CONTROL_AUTHORITY_CHECK` — корневой узел AUTHORITY-CHECK;
* `AST_AUTH_CHECK_OBJECT`, `AST_AUTH_CHECK_ID`, `AST_AUTH_CHECK_FIELD` — параметры проверки.

---

Если хочешь следующий файл — скажи `следующий файл`.
