Следующий файл — `parser/class/endclass.c`.

---

### Задача `endclass.c`:

Парсинг окончания класса — ключевое слово `ENDCLASS.`

---

## Реализация `endclass.c`

```c
#include "../../include/parser.h"
#include "../../include/token.h"
#include "../../include/error.h"

/**
 * parse_endclass - Парсит конец определения класса.
 *
 * Ожидает конструкцию:
 * ENDCLASS.
 *
 * Возвращает 1 при успехе, 0 при ошибке.
 */
int parse_endclass(TokenStream *ts) {
    if (!ts) {
        report_error("TokenStream is NULL in parse_endclass");
        return 0;
    }

    Token *token = token_stream_next(ts);
    if (!token || token->type != TOKEN_KEYWORD_ENDCLASS) {
        report_error("Expected 'ENDCLASS' keyword");
        return 0;
    }

    token = token_stream_next(ts);
    if (!token || token->type != TOKEN_DOT) {
        report_error("Expected '.' after 'ENDCLASS'");
        return 0;
    }

    return 1;
}
```

---

### Объяснение:

* Функция проверяет, что следующий токен — `ENDCLASS`, за которым следует точка `.`.
* Возвращает 1 при корректном окончании, 0 при ошибке.
* Используется для завершения парсинга класса.

---

Готов помочь с другим файлом!
