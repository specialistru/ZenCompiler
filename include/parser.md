Следующий файл из директории `include/` — `parser.h`

---

### Назначение `parser.h`:

Объявляет интерфейсы и структуры, необходимые для парсера ABAP. Включает функции для запуска парсинга, работы с деревом синтаксического разбора (AST) и обработки ошибок.

---

## Пример реализации `parser.h`

```c
#ifndef PARSER_H
#define PARSER_H

#include "token.h"
#include "ast.h"

// Структура потока токенов для парсера
typedef struct {
    Token **tokens;      // Массив указателей на токены
    int count;           // Количество токенов
    int position;        // Текущая позиция в потоке токенов
} TokenStream;

// Инициализация TokenStream
void token_stream_init(TokenStream *ts, Token **tokens, int count);

// Получение следующего токена (сдвиг позиции)
Token *token_stream_next(TokenStream *ts);

// Просмотр текущего токена без сдвига позиции
Token *token_stream_peek(TokenStream *ts);

// Функция для запуска парсинга исходного кода
ASTNode *parse_program(TokenStream *ts);

// Функция для освобождения ресурсов AST
void ast_node_free(ASTNode *node);

// Функция для вывода ошибок парсинга
void report_error(const char *format, ...);

#endif // PARSER_H
```

---

Если нужен исходник `parser.c` или следующий файл — скажи.
