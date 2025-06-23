Следующий файл — `parser/control/return.c`.

---

### Задача `return.c` (control):

Обработка оператора `RETURN` в ABAP — выход из текущей процедуры или метода с возвратом управления.

---

## Реализация `return.c`

```c
#include "../../include/parser.h"
#include "../../include/token.h"
#include "../../include/ast.h"
#include "../../include/error.h"

/**
 * parse_control_return - Парсит оператор RETURN.
 *
 * Оператор RETURN завершает выполнение текущей процедуры или метода,
 * возвращая управление вызывающему коду.
 * Синтаксис: RETURN.
 *
 * Возвращает AST узел оператора RETURN.
 */
ASTNode *parse_control_return(TokenStream *ts) {
    if (!ts) {
        report_error("TokenStream is NULL in parse_control_return");
        return NULL;
    }

    Token *tok = token_stream_next(ts);
    if (!tok || tok->type != TOKEN_RETURN) {
        report_error("Expected RETURN token");
        return NULL;
    }

    ASTNode *return_node = ast_node_create(AST_CONTROL_RETURN);
    if (!return_node) {
        report_error("Failed to create AST node for RETURN");
        return NULL;
    }

    return return_node;
}
```

---

### Пример ABAP:

```abap
FORM do_something.
  IF error_condition.
    RETURN.
  ENDIF.
  " Остальной код формы
ENDFORM.
```

---

### Основной AST узел:

* `AST_CONTROL_RETURN` — оператор RETURN.

---

Если нужно, могу подготовить следующий файл — скажи `следующий файл`.
