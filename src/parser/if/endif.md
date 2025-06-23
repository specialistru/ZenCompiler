Конечно! Следующий файл — `parser/if/endif.c`.

---

### Задача `endif.c`:

Обработка окончания блока IF — обычно здесь логика минимальна, так как ENDIF проверяется в парсерах выше. Но можно сделать функцию для проверки и считывания ENDIF.

---

## Пример реализации `endif.c`

```c
#include "../../include/parser.h"
#include "../../include/token.h"
#include "../../include/error.h"
#include <stdio.h>

/**
 * parse_if_end - Проверяет и парсит ключевое слово ENDIF с точкой.
 *
 * Возвращает 0 при успехе, -1 при ошибке.
 */
int parse_if_end(TokenStream *ts) {
    if (!ts) {
        fprintf(stderr, "TokenStream is NULL\n");
        return -1;
    }

    Token *token = token_stream_next(ts);
    if (!token || token->type != TOKEN_KEYWORD_ENDIF) {
        report_error("Expected ENDIF keyword");
        return -1;
    }

    token = token_stream_next(ts);
    if (!token || token->type != TOKEN_DOT) {
        report_error("Expected '.' after ENDIF");
        return -1;
    }

    return 0; // успех
}
```

---

### Объяснение:

* Простая функция, которая пытается считать ENDIF и точку.
* Выдает ошибку если формат неверен.
* Обычно используется в основном парсере IF для контроля окончания блока.

---

Готов продолжать по списку!
Какой файл дальше?
