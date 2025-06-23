Следующий файл — `parser/control/check.c`.

---

### Задача `check.c` (control):

Обработка оператора `CHECK` в ABAP — проверка условия, и если оно ложно, прерывание текущей итерации цикла или процедуры.

---

## Реализация `check.c`

```c
#include "../../include/parser.h"
#include "../../include/token.h"
#include "../../include/ast.h"
#include "../../include/error.h"

/**
 * parse_control_check - Парсит оператор CHECK.
 *
 * Оператор CHECK проверяет условие, и если оно ложно,
 * выполнение текущей процедуры или цикла прерывается.
 * Синтаксис: CHECK <условие>.
 *
 * Возвращает AST узел оператора CHECK с дочерним узлом условия.
 */
ASTNode *parse_control_check(TokenStream *ts) {
    if (!ts) {
        report_error("TokenStream is NULL in parse_control_check");
        return NULL;
    }

    Token *tok = token_stream_next(ts);
    if (!tok || tok->type != TOKEN_CHECK) {
        report_error("Expected CHECK token");
        return NULL;
    }

    // Парсим выражение условия после CHECK
    ASTNode *condition = parse_expression(ts);
    if (!condition) {
        report_error("Failed to parse condition expression after CHECK");
        return NULL;
    }

    ASTNode *check_node = ast_node_create(AST_CONTROL_CHECK);
    if (!check_node) {
        report_error("Failed to create AST node for CHECK");
        ast_node_free(condition);
        return NULL;
    }

    ast_node_add_child(check_node, condition);
    return check_node;
}
```

---

### Пример ABAP:

```abap
LOOP AT itab INTO wa.
  CHECK wa-field > 0.
  " Если условие ложно, текущая итерация пропускается
ENDLOOP.
```

---

### Основные AST узлы:

* `AST_CONTROL_CHECK` — оператор CHECK;
* дочерний узел — выражение условия.

---

Если нужно продолжить, скажи `следующий файл`.
