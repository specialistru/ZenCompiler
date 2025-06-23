Следующий файл — `parser/special/set_get_parameter.c`.

---

### Задача `set_get_parameter.c` (special):

Обработка операторов `SET PARAMETER` и `GET PARAMETER` в ABAP, которые устанавливают и получают параметры, обычно для сохранения состояния между сессиями.

---

## Реализация `set_get_parameter.c`

```c
#include "../../include/parser.h"
#include "../../include/token.h"
#include "../../include/ast.h"
#include "../../include/error.h"

/**
 * parse_special_set_get_parameter - Парсит операторы SET PARAMETER и GET PARAMETER.
 *
 * SET PARAMETER ID <id> FIELD <field> - установка параметра.
 * GET PARAMETER ID <id> FIELD <field> - получение параметра.
 *
 * Возвращает AST узел SET_PARAMETER или GET_PARAMETER с соответствующими дочерними узлами.
 */
ASTNode *parse_special_set_get_parameter(TokenStream *ts) {
    if (!ts) {
        report_error("TokenStream is NULL in parse_special_set_get_parameter");
        return NULL;
    }

    Token *tok = token_stream_peek(ts);
    if (!tok) {
        report_error("Unexpected end of tokens in set_get_parameter");
        return NULL;
    }

    ASTNode *node = NULL;

    if (tok->type == TOKEN_SET) {
        token_stream_next(ts); // consume SET
        Token *param_tok = token_stream_next(ts);
        if (!param_tok || param_tok->type != TOKEN_PARAMETER) {
            report_error("Expected PARAMETER after SET");
            return NULL;
        }

        node = ast_node_create(AST_SPECIAL_SET_PARAMETER);
        if (!node) {
            report_error("Failed to create AST node for SET PARAMETER");
            return NULL;
        }

    } else if (tok->type == TOKEN_GET) {
        token_stream_next(ts); // consume GET
        Token *param_tok = token_stream_next(ts);
        if (!param_tok || param_tok->type != TOKEN_PARAMETER) {
            report_error("Expected PARAMETER after GET");
            return NULL;
        }

        node = ast_node_create(AST_SPECIAL_GET_PARAMETER);
        if (!node) {
            report_error("Failed to create AST node for GET PARAMETER");
            return NULL;
        }

    } else {
        report_error("Expected SET or GET token");
        return NULL;
    }

    // Далее ожидаем ID <id>
    Token *id_tok = token_stream_next(ts);
    if (!id_tok || id_tok->type != TOKEN_ID) {
        report_error("Expected ID after SET/GET PARAMETER");
        ast_node_free(node);
        return NULL;
    }

    Token *param_id_tok = token_stream_next(ts);
    if (!param_id_tok || param_id_tok->type != TOKEN_IDENTIFIER) {
        report_error("Expected identifier after ID");
        ast_node_free(node);
        return NULL;
    }

    ASTNode *id_node = ast_node_create(AST_IDENTIFIER);
    id_node->string_value = strdup(param_id_tok->text);
    ast_node_add_child(node, id_node);

    // Далее ожидаем FIELD <field>
    Token *field_tok = token_stream_next(ts);
    if (!field_tok || field_tok->type != TOKEN_FIELD) {
        report_error("Expected FIELD token");
        ast_node_free(node);
        return NULL;
    }

    Token *field_name_tok = token_stream_next(ts);
    if (!field_name_tok || field_name_tok->type != TOKEN_IDENTIFIER) {
        report_error("Expected identifier after FIELD");
        ast_node_free(node);
        return NULL;
    }

    ASTNode *field_node = ast_node_create(AST_IDENTIFIER);
    field_node->string_value = strdup(field_name_tok->text);
    ast_node_add_child(node, field_node);

    return node;
}
```

---

### Пример ABAP:

```abap
SET PARAMETER ID 'ABC' FIELD lv_value.
GET PARAMETER ID 'ABC' FIELD lv_value.
```

---

Если хочешь следующий файл — скажи `следующий файл`.
