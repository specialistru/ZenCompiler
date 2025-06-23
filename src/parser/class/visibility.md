Следующий файл — `parser/class/visibility.c`.

---

### Задача `visibility.c` для class:

Парсинг спецификаторов видимости (`PUBLIC`, `PROTECTED`, `PRIVATE`) в определении класса или методов.

---

## Реализация `visibility.c`

```c
#include "../../include/parser.h"
#include "../../include/token.h"
#include "../../include/ast.h"
#include "../../include/error.h"

/**
 * parse_visibility - Парсит спецификатор видимости в классе.
 *
 * Поддерживаются ключевые слова:
 * PUBLIC SECTION.
 * PROTECTED SECTION.
 * PRIVATE SECTION.
 *
 * Возвращает тип видимости или ошибку.
 */
VisibilityType parse_visibility(TokenStream *ts) {
    if (!ts) {
        report_error("TokenStream is NULL in parse_visibility");
        return VISIBILITY_INVALID;
    }

    Token *token = token_stream_next(ts);
    if (!token) {
        report_error("Unexpected end of tokens in parse_visibility");
        return VISIBILITY_INVALID;
    }

    VisibilityType vis = VISIBILITY_INVALID;

    if (token->type == TOKEN_KEYWORD_PUBLIC) {
        vis = VISIBILITY_PUBLIC;
    } else if (token->type == TOKEN_KEYWORD_PROTECTED) {
        vis = VISIBILITY_PROTECTED;
    } else if (token->type == TOKEN_KEYWORD_PRIVATE) {
        vis = VISIBILITY_PRIVATE;
    } else {
        report_error_token("Expected visibility specifier (PUBLIC, PROTECTED, PRIVATE)", token);
        return VISIBILITY_INVALID;
    }

    // Ожидаем SECTION после видимости
    token = token_stream_next(ts);
    if (!token || token->type != TOKEN_KEYWORD_SECTION) {
        report_error("Expected 'SECTION' keyword after visibility specifier");
        return VISIBILITY_INVALID;
    }

    token = token_stream_next(ts);
    if (!token || token->type != TOKEN_DOT) {
        report_error("Expected '.' after visibility SECTION");
        return VISIBILITY_INVALID;
    }

    return vis;
}
```

---

### Объяснение:

* Обрабатывает ключевые слова видимости и проверяет правильность синтаксиса (`SECTION.`).
* Возвращает перечисление `VisibilityType`.
* Использует поток токенов и сообщает об ошибках через `report_error`.

---

Если хочешь, могу помочь с другим файлом.
