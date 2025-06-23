Следующий файл — `parser/try/catch.c`.

---

### Задача `catch.c` (try):

Обработка отдельного блока CATCH в конструкции TRY...CATCH...ENDTRY, в случае если его нужно парсить отдельно от основного блока TRY.

---

## Реализация `catch.c`

```c
#include "../../include/parser.h"
#include "../../include/token.h"
#include "../../include/ast.h"
#include "../../include/error.h"
#include <stdlib.h>
#include <string.h>

/**
 * parse_catch_block - Парсит блок CATCH с обработкой исключения.
 *
 * Ожидается, что ключевое слово CATCH уже было прочитано.
 * Возвращает AST узел CATCH или NULL при ошибке.
 */

ASTNode *parse_catch_block(TokenStream *ts) {
    if (!ts) {
        report_error("TokenStream is NULL in parse_catch_block");
        return NULL;
    }

    // Ожидается идентификатор исключения
    Token *ex_token = token_stream_next(ts);
    if (!ex_token || ex_token->type != TOKEN_IDENTIFIER) {
        report_error("Expected exception identifier after CATCH");
        return NULL;
    }

    ASTNode *catch_node = ast_node_create(AST_CATCH);
    if (!catch_node) {
        report_error("Failed to allocate AST node for CATCH");
        return NULL;
    }

    catch_node->string_value = strdup(ex_token->text);
    if (!catch_node->string_value) {
        report_error("Failed to allocate memory for exception identifier");
        ast_node_free(catch_node);
        return NULL;
    }

    // Парсим тело CATCH до следующего CATCH или ENDTRY
    while (1) {
        Token *peek = token_stream_peek(ts);
        if (!peek) {
            report_error("Unexpected end of tokens inside CATCH block");
            ast_node_free(catch_node);
            return NULL;
        }

        if (peek->type == TOKEN_CATCH || peek->type == TOKEN_ENDTRY) {
            break;
        }

        ASTNode *stmt = parse_statement(ts);
        if (!stmt) {
            report_error("Failed to parse statement inside CATCH block");
            ast_node_free(catch_node);
            return NULL;
        }

        ast_node_add_child(catch_node, stmt);
    }

    return catch_node;
}
```

---

### Объяснение:

* Предполагается, что CATCH уже считан.
* Считывается идентификатор исключения.
* Парсятся вложенные инструкции до следующего CATCH или ENDTRY.
* Возвращается узел AST\_CATCH с телом.

---

Если нужно — могу перейти к следующему файлу.
