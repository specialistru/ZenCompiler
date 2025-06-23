Следующий файл — `parser/call_function/exceptions.c`.

---

### Задача `exceptions.c` для call\_function:

Парсинг блока обработки исключений в конструкции `CALL FUNCTION`.

---

## Реализация `exceptions.c`

```c
#include "../../include/parser.h"
#include "../../include/token.h"
#include "../../include/ast.h"
#include "../../include/error.h"
#include <stdlib.h>

/**
 * parse_call_function_exceptions - Парсит блок EXCEPTIONS в CALL FUNCTION.
 *
 * Синтаксис:
 * EXCEPTIONS
 *   exc1 = value1
 *   exc2 = value2
 *   ...
 *
 * Возвращает список AST узлов исключений или NULL при ошибке.
 */
ASTNodeList *parse_call_function_exceptions(TokenStream *ts) {
    if (!ts) {
        report_error("TokenStream is NULL in parse_call_function_exceptions");
        return NULL;
    }

    Token *token = token_stream_next(ts);
    if (!token || token->type != TOKEN_KEYWORD_EXCEPTIONS) {
        report_error("Expected EXCEPTIONS keyword");
        return NULL;
    }

    ASTNodeList *exceptions = ast_node_list_create();
    if (!exceptions) {
        report_error("Failed to allocate ASTNodeList for EXCEPTIONS");
        return NULL;
    }

    while (1) {
        token = token_stream_peek(ts);
        if (!token || token->type != TOKEN_IDENTIFIER)
            break;

        // exc_name
        token = token_stream_next(ts);
        char *exc_name = strdup(token->text);

        token = token_stream_next(ts);
        if (!token || token->type != TOKEN_OPERATOR_EQUALS) {
            report_error("Expected '=' after exception name");
            free(exc_name);
            ast_node_list_free(exceptions);
            return NULL;
        }

        // значение (обычно константа)
        token = token_stream_next(ts);
        if (!token || (token->type != TOKEN_NUMBER && token->type != TOKEN_IDENTIFIER)) {
            report_error("Expected constant or identifier for exception value");
            free(exc_name);
            ast_node_list_free(exceptions);
            return NULL;
        }

        ASTNode *exc_node = ast_node_create(AST_EXCEPTION_MAPPING);
        if (!exc_node) {
            report_error("Failed to create AST node for exception mapping");
            free(exc_name);
            ast_node_list_free(exceptions);
            return NULL;
        }

        exc_node->exception_name = exc_name;
        exc_node->exception_value = strdup(token->text);
        ast_node_list_append(exceptions, exc_node);
    }

    return exceptions;
}
```

---

### Объяснение:

* Парсит блок `EXCEPTIONS`, в котором перечислены пары `имя = значение`.
* Создаёт список AST узлов `AST_EXCEPTION_MAPPING`.
* Обрабатывает ошибки синтаксиса и выделения памяти.
* Возвращает `ASTNodeList`, которую можно прикрепить к узлу `CALL FUNCTION`.

---

Готов к следующему файлу!
