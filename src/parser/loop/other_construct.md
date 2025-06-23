Следующий файл — `parser/loop/other_construct.c`.

---

### Задача `other_construct.c` (loop):

Обработка прочих управляющих конструкций в блоках циклов, таких как EXIT, CONTINUE и RETURN.

---

## Реализация `other_construct.c`

```c
#include "../../include/parser.h"
#include "../../include/token.h"
#include "../../include/ast.h"
#include "../../include/error.h"
#include <stdlib.h>

/**
 * parse_loop_control_construct - Парсит управляющие конструкции в циклах:
 * EXIT, CONTINUE, RETURN.
 *
 * Ожидается ключевое слово и точка, например:
 * EXIT.
 * CONTINUE.
 * RETURN.
 *
 * Возвращает AST узел управляющей конструкции или NULL при ошибке.
 */
ASTNode *parse_loop_control_construct(TokenStream *ts) {
    if (!ts) {
        report_error("TokenStream is NULL in parse_loop_control_construct");
        return NULL;
    }

    Token *token = token_stream_next(ts);
    if (!token) {
        report_error("Unexpected end of tokens while parsing loop control construct");
        return NULL;
    }

    ASTNodeType node_type;
    if (token->type == TOKEN_KEYWORD_EXIT) {
        node_type = AST_EXIT;
    } else if (token->type == TOKEN_KEYWORD_CONTINUE) {
        node_type = AST_CONTINUE;
    } else if (token->type == TOKEN_KEYWORD_RETURN) {
        node_type = AST_RETURN;
    } else {
        report_error("Expected 'EXIT', 'CONTINUE' or 'RETURN' keyword");
        return NULL;
    }

    Token *dot_token = token_stream_next(ts);
    if (!dot_token || dot_token->type != TOKEN_DOT) {
        report_error("Expected '.' after loop control keyword");
        return NULL;
    }

    ASTNode *control_node = ast_node_create(node_type);
    if (!control_node) {
        report_error("Failed to allocate AST node for loop control construct");
        return NULL;
    }

    return control_node;
}
```

---

### Объяснение:

* Обрабатывает управляющие команды, которые могут встречаться внутри циклов.
* Проверяет правильность синтаксиса — ключевое слово + точка.
* Возвращает соответствующий AST узел.

---

Готов перейти к следующему файлу!
