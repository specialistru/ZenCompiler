Следующий файл — `parser/assignment/chain.c`.

---

### Задача `chain.c` (assignment):

Парсинг цепочек присваиваний в ABAP, например, `var1 = var2 = expression.`

---

## Реализация `chain.c`

```c
#include "../../include/parser.h"
#include "../../include/token.h"
#include "../../include/ast.h"
#include "../../include/error.h"
#include <stdlib.h>
#include <string.h>

/**
 * parse_chain_assignment - Парсит цепочку присваиваний вида:
 * var1 = var2 = expression.
 *
 * Реализует правостороннюю ассоциативность:
 * выражение справа присваивается var2, затем var2 присваивается var1.
 *
 * Возвращает AST узел присваивания или NULL при ошибке.
 */

ASTNode *parse_chain_assignment(TokenStream *ts) {
    if (!ts) {
        report_error("TokenStream is NULL in parse_chain_assignment");
        return NULL;
    }

    // Парсим левый идентификатор
    Token *var_token = token_stream_next(ts);
    if (!var_token || var_token->type != TOKEN_IDENTIFIER) {
        report_error("Expected identifier at start of chain assignment");
        return NULL;
    }

    Token *assign_token = token_stream_next(ts);
    if (!assign_token || assign_token->type != TOKEN_ASSIGN) {
        report_error("Expected '=' after identifier in chain assignment");
        return NULL;
    }

    // Проверяем следующий токен: если идентификатор, возможно цепочка, иначе выражение
    Token *next_token = token_stream_peek(ts);
    if (!next_token) {
        report_error("Unexpected end of tokens in chain assignment");
        return NULL;
    }

    ASTNode *right_node = NULL;

    if (next_token->type == TOKEN_IDENTIFIER) {
        // Рекурсивно парсим цепочку дальше
        right_node = parse_chain_assignment(ts);
        if (!right_node) return NULL;
    } else {
        // Парсим простое выражение справа
        right_node = parse_expression(ts);
        if (!right_node) return NULL;
    }

    // Создаем узел присваивания для текущего идентификатора
    ASTNode *assign_node = ast_node_create(AST_ASSIGNMENT);
    if (!assign_node) {
        report_error("Failed to allocate AST node for chain assignment");
        ast_node_free(right_node);
        return NULL;
    }

    ASTNode *var_node = ast_node_create(AST_VARIABLE);
    if (!var_node) {
        report_error("Failed to allocate AST node for variable");
        ast_node_free(right_node);
        ast_node_free(assign_node);
        return NULL;
    }
    var_node->string_value = strdup(var_token->text);
    if (!var_node->string_value) {
        report_error("Failed to allocate memory for variable name");
        ast_node_free(right_node);
        ast_node_free(assign_node);
        ast_node_free(var_node);
        return NULL;
    }

    ast_node_add_child(assign_node, var_node);
    ast_node_add_child(assign_node, right_node);

    // Проверяем конец оператора, если это верхний уровень
    Token *end_token = token_stream_peek(ts);
    if (end_token && (end_token->type == TOKEN_DOT || end_token->type == TOKEN_SEMICOLON)) {
        token_stream_next(ts); // consume end token
    }

    return assign_node;
}
```

---

### Объяснение:

* Реализована рекурсивная обработка цепочек присваиваний.
* Каждое присваивание строит AST с двумя детьми: переменная и правая часть.
* Правая часть может быть либо выражением, либо следующей цепочкой.
* Проверяется корректность синтаксиса.

---

Готов к следующему файлу?
