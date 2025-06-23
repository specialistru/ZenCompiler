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


Конечно! Ниже приведён полный список основных типов токенов, которые можно выделить в ABAP — включая ключевые слова, операторы, литералы, знаки пунктуации и специальные конструкции. Этот список можно использовать как основу для enum `TokenType` в вашем лексере.

---

### Полный список типов токенов ABAP

```c
typedef enum {
    // Специальные и служебные токены
    TOKEN_UNKNOWN = 0,         // Неизвестный токен
    TOKEN_EOF,                 // Конец файла

    // Ключевые слова (примерный набор)
    TOKEN_KEYWORD_ABORT,
    TOKEN_KEYWORD_ACCEPT,
    TOKEN_KEYWORD_APPEND,
    TOKEN_KEYWORD_BREAK,
    TOKEN_KEYWORD_CALL,
    TOKEN_KEYWORD_CASE,
    TOKEN_KEYWORD_CHECK,
    TOKEN_KEYWORD_CLASS,
    TOKEN_KEYWORD_CLEAR,
    TOKEN_KEYWORD_CLOSE,
    TOKEN_KEYWORD_COMMIT,
    TOKEN_KEYWORD_CONSTANTS,
    TOKEN_KEYWORD_CONTINUE,
    TOKEN_KEYWORD_DATA,
    TOKEN_KEYWORD_DELETE,
    TOKEN_KEYWORD_DO,
    TOKEN_KEYWORD_ELSE,
    TOKEN_KEYWORD_ELSEIF,
    TOKEN_KEYWORD_ENDCLASS,
    TOKEN_KEYWORD_ENDIF,
    TOKEN_KEYWORD_ENDLOOP,
    TOKEN_KEYWORD_ENDTRY,
    TOKEN_KEYWORD_ENDWHILE,
    TOKEN_KEYWORD_EVENT,
    TOKEN_KEYWORD_EXIT,
    TOKEN_KEYWORD_EXPORT,
    TOKEN_KEYWORD_FORM,
    TOKEN_KEYWORD_FUNCTION,
    TOKEN_KEYWORD_IF,
    TOKEN_KEYWORD_IMPORT,
    TOKEN_KEYWORD_INTERFACE,
    TOKEN_KEYWORD_LOOP,
    TOKEN_KEYWORD_METHODS,
    TOKEN_KEYWORD_MODIFY,
    TOKEN_KEYWORD_PERFORM,
    TOKEN_KEYWORD_PROCEDURE,
    TOKEN_KEYWORD_RAISE,
    TOKEN_KEYWORD_READ,
    TOKEN_KEYWORD_REFRESH,
    TOKEN_KEYWORD_RETURN,
    TOKEN_KEYWORD_SELECT,
    TOKEN_KEYWORD_SET,
    TOKEN_KEYWORD_SORT,
    TOKEN_KEYWORD_START,
    TOKEN_KEYWORD_STOP,
    TOKEN_KEYWORD_STRUCTURES,
    TOKEN_KEYWORD_SUBMIT,
    TOKEN_KEYWORD_SWITCH,
    TOKEN_KEYWORD_THEN,
    TOKEN_KEYWORD_THROW,
    TOKEN_KEYWORD_TRY,
    TOKEN_KEYWORD_WHILE,
    TOKEN_KEYWORD_WITH,

    // Литералы
    TOKEN_LITERAL_STRING,      // Строковый литерал (например 'текст')
    TOKEN_LITERAL_NUM_INT,     // Целочисленный литерал
    TOKEN_LITERAL_NUM_FLOAT,   // Вещественный литерал
    TOKEN_LITERAL_NUM_HEX,     // Шестнадцатеричный литерал
    TOKEN_LITERAL_CHAR,        // Символьный литерал

    // Идентификаторы (имена переменных, функций, классов и т.п.)
    TOKEN_IDENTIFIER,

    // Операторы
    TOKEN_OPERATOR_ASSIGN,     // =
    TOKEN_OPERATOR_PLUS,       // +
    TOKEN_OPERATOR_MINUS,      // -
    TOKEN_OPERATOR_MULTIPLY,   // *
    TOKEN_OPERATOR_DIVIDE,     // /
    TOKEN_OPERATOR_POWER,      // **
    TOKEN_OPERATOR_MODULO,     // MOD

    TOKEN_OPERATOR_EQ,         // =
    TOKEN_OPERATOR_NEQ,        // <>
    TOKEN_OPERATOR_LT,         // <
    TOKEN_OPERATOR_GT,         // >
    TOKEN_OPERATOR_LE,         // <=
    TOKEN_OPERATOR_GE,         // >=

    TOKEN_OPERATOR_AND,        // AND
    TOKEN_OPERATOR_OR,         // OR
    TOKEN_OPERATOR_NOT,        // NOT

    // Пунктуация и разделители
    TOKEN_PUNCTUATION_SEMICOLON,   // ;
    TOKEN_PUNCTUATION_COMMA,       // ,
    TOKEN_PUNCTUATION_DOT,         // .
    TOKEN_PUNCTUATION_COLON,       // :
    TOKEN_PUNCTUATION_LPAREN,      // (
    TOKEN_PUNCTUATION_RPAREN,      // )
    TOKEN_PUNCTUATION_LBRACKET,    // [
    TOKEN_PUNCTUATION_RBRACKET,    // ]
    TOKEN_PUNCTUATION_LBRACE,      // {
    TOKEN_PUNCTUATION_RBRACE,      // }

    // Специальные конструкции и символы
    TOKEN_COMMENT_LINE,         // Однострочный комментарий
    TOKEN_COMMENT_BLOCK,        // Многострочный комментарий

    // Другие
    TOKEN_NEWLINE,              // Перевод строки
    TOKEN_WHITESPACE,           // Пробелы, табуляции и т.п.

} TokenType;
```

---

### Пояснения:

* **Ключевые слова**: перечислены основные конструкции ABAP, их список можно расширять.
* **Литералы**: строки, числа, символы.
* **Идентификаторы**: имена переменных, функций, классов и т.п.
* **Операторы**: арифметические, логические, сравнительные.
* **Пунктуация**: важна для разбора структуры кода.
* **Комментарии и пробельные символы**: учитываются для полноценного лексического анализа.

---

Если хотите, могу помочь с генерацией кода для работы с этими токенами или с их распознаванием в лексере!

