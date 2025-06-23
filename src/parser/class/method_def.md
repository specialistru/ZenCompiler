Следующий файл — `parser/class/method_def.c`.

---

### Задача `method_def.c` для class:

Парсинг определения метода внутри класса или интерфейса — `METHODS <name> [IMPORTING ... EXPORTING ... CHANGING ... RETURNING ...]`.

---

## Реализация `method_def.c`

```c
#include "../../include/parser.h"
#include "../../include/token.h"
#include "../../include/ast.h"
#include "../../include/error.h"
#include <stdlib.h>
#include <string.h>

/**
 * parse_method_declaration - Парсит объявление метода.
 *
 * Синтаксис:
 * METHODS <method_name>
 *   [IMPORTING param1 TYPE type1 ...]
 *   [EXPORTING ...]
 *   [CHANGING ...]
 *   [RETURNING VALUE(rv) TYPE type]
 *   [RAISING ...].
 *
 * Возвращает AST узел метода.
 */
ASTNode *parse_method_declaration(TokenStream *ts) {
    if (!ts) {
        report_error("TokenStream is NULL in parse_method_declaration");
        return NULL;
    }

    Token *token = token_stream_next(ts);
    if (!token || token->type != TOKEN_KEYWORD_METHODS) {
        report_error("Expected 'METHODS' keyword");
        return NULL;
    }

    token = token_stream_next(ts);
    if (!token || token->type != TOKEN_IDENTIFIER) {
        report_error("Expected method name after 'METHODS'");
        return NULL;
    }

    ASTNode *method_node = ast_node_create(AST_METHOD_DECL);
    if (!method_node) {
        report_error("Failed to create AST node for method declaration");
        return NULL;
    }

    method_node->method_name = strdup(token->text);

    while ((token = token_stream_peek(ts)) != NULL) {
        if (token->type == TOKEN_KEYWORD_IMPORTING) {
            token_stream_next(ts);
            method_node->importing_params = parse_parameter_list(ts);
        } else if (token->type == TOKEN_KEYWORD_EXPORTING) {
            token_stream_next(ts);
            method_node->exporting_params = parse_parameter_list(ts);
        } else if (token->type == TOKEN_KEYWORD_CHANGING) {
            token_stream_next(ts);
            method_node->changing_params = parse_parameter_list(ts);
        } else if (token->type == TOKEN_KEYWORD_RETURNING) {
            token_stream_next(ts);
            Token *value_tok = token_stream_next(ts);
            if (!value_tok || value_tok->type != TOKEN_KEYWORD_VALUE) {
                report_error("Expected VALUE after RETURNING");
                ast_node_free(method_node);
                return NULL;
            }

            token = token_stream_next(ts);
            if (!token || token->type != TOKEN_LPAREN) {
                report_error("Expected '(' after VALUE in RETURNING");
                ast_node_free(method_node);
                return NULL;
            }

            token = token_stream_next(ts);
            if (!token || token->type != TOKEN_IDENTIFIER) {
                report_error("Expected return variable name");
                ast_node_free(method_node);
                return NULL;
            }

            char *return_var = strdup(token->text);

            token = token_stream_next(ts);
            if (!token || token->type != TOKEN_RPAREN) {
                report_error("Expected ')' after return variable");
                free(return_var);
                ast_node_free(method_node);
                return NULL;
            }

            token = token_stream_next(ts);
            if (!token || token->type != TOKEN_KEYWORD_TYPE) {
                report_error("Expected TYPE after RETURNING VALUE(...)");
                free(return_var);
                ast_node_free(method_node);
                return NULL;
            }

            token = token_stream_next(ts);
            if (!token || token->type != TOKEN_IDENTIFIER) {
                report_error("Expected type after TYPE in RETURNING");
                free(return_var);
                ast_node_free(method_node);
                return NULL;
            }

            ASTNode *ret = ast_node_create(AST_METHOD_RETURN);
            if (!ret) {
                report_error("Failed to create return node");
                free(return_var);
                ast_node_free(method_node);
                return NULL;
            }

            ret->return_variable = return_var;
            ret->return_type = strdup(token->text);
            method_node->returning_param = ret;
        } else if (token->type == TOKEN_KEYWORD_RAISING) {
            token_stream_next(ts);
            method_node->raising_exceptions = parse_identifier_list(ts);
        } else {
            break;  // завершили парсинг объявления
        }
    }

    return method_node;
}
```

---

### Объяснение:

* Обрабатывает объявления методов ABAP в `CLASS` и `INTERFACE`.
* Поддерживает параметры `IMPORTING`, `EXPORTING`, `CHANGING`, `RETURNING`, `RAISING`.
* Возвращает AST узел с полями, отражающими параметры метода и тип возвращаемого значения.

---

Готов к следующему файлу!
