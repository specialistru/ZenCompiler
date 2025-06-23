Следующий файл — `parser/try/simple.c`.

---

### Задача `simple.c` (try):

Парсинг простых блоков обработки исключений TRY...CATCH...ENDTRY в ABAP с базовой структурой без вложенных блоков.

---

## Реализация `simple.c`

```c
#include "../../include/parser.h"
#include "../../include/token.h"
#include "../../include/ast.h"
#include "../../include/error.h"
#include <stdlib.h>

/**
 * parse_simple_try - Парсит простой блок TRY...CATCH...ENDTRY.
 *
 * Обрабатывает TRY-блок, содержащий операторы,
 * и CATCH-блоки с обработчиками исключений.
 *
 * Возвращает AST узел TRY или NULL при ошибке.
 */

ASTNode *parse_simple_try(TokenStream *ts) {
    if (!ts) {
        report_error("TokenStream is NULL in parse_simple_try");
        return NULL;
    }

    // Ожидается ключевое слово TRY уже прочитано
    ASTNode *try_node = ast_node_create(AST_TRY);
    if (!try_node) {
        report_error("Failed to allocate AST node for TRY");
        return NULL;
    }

    // Парсим тело TRY до CATCH или ENDTRY
    while (1) {
        Token *next_token = token_stream_peek(ts);
        if (!next_token) {
            report_error("Unexpected end of tokens inside TRY block");
            ast_node_free(try_node);
            return NULL;
        }

        if (next_token->type == TOKEN_CATCH || next_token->type == TOKEN_ENDTRY) {
            break;
        }

        ASTNode *stmt = parse_statement(ts);
        if (!stmt) {
            report_error("Failed to parse statement inside TRY block");
            ast_node_free(try_node);
            return NULL;
        }

        ast_node_add_child(try_node, stmt);
    }

    // Парсим CATCH-блоки, если есть
    while (1) {
        Token *next_token = token_stream_peek(ts);
        if (!next_token) {
            report_error("Unexpected end of tokens inside TRY");
            ast_node_free(try_node);
            return NULL;
        }

        if (next_token->type == TOKEN_CATCH) {
            token_stream_next(ts); // consume CATCH

            // Ожидается идентификатор исключения
            Token *ex_token = token_stream_next(ts);
            if (!ex_token || ex_token->type != TOKEN_IDENTIFIER) {
                report_error("Expected exception identifier after CATCH");
                ast_node_free(try_node);
                return NULL;
            }

            ASTNode *catch_node = ast_node_create(AST_CATCH);
            if (!catch_node) {
                report_error("Failed to allocate AST node for CATCH");
                ast_node_free(try_node);
                return NULL;
            }

            catch_node->string_value = strdup(ex_token->text);
            if (!catch_node->string_value) {
                report_error("Failed to allocate memory for exception identifier");
                ast_node_free(catch_node);
                ast_node_free(try_node);
                return NULL;
            }

            // Парсим тело CATCH до следующего CATCH или ENDTRY
            while (1) {
                Token *peek = token_stream_peek(ts);
                if (!peek) {
                    report_error("Unexpected end of tokens inside CATCH block");
                    ast_node_free(catch_node);
                    ast_node_free(try_node);
                    return NULL;
                }

                if (peek->type == TOKEN_CATCH || peek->type == TOKEN_ENDTRY) {
                    break;
                }

                ASTNode *stmt = parse_statement(ts);
                if (!stmt) {
                    report_error("Failed to parse statement inside CATCH block");
                    ast_node_free(catch_node);
                    ast_node_free(try_node);
                    return NULL;
                }

                ast_node_add_child(catch_node, stmt);
            }

            ast_node_add_child(try_node, catch_node);
        }
        else {
            break; // нет больше CATCH-блоков
        }
    }

    // Ожидается ENDTRY
    Token *endtry = token_stream_next(ts);
    if (!endtry || endtry->type != TOKEN_ENDTRY) {
        report_error("Expected ENDTRY after TRY/CATCH blocks");
        ast_node_free(try_node);
        return NULL;
    }

    // Точка после ENDTRY
    Token *dot = token_stream_next(ts);
    if (!dot || dot->type != TOKEN_DOT) {
        report_error("Expected '.' after ENDTRY");
        ast_node_free(try_node);
        return NULL;
    }

    return try_node;
}
```

---

### Объяснение:

* Парсится тело TRY-блока до CATCH или ENDTRY.
* Парсятся ноль или более блоков CATCH с идентификаторами исключений и их телами.
* После чего ожидается ENDTRY с точкой.
* Все части добавляются в AST узел TRY.

---

Если нужно, могу продолжить на следующий файл.
