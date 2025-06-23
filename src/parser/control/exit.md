Следующий файл — `parser/control/exit.c`.

---

### Задача `exit.c` (control):

Обработка оператора управления `EXIT` в ABAP — выход из текущего цикла или процедуры.

---

## Реализация `exit.c`

```c
#include "../../include/parser.h"
#include "../../include/token.h"
#include "../../include/ast.h"
#include "../../include/error.h"

/**
 * parse_control_exit - Парсит оператор EXIT.
 *
 * Оператор EXIT используется для досрочного выхода из цикла или блока.
 * Синтаксис: EXIT.
 *
 * Возвращает AST узел оператора EXIT.
 */
ASTNode *parse_control_exit(TokenStream *ts) {
    if (!ts) {
        report_error("TokenStream is NULL in parse_control_exit");
        return NULL;
    }

    Token *tok = token_stream_next(ts);
    if (!tok || tok->type != TOKEN_EXIT) {
        report_error("Expected EXIT token");
        return NULL;
    }

    ASTNode *exit_node = ast_node_create(AST_CONTROL_EXIT);
    if (!exit_node) {
        report_error("Failed to create AST node for EXIT");
        return NULL;
    }

    return exit_node;
}
```

---

### Пример ABAP:

```abap
LOOP AT itab INTO wa.
  IF condition.
    EXIT.
  ENDIF.
ENDLOOP.
```

---

### Основной AST узел:

* `AST_CONTROL_EXIT` — оператор EXIT.

---

Готов к следующему файлу — просто скажи `следующий файл`.
