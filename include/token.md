Следующий файл из директории `include/` — `token.h`

---

### Назначение `token.h`:

Определяет структуру токена, типы токенов, а также функции и константы, используемые для лексического анализа. Обеспечивает интерфейс для работы с токенами в лексере и парсере.

---

## Пример реализации `token.h`

```c
#ifndef TOKEN_H
#define TOKEN_H

#include <stdint.h>

// Типы токенов, встречающиеся при разборе ABAP
typedef enum {
    TOKEN_EOF,          // Конец файла
    TOKEN_IDENTIFIER,   // Идентификатор (имена переменных, функций и т.п.)
    TOKEN_KEYWORD,      // Ключевое слово языка
    TOKEN_LITERAL,      // Литералы (числа, строки)
    TOKEN_OPERATOR,     // Операторы (+, -, *, / и др.)
    TOKEN_SEPARATOR,    // Разделители (, ; ( ) и др.)
    TOKEN_COMMENT,      // Комментарии
    TOKEN_UNKNOWN       // Неизвестный или ошибочный токен
} TokenType;

// Структура токена
typedef struct {
    TokenType type;      // Тип токена
    char *text;          // Текст токена (строка)
    uint32_t line;       // Номер строки в исходном коде
    uint32_t column;     // Позиция в строке
} Token;

// Функции создания и уничтожения токена
Token *token_create(TokenType type, const char *text, uint32_t line, uint32_t column);
void token_destroy(Token *token);

#endif // TOKEN_H
```

---

Если хочешь, могу реализовать и `token.c` или следующий файл из `include`. Просто скажи.
