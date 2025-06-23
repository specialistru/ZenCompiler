Следующий файл — `parser/form/syntax_error.c`.

---

### Задача `syntax_error.c` (form):

Обработка и выдача ошибок синтаксиса, связанных с конструкциями FORM, например, отсутствие обязательных ключевых слов, неправильный порядок, ошибки в параметрах.

---

## Реализация `syntax_error.c`

```c
#include "../../include/error.h"
#include "../../include/token.h"
#include <stdio.h>

/**
 * report_form_syntax_error - Выводит подробное сообщение об ошибке синтаксиса
 * при парсинге конструкции FORM.
 *
 * @param token   - Токен, на котором обнаружена ошибка.
 * @param message - Сообщение с описанием ошибки.
 *
 * Функция логирует ошибку с указанием позиции и контекста.
 */

void report_form_syntax_error(const Token *token, const char *message) {
    if (!token || !message) {
        fprintf(stderr, "Syntax error in FORM: unknown location - %s\n", message ? message : "no message");
        return;
    }

    fprintf(stderr, "Syntax error in FORM at line %d, column %d: %s\n",
            token->line, token->column, message);
}
```

---

### Объяснение:

* Функция принимает токен и сообщение ошибки.
* Выводит детальное сообщение с позицией (строка, столбец).
* Помогает локализовать ошибки при парсинге FORM.

---

Если нужна интеграция с общей системой ошибок — могу помочь дополнительно.

Готов к следующему файлу?
