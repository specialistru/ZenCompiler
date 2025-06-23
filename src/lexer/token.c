#include "token.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// Создание нового токена с указанным типом и текстом
Token *token_create(TokenType type, const char *text, int line, int column) {
    Token *token = (Token *)malloc(sizeof(Token));
    if (!token) {
        fprintf(stderr, "Out of memory while creating token\n");
        exit(EXIT_FAILURE);
    }
    token->type = type;
    token->line = line;
    token->column = column;

    if (text) {
        token->text = strdup(text);
        if (!token->text) {
            fprintf(stderr, "Out of memory while copying token text\n");
            free(token);
            exit(EXIT_FAILURE);
        }
    } else {
        token->text = NULL;
    }

    return token;
}

// Копирование токена (создает новый токен с такими же данными)
Token *token_copy(const Token *src) {
    if (!src) return NULL;
    return token_create(src->type, src->text, src->line, src->column);
}

// Освобождение памяти, занятой токеном
void token_free(Token *token) {
    if (!token) return;
    if (token->text) {
        free(token->text);
        token->text = NULL;
    }
    free(token);
}

// Вывод информации о токене в stdout (для отладки)
void token_print(const Token *token) {
    if (!token) {
        printf("Token: NULL\n");
        return;
    }
    printf("Token(type=%d, text=\"%s\", line=%d, column=%d)\n",
           token->type,
           token->text ? token->text : "(null)",
           token->line,
           token->column);
}
