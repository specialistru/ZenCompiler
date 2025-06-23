Следующий файл — `parser/method/error.c`.

---

### Задача `error.c` (method):

Обработка и регистрация ошибок, возникающих при парсинге методов.

---

## Реализация `error.c`

```c
#include "../../include/error.h"
#include "../../include/token.h"
#include <stdio.h>

/**
 * report_method_error - Выводит ошибку, связанную с парсингом метода.
 *
 * @param message - Сообщение об ошибке.
 * @param token - Токен, на котором обнаружена ошибка (может быть NULL).
 */
void report_method_error(const char *message, const Token *token) {
    if (token) {
        fprintf(stderr, "Method parser error at line %d, column %d: %s (token: '%s')\n",
                token->line, token->column, message, token->text);
    } else {
        fprintf(stderr, "Method parser error: %s\n", message);
    }
}
```

---

### Объяснение:

* Функция принимает описание ошибки и токен для точной локализации.
* Выводит подробную информацию об ошибке в стандартный поток ошибок.
* Помогает быстро идентифицировать ошибки при парсинге методов.

---

Готов перейти к следующему файлу!
