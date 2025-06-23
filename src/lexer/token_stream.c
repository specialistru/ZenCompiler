/*
#include "token_stream.h"

void token_stream_init(TokenStream *stream, Token *tokens, int count) {
    stream->tokens = tokens;
    stream->token_count = count;
    stream->current_index = 0;
}

Token token_stream_peek(TokenStream *stream) {
    if (stream->current_index >= stream->token_count) {
        Token eof_token = { TOKEN_EOF, NULL, 0, 0 };
        return eof_token;
    }
    return stream->tokens[stream->current_index];
}

Token token_stream_next(TokenStream *stream) {
    if (stream->current_index >= stream->token_count) {
        Token eof_token = { TOKEN_EOF, NULL, 0, 0 };
        return eof_token;
    }
    return stream->tokens[stream->current_index++];
}

bool token_stream_is_end(TokenStream *stream) {
    return stream->current_index >= stream->token_count;
}

void token_stream_reset(TokenStream *stream) {
    stream->current_index = 0;
}
*/

#include "token_stream.h"

/**
 * @brief Инициализация потока токенов.
 */
void token_stream_init(TokenStream *stream, Token *tokens, int count) {
    stream->tokens = tokens;
    stream->token_count = count;
    stream->current_index = 0;
    stream->saved_count = 0;
}

/**
 * @brief Получение текущего токена без продвижения позиции.
 */
Token token_stream_peek(TokenStream *stream) {
    if (stream->current_index >= stream->token_count) {
        Token eof_token = { TOKEN_EOF, NULL, 0, 0 };
        return eof_token;
    }
    return stream->tokens[stream->current_index];
}

/**
 * @brief Получение текущего токена и продвижение позиции.
 */
Token token_stream_next(TokenStream *stream) {
    if (stream->current_index >= stream->token_count) {
        Token eof_token = { TOKEN_EOF, NULL, 0, 0 };
        return eof_token;
    }
    return stream->tokens[stream->current_index++];
}

/**
 * @brief Проверка, достигнут ли конец потока токенов.
 */
bool token_stream_is_end(TokenStream *stream) {
    return stream->current_index >= stream->token_count;
}

/**
 * @brief Сброс позиции чтения потока на начало.
 */
void token_stream_reset(TokenStream *stream) {
    stream->current_index = 0;
    stream->saved_count = 0;
}

/**
 * @brief Сохранение текущей позиции в стек.
 */
void token_stream_save(TokenStream *stream) {
    if (stream->saved_count < MAX_SAVED_POSITIONS) {
        stream->saved_positions[stream->saved_count++] = stream->current_index;
    }
}

/**
 * @brief Восстановление последней сохранённой позиции.
 */
void token_stream_restore(TokenStream *stream) {
    if (stream->saved_count > 0) {
        stream->current_index = stream->saved_positions[--stream->saved_count];
    }
}

/**
 * @brief Удаление последней сохранённой позиции без изменения текущего индекса.
 */
void token_stream_discard(TokenStream *stream) {
    if (stream->saved_count > 0) {
        --stream->saved_count;
    }
}
