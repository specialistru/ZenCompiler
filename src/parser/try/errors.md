Следующий файл — `parser/try/errors.c`.

---

### Задача `errors.c` (try):

Обработка ошибок парсинга конструкции TRY...CATCH...ENDTRY — детальная диагностика ошибок, вызванных неверным синтаксисом или пропущенными блоками.

---

## Реализация `errors.c`

```c
#include "../../include/parser.h"
#include "../../include/token.h"
#include "../../include/error.h"
#include <stdio.h>

/**
 * check_try_syntax_errors - Выполняет базовую проверку синтаксиса TRY блока.
 *
 * Проверяет наличие обязательных блоков (TRY, CATCH/ENDTRY),
 * корректность последовательности и ожидаемых токенов.
 * Возвращает 1, если есть ошибки, 0 если синтаксис корректен.
 */
int check_try_syntax_errors(TokenStream *ts) {
    if (!ts) {
        report_error("TokenStream is NULL in check_try_syntax_errors");
        return 1;
    }

    // Проверка начала блока TRY
    Token *first = token_stream_peek(ts);
    if (!first || first->type != TOKEN_TRY) {
        report_error("Expected 'TRY' keyword at the beginning");
        return 1;
    }

    // Проверка, что блок заканчивается ENDTRY.
    // Здесь можно пройти по токенам вперед и проверить структуру.

    int has_endtry = 0;
    int has_catch = 0;
    int pos = ts->position;
    Token *tok = NULL;
    while ((tok = token_stream_peek_offset(ts, pos++)) != NULL) {
        if (tok->type == TOKEN_CATCH) {
            has_catch = 1;
        }
        else if (tok->type == TOKEN_ENDTRY) {
            has_endtry = 1;
            break;
        }
    }

    if (!has_endtry) {
        report_error("Missing ENDTRY for TRY block");
        return 1;
    }

    if (!has_catch) {
        report_error("Missing CATCH block in TRY construct");
        return 1;
    }

    // Можно добавить дополнительные проверки (например, порядок блоков).

    return 0; // Ошибок не найдено
}
```

---

### Объяснение:

* Проверяется правильное начало блока TRY.
* Проверяется наличие CATCH и ENDTRY.
* Возвращается ошибка, если обязательные элементы отсутствуют.

---

Если надо — могу сделать следующий файл.
