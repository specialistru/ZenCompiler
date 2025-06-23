Следующий файл — `parser/try/nested.c`.

---

### Задача `nested.c` (try):

Парсинг вложенных блоков TRY...CATCH...ENDTRY, когда TRY блоки могут быть вложены друг в друга.

---

## Реализация `nested.c`

```c
#include "../../include/parser.h"
#include "../../include/token.h"
#include "../../include/ast.h"
#include "../../include/error.h"
#include <stdlib.h>
#include <string.h>

/**
 * parse_nested_try - Парсит вложенный блок TRY...CATCH...ENDTRY.
 *
 * Поддерживает рекурсивный разбор вложенных TRY-блоков.
 * Возвращает AST узел TRY с вложенными блоками или NULL при ошибке.
 */

ASTNode *parse_nested_try(TokenStream *ts) {
    if (!ts) {
        report_error("TokenStream is NULL in parse_nested_try");
        return NULL;
    }

    // Ожидается, что ключевое слово TRY уже прочитано
    ASTNode *try_node = ast_node_create(AST_TRY);
    if (!try_node) {
        report_error("Failed to allocate AST node for TRY");
        return NULL;
    }

    while (1) {
        Token *next_token = token_stream_peek(ts);
        if (!next_token) {
            report_error("Unexpected end of tokens inside TRY block");
            ast_node_free(try_node);
            return NULL;
        }

        if (next_token->type == TOKEN_CATCH || next_token->type == TOKEN_CLEANUP || next_token->type == TOKEN_ENDTRY) {
            break;
        }

        if (next_token->type == TOKEN_TRY) {
            token_stream_next(ts); // consume TRY
            ASTNode *nested_try = parse_nested_try(ts);
            if (!nested_try) {
                ast_node_free(try_node);
                return NULL;
            }
            ast_node_add_child(try_node, nested_try);
            continue;
        }

        ASTNode *stmt = parse_statement(ts);
        if (!stmt) {
            report_error("Failed to parse statement inside TRY block");
            ast_node_free(try_node);
            return NULL;
        }

        ast_node_add_child(try_node, stmt);
    }

    // Обработка блоков CATCH и CLEANUP
    while (1) {
        Token *peek = token_stream_peek(ts);
        if (!peek) {
            report_error("Unexpected end of tokens inside TRY");
            ast_node_free(try_node);
            return NULL;
        }

        if (peek->type == TOKEN_CATCH) {
            token_stream_next(ts); // consume CATCH
            ASTNode *catch_node = parse_catch_block(ts);
            if (!catch_node) {
                ast_node_free(try_node);
                return NULL;
            }
            ast_node_add_child(try_node, catch_node);
            continue;
        }
        else if (peek->type == TOKEN_CLEANUP) {
            token_stream_next(ts); // consume CLEANUP
            ASTNode *cleanup_node = parse_cleanup_block(ts);
            if (!cleanup_node) {
                ast_node_free(try_node);
                return NULL;
            }
            ast_node_add_child(try_node, cleanup_node);
            continue;
        }
        else if (peek->type == TOKEN_ENDTRY) {
            token_stream_next(ts); // consume ENDTRY
            Token *dot = token_stream_next(ts);
            if (!dot || dot->type != TOKEN_DOT) {
                report_error("Expected '.' after ENDTRY");
                ast_node_free(try_node);
                return NULL;
            }
            break;
        }
        else {
            report_error("Unexpected token inside TRY block");
            ast_node_free(try_node);
            return NULL;
        }
    }

    return try_node;
}
```

---

### Объяснение:

* Парсит тело TRY, рекурсивно обрабатывая вложенные TRY.
* Обрабатывает блоки CATCH и CLEANUP.
* Завершается на ENDTRY с точкой.
* Возвращает AST с полным вложенным содержимым.

---

Готов продолжать, если надо!
