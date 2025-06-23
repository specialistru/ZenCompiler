#ifndef TOKEN_STREAM_H
#define TOKEN_STREAM_H

#include "token.h"  // Определения токенов и их типов
#include <stdbool.h>

/**
 * @file token_stream.h
 * @brief Интерфейс для работы с потоком токенов (Token Stream).
 *
 * Поток токенов используется для последовательного чтения токенов, полученных на этапе лексического анализа.
 * Он обеспечивает интерфейс для итерации, просмотра следующего токена и управления позицией в потоке.
 */

/**
 * Структура потока токенов.
 * Хранит массив токенов, текущий индекс и количество токенов.
 */
typedef struct TokenStream {
    Token *tokens;       ///< Указатель на массив токенов
    int token_count;     ///< Общее количество токенов в массиве
    int current_index;   ///< Индекс текущего токена (для чтения)
} TokenStream;

/**
 * Инициализация потока токенов.
 * 
 * @param stream Указатель на TokenStream для инициализации.
 * @param tokens Массив токенов.
 * @param count Количество токенов в массиве.
 */
void token_stream_init(TokenStream *stream, Token *tokens, int count);

/**
 * Получить текущий токен без смещения индекса.
 * 
 * @param stream Указатель на поток токенов.
 * @return Текущий токен или токен типа TOKEN_EOF, если поток закончился.
 */
Token token_stream_peek(TokenStream *stream);

/**
 * Получить текущий токен и сдвинуть указатель на следующий.
 * 
 * @param stream Указатель на поток токенов.
 * @return Текущий токен перед сдвигом, или TOKEN_EOF если конец потока.
 */
Token token_stream_next(TokenStream *stream);

/**
 * Проверить, достигнут ли конец потока токенов.
 * 
 * @param stream Указатель на поток токенов.
 * @return true если достигнут конец, иначе false.
 */
bool token_stream_is_end(TokenStream *stream);

/**
 * Сбросить указатель потока токенов на начало.
 * 
 * @param stream Указатель на поток токенов.
 */
void token_stream_reset(TokenStream *stream);

#endif // TOKEN_STREAM_H
