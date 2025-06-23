Следующий файл из директории `src/core/` — `lexer.c`

---

### Назначение `lexer.c`:

Реализация лексического анализатора для разбора исходного кода ABAP на токены, учитывая ключевые слова, идентификаторы, литералы, комментарии, операторы и прочее.

---

## Пример реализации `lexer.c`

```c
#include "lexer.h"
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

static int is_keyword(const char *text, size_t length);
static TokenType identify_token_type(const char *text, size_t length);

void lexer_init(Lexer *lexer, const char *source) {
    lexer->source = source;
    lexer->position = 0;
    lexer->line = 1;
    lexer->column = 1;
}

static void advance(Lexer *lexer) {
    if (lexer->source[lexer->position] == '\n') {
        lexer->line++;
        lexer->column = 1;
    } else {
        lexer->column++;
    }
    lexer->position++;
}

static char current_char(Lexer *lexer) {
    return lexer->source[lexer->position];
}

static int is_identifier_start(char c) {
    return isalpha((unsigned char)c) || c == '_';
}

static int is_identifier_part(char c) {
    return isalnum((unsigned char)c) || c == '_';
}

Token lexer_next_token(Lexer *lexer) {
    // Skip whitespace
    while (isspace((unsigned char)current_char(lexer))) {
        advance(lexer);
    }

    char c = current_char(lexer);

    if (c == '\0') {
        return (Token){TOKEN_EOF, NULL, 0, lexer->line, lexer->column};
    }

    size_t start_pos = lexer->position;
    int start_line = lexer->line;
    int start_col = lexer->column;

    // Identifier or keyword
    if (is_identifier_start(c)) {
        while (is_identifier_part(current_char(lexer))) {
            advance(lexer);
        }
        size_t length = lexer->position - start_pos;
        const char *text = lexer->source + start_pos;

        TokenType type = is_keyword(text, length) ? TOKEN_KEYWORD : TOKEN_IDENTIFIER;

        return (Token){type, text, length, start_line, start_col};
    }

    // Number literal (integer only for simplicity)
    if (isdigit((unsigned char)c)) {
        while (isdigit((unsigned char)current_char(lexer))) {
            advance(lexer);
        }
        size_t length = lexer->position - start_pos;
        const char *text = lexer->source + start_pos;

        return (Token){TOKEN_LITERAL_NUMBER, text, length, start_line, start_col};
    }

    // String literal (assuming single quotes for ABAP)
    if (c == '\'') {
        advance(lexer); // Skip opening quote
        size_t str_start = lexer->position;

        while (current_char(lexer) != '\'' && current_char(lexer) != '\0') {
            advance(lexer);
        }

        size_t length = lexer->position - str_start;
        const char *text = lexer->source + str_start;

        if (current_char(lexer) == '\'') {
            advance(lexer); // Skip closing quote
            return (Token){TOKEN_LITERAL_STRING, text, length, start_line, start_col};
        } else {
            // Unterminated string literal
            return (Token){TOKEN_UNKNOWN, text, length, start_line, start_col};
        }
    }

    // Comment (single line starting with "*")
    if (c == '*') {
        // Consume till end of line
        advance(lexer);
        size_t comment_start = lexer->position;
        while (current_char(lexer) != '\n' && current_char(lexer) != '\0') {
            advance(lexer);
        }
        size_t length = lexer->position - comment_start;
        const char *text = lexer->source + comment_start;

        return (Token){TOKEN_COMMENT, text, length, start_line, start_col};
    }

    // Operators and special symbols
    // For simplicity, handle common operators: =, +, -, *, /, ., ,, ;, (), etc.
    {
        // single char token
        advance(lexer);
        return (Token){TOKEN_OPERATOR, lexer->source + start_pos, 1, start_line, start_col};
    }
}

// Проверяет, является ли текст ключевым словом ABAP
static int is_keyword(const char *text, size_t length) {
    // Для демонстрации небольшой набор ключевых слов
    static const char *keywords[] = {
        "IF", "ELSE", "ENDIF", "WHILE", "DO", "LOOP", "PERFORM", "DATA", "TYPES", "CONSTANTS", "CLASS", "INTERFACE", "FUNCTION"
    };
    for (size_t i = 0; i < sizeof(keywords)/sizeof(keywords[0]); i++) {
        if (strlen(keywords[i]) == length && strncasecmp(text, keywords[i], length) == 0) {
            return 1;
        }
    }
    return 0;
}
```

---

Если хочешь, могу продолжить с `parser.c` или другим файлом.
