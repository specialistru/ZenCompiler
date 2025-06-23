Следующий файл — `parser/call_function/simple.c`.

---

### Задача `simple.c` для call\_function:

Парсинг простого вызова функции без параметров или с простыми параметрами.

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
 * parse_call_function_simple - Парсит простой вызов функции.
 *
 * Синтаксис:
 * CALL FUNCTION <function_name> [EXPORTING <params>] [IMPORTING <params>] [CHANGING <params>].
 *
 * Возвращает AST узел вызова функции или NULL при ошибке.
 */
ASTNode *parse_call_function_simple(TokenStream *ts) {
    if (!ts) {
        report_error("TokenStream is NULL in parse_call_function_simple");
        return NULL;
    }

    Token *token = token_stream_next(ts);
    if (!token || token->type != TOKEN_KEYWORD_CALL) {
        report_error("Expected CALL keyword");
        return NULL;
    }

    token = token_stream_next(ts);
    if (!token || token->type != TOKEN_KEYWORD_FUNCTION) {
        report_error("Expected FUNCTION keyword after CALL");
        return NULL;
    }

    token = token_stream_next(ts);
    if (!token || token->type != TOKEN_IDENTIFIER) {
        report_error("Expected function name after CALL FUNCTION");
        return NULL;
    }

    ASTNode *call_node = ast_node_create(AST_CALL_FUNCTION);
    if (!call_node) {
        report_error("Failed to create AST node for CALL FUNCTION");
        return NULL;
    }

    call_node->function_name = strdup(token->text);
    if (!call_node->function_name) {
        report_error("Memory allocation failed for function_name");
        ast_node_free(call_node);
        return NULL;
    }

    // Обработка опциональных параметров (EXPORTING, IMPORTING, CHANGING)
    while (1) {
        token = token_stream_peek(ts);
        if (!token) break;

        if (token->type == TOKEN_KEYWORD_EXPORTING) {
            token_stream_next(ts); // съесть EXPORTING
            ASTNodeList *exporting_params = parse_parameter_list(ts);
            if (!exporting_params) {
                report_error("Failed to parse EXPORTING parameters");
                ast_node_free(call_node);
                return NULL;
            }
            call_node->exporting_params = exporting_params;
        } else if (token->type == TOKEN_KEYWORD_IMPORTING) {
            token_stream_next(ts); // съесть IMPORTING
            ASTNodeList *importing_params = parse_parameter_list(ts);
            if (!importing_params) {
                report_error("Failed to parse IMPORTING parameters");
                ast_node_free(call_node);
                return NULL;
            }
            call_node->importing_params = importing_params;
        } else if (token->type == TOKEN_KEYWORD_CHANGING) {
            token_stream_next(ts); // съесть CHANGING
            ASTNodeList *changing_params = parse_parameter_list(ts);
            if (!changing_params) {
                report_error("Failed to parse CHANGING parameters");
                ast_node_free(call_node);
                return NULL;
            }
            call_node->changing_params = changing_params;
        } else {
            break;
        }
    }

    return call_node;
}
```

---

### Объяснение:

* Парсит ключевые слова CALL FUNCTION.
* Сохраняет имя функции.
* Обрабатывает опциональные списки параметров EXPORTING, IMPORTING, CHANGING.
* Возвращает AST узел вызова функции или NULL при ошибках.

---

Готов к следующему файлу!
