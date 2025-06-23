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
