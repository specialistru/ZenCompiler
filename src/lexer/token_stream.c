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
