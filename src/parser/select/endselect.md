Следующий файл — `parser/select/endselect.c`.

---

### Задача `endselect.c` (select):

Обработка ключевого слова `ENDSELECT` — конец цикла обработки результатов SELECT-запроса.

---

## Реализация `endselect.c`

```c
#include "../../include/parser.h"
#include "../../include/token.h"
#include "../../include/ast.h"
#include "../../include/error.h"

/**
 * parse_endselect - Обрабатывает ключевое слово ENDSELECT, завершая цикл обработки результата SELECT.
 *
 * Возвращает AST узел ENDSELECT или NULL при ошибке.
 */
ASTNode *parse_endselect(TokenStream *ts) {
    if (!ts) {
        report_error("TokenStream is NULL in parse_endselect");
        return NULL;
    }

    // Просто создаём узел AST с типом ENDSELECT, поскольку это маркер конца
    ASTNode *endselect_node = ast_node_create(AST_ENDSELECT);
    if (!endselect_node) {
        report_error("Failed to allocate AST node for ENDSELECT");
        return NULL;
    }

    return endselect_node;
}
```

---

### Объяснение:

* `ENDSELECT` — специальный маркер окончания цикла обработки выборки.
* Создаёт узел AST для этого ключевого слова.
* Поддержка семантического анализа и генерации кода будет учитывать этот узел.

---

Готов продолжать с следующим файлом!
