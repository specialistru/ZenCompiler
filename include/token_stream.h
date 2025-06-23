/*

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
*/

#ifndef TOKEN_STREAM_H
#define TOKEN_STREAM_H

#include "token.h"
#include <stdbool.h>

/**
 * @file token_stream.h
 * @brief Интерфейс для работы с потоком токенов (Token Stream) на этапе синтаксического анализа.
 *
 * Модуль обеспечивает последовательный доступ к лексемам (токенам), а также поддерживает
 * сохранение и восстановление позиции для реализации lookahead и откатов.
 */

/// Максимальное количество сохраняемых позиций (для lookahead и бэктрекинга)
#define MAX_SAVED_POSITIONS 16

/**
 * @struct TokenStream
 * @brief Поток токенов, полученных от лексера.
 *
 * Структура содержит массив токенов, текущую позицию чтения, а также стек сохранённых позиций.
 */
typedef struct TokenStream {
    Token *tokens;                         ///< Указатель на массив токенов
    int token_count;                       ///< Общее количество токенов в массиве
    int current_index;                     ///< Индекс текущего токена
    int saved_positions[MAX_SAVED_POSITIONS]; ///< Стек сохранённых позиций
    int saved_count;                       ///< Текущее количество сохранённых позиций
} TokenStream;

/**
 * @brief Инициализация потока токенов.
 *
 * @param stream Указатель на поток.
 * @param tokens Указатель на массив токенов.
 * @param count Количество токенов в массиве.
 */
void token_stream_init(TokenStream *stream, Token *tokens, int count);

/**
 * @brief Просмотр текущего токена без смещения позиции.
 *
 * @param stream Поток токенов.
 * @return Текущий токен или TOKEN_EOF, если достигнут конец потока.
 */
Token token_stream_peek(TokenStream *stream);

/**
 * @brief Получение текущего токена и смещение на следующий.
 *
 * @param stream Поток токенов.
 * @return Возвращает текущий токен и перемещает позицию вперёд.
 */
Token token_stream_next(TokenStream *stream);

/**
 * @brief Проверка на достижение конца потока.
 *
 * @param stream Поток токенов.
 * @return true, если конец достигнут, иначе false.
 */
bool token_stream_is_end(TokenStream *stream);

/**
 * @brief Сброс текущей позиции потока к началу.
 *
 * @param stream Поток токенов.
 */
void token_stream_reset(TokenStream *stream);

/**
 * @brief Сохранение текущей позиции в стек.
 *
 * Используется для реализации lookahead и альтернативного парсинга.
 *
 * @param stream Поток токенов.
 */
void token_stream_save(TokenStream *stream);

/**
 * @brief Восстановление последней сохранённой позиции.
 *
 * @param stream Поток токенов.
 */
void token_stream_restore(TokenStream *stream);

/**
 * @brief Удаление последней сохранённой позиции без отката.
 *
 * @param stream Поток токенов.
 */
void token_stream_discard(TokenStream *stream);

#endif // TOKEN_STREAM_H
