Следующий файл — `parser/assignment/simple.c`.

---

### Задача `simple.c` (assignment):

Парсинг простых операторов присваивания в ABAP, например, `variable = expression.`

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
 * parse_simple_assignment - Парсит простое присваивание вида:
 * variable = expression.
 *
 * Проверяет, что левый операнд — идентификатор, правый — корректное выражение.
 * Возвращает AST узел присваивания или NULL при ошибке.
 */

ASTNode *parse_simple_assignment(TokenStream *ts) {
    if (!ts) {
        report_error("TokenStream is NULL in parse_simple_assignment");
        return NULL;
    }

    Token *var_token = token_stream_next(ts);
    if (!var_token || var_token->type != TOKEN_IDENTIFIER) {
        report_error("Expected identifier on left side of assignment");
        return NULL;
    }

    Token *assign_token = token_stream_next(ts);
    if (!assign_token || assign_token->type != TOKEN_ASSIGN) { // обычно '='
        report_error("Expected '=' in assignment");
        return NULL;
    }

    ASTNode *expr = parse_expression(ts);
    if (!expr) {
        report_error("Failed to parse expression on right side of assignment");
        return NULL;
    }

    ASTNode *assign_node = ast_node_create(AST_ASSIGNMENT);
    if (!assign_node) {
        report_error("Failed to allocate AST node for assignment");
        ast_node_free(expr);
        return NULL;
    }

    ASTNode *var_node = ast_node_create(AST_VARIABLE);
    if (!var_node) {
        report_error("Failed to allocate AST node for variable");
        ast_node_free(expr);
        ast_node_free(assign_node);
        return NULL;
    }
    var_node->string_value = strdup(var_token->text);
    if (!var_node->string_value) {
        report_error("Failed to allocate memory for variable name");
        ast_node_free(expr);
        ast_node_free(assign_node);
        ast_node_free(var_node);
        return NULL;
    }

    ast_node_add_child(assign_node, var_node);
    ast_node_add_child(assign_node, expr);

    // Проверка точки с запятой или точки (конец оператора)
    Token *end_token = token_stream_next(ts);
    if (!end_token || (end_token->type != TOKEN_DOT && end_token->type != TOKEN_SEMICOLON)) {
        report_error("Expected '.' or ';' at end of assignment statement");
        ast_node_free(assign_node);
        return NULL;
    }

    return assign_node;
}
```

---

### Объяснение:

* Левый операнд — идентификатор, затем оператор присваивания `=`.
* Правая часть — выражение (вызов функции `parse_expression`).
* Создается AST с узлом присваивания и двумя дочерними: переменная и выражение.
* Обязательно проверяется завершение оператора точкой или точкой с запятой.
* Сообщения об ошибках при некорректном синтаксисе.

---

Готов к следующему файлу?
