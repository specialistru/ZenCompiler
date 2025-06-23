#ifndef LEXER_H
#define LEXER_H

#include "token.h"

// Структура лексера, содержащая исходный текст и состояние
typedef struct {
    const char *source;    // Указатель на исходный текст
    size_t length;         // Длина исходного текста
    size_t position;       // Текущая позиция в тексте
    uint32_t line;         // Текущая строка (для ошибок)
    uint32_t column;       // Текущая колонка (для ошибок)
} Lexer;

// Инициализация лексера с исходным текстом
void lexer_init(Lexer *lexer, const char *source, size_t length);

// Получение следующего токена из исходного текста
Token *lexer_next_token(Lexer *lexer);

#endif // LEXER_H

////////////////////////////////

#ifndef LEXER_H
#define LEXER_H

#include <stddef.h>

// Типы токенов лексера
typedef enum {
    TOKEN_IDENTIFIER,
    TOKEN_KEYWORD,
    TOKEN_LITERAL_STRING,
    TOKEN_LITERAL_NUMBER,
    TOKEN_OPERATOR,
    TOKEN_COMMENT,
    TOKEN_EOF,
    TOKEN_UNKNOWN
} TokenType;

// Структура токена
typedef struct {
    TokenType type;
    const char *text;     // Указатель на текст токена в исходном буфере
    size_t length;        // Длина токена
    int line;             // Номер строки в исходном файле
    int column;           // Номер столбца в исходном файле
} Token;

// Контекст лексера
typedef struct {
    const char *source;   // Исходный текст
    size_t position;      // Текущая позиция в тексте
    int line;             // Текущая строка
    int column;           // Текущий столбец
} Lexer;

// Инициализация лексера
void lexer_init(Lexer *lexer, const char *source);

// Получение следующего токена
Token lexer_next_token(Lexer *lexer);

#endif // LEXER_H
