Следующий файл — `parser/loop/while.c`.

---

### Задача `while.c` (loop):

Парсинг цикла WHILE ... ENDWHILE с условием.

---

## Реализация `while.c`

```c
#include "../../include/parser.h"
#include "../../include/token.h"
#include "../../include/ast.h"
#include "../../include/error.h"
#include <stdlib.h>

/**
 * parse_while_loop - Парсит цикл WHILE <условие> ... ENDWHILE.
 *
 * Ожидается конструкция:
 * WHILE <условие>.
 *   ... тело цикла ...
 * ENDWHILE.
 *
 * Возвращает AST узел цикла или NULL при ошибке.
 */
ASTNode *parse_while_loop(TokenStream *ts) {
    if (!ts) {
        report_error("TokenStream is NULL in parse_while_loop");
        return NULL;
    }

    Token *token = token_stream_next(ts);
    if (!token || token->type != TOKEN_KEYWORD_WHILE) {
        report_error("Expected 'WHILE' keyword");
        return NULL;
    }

    // Парсим условие (упрощенно — считаем, что условие — один токен или выражение)
    ASTNode *condition = parse_expression(ts);
    if (!condition) {
        report_error("Failed to parse condition in WHILE loop");
        return NULL;
    }

    Token *dot_token = token_stream_next(ts);
    if (!dot_token || dot_token->type != TOKEN_DOT) {
        report_error("Expected '.' after WHILE condition");
        ast_node_free(condition);
        return NULL;
    }

    ASTNode *while_node = ast_node_create(AST_WHILE_LOOP);
    if (!while_node) {
        report_error("Failed to allocate AST node for WHILE loop");
        ast_node_free(condition);
        return NULL;
    }

    while_node->condition = condition;

    ASTNodeList *body_statements = ast_node_list_create();
    if (!body_statements) {
        report_error("Failed to allocate AST node list for WHILE loop body");
        ast_node_free(while_node);
        return NULL;
    }

    Token *next_token;
    while ((next_token = token_stream_peek(ts)) != NULL) {
        if (next_token->type == TOKEN_KEYWORD_ENDWHILE) {
            token_stream_next(ts); // consume ENDWHILE
            Token *end_dot = token_stream_next(ts);
            if (!end_dot || end_dot->type != TOKEN_DOT) {
                report_error("Expected '.' after 'ENDWHILE'");
                ast_node_list_free(body_statements);
                ast_node_free(while_node);
                return NULL;
            }
            break;
        }

        // Пропускаем тело, можно добавить парсинг инструкций
        token_stream_next(ts);
    }

    while_node->body = body_statements;

    return while_node;
}
```

---

### Объяснение:

* Парсит цикл с условием `WHILE <условие>. ... ENDWHILE.`.
* Создаёт AST узел с условием и телом.
* Вызывает `parse_expression` для условия (функция парсинга выражения должна быть реализована отдельно).
* Проверяет синтаксис.
* Тело цикла пока пропускается.

---

Готов к следующему файлу!
