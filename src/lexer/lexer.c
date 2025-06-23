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


///////////////////////////////

#include "lexer.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

static void lexer_advance(Lexer *lexer);
static char lexer_peek(Lexer *lexer);
static Token lexer_make_token(Lexer *lexer, TokenType type);
static void lexer_skip_whitespace(Lexer *lexer);
static Token lexer_parse_identifier_or_keyword(Lexer *lexer);
static Token lexer_parse_number(Lexer *lexer);
static Token lexer_parse_string(Lexer *lexer);
static void lexer_skip_comment(Lexer *lexer);

// Инициализация лексера
void lexer_init(Lexer *lexer, const char *source) {
    lexer->source = source;
    lexer->pos = 0;
    lexer->line = 1;
    lexer->column = 1;
    lexer_advance(lexer);
}

// Получение следующего токена
Token lexer_next_token(Lexer *lexer) {
    lexer_skip_whitespace(lexer);

    if (lexer->current_char == '\0') {
        return lexer_make_token(lexer, TOKEN_EOF);
    }

    if (isalpha(lexer->current_char) || lexer->current_char == '_') {
        return lexer_parse_identifier_or_keyword(lexer);
    }

    if (isdigit(lexer->current_char)) {
        return lexer_parse_number(lexer);
    }

    if (lexer->current_char == '\'') {
        return lexer_parse_string(lexer);
    }

    // Обработка комментариев
    if (lexer->current_char == '*' && lexer->pos == 0) {
        lexer_skip_comment(lexer);
        return lexer_next_token(lexer);
    }

    // Обработка операторов и спецсимволов
    char c = lexer->current_char;
    lexer_advance(lexer);
    switch (c) {
        case '+': return lexer_make_token(lexer, TOKEN_PLUS);
        case '-': return lexer_make_token(lexer, TOKEN_MINUS);
        case '*': return lexer_make_token(lexer, TOKEN_MULTIPLY);
        case '/': return lexer_make_token(lexer, TOKEN_DIVIDE);
        case '=': return lexer_make_token(lexer, TOKEN_EQUAL);
        case '<': return lexer_make_token(lexer, TOKEN_LESS);
        case '>': return lexer_make_token(lexer, TOKEN_GREATER);
        case '(': return lexer_make_token(lexer, TOKEN_LPAREN);
        case ')': return lexer_make_token(lexer, TOKEN_RPAREN);
        case ';': return lexer_make_token(lexer, TOKEN_SEMICOLON);
        case ',': return lexer_make_token(lexer, TOKEN_COMMA);
        default:
            return lexer_make_token(lexer, TOKEN_UNKNOWN);
    }
}

// Переход к следующему символу
static void lexer_advance(Lexer *lexer) {
    if (lexer->current_char == '\n') {
        lexer->line++;
        lexer->column = 0;
    }

    if (lexer->source[lexer->pos] != '\0') {
        lexer->current_char = lexer->source[lexer->pos++];
        lexer->column++;
    } else {
        lexer->current_char = '\0';
    }
}

// Просмотр следующего символа без продвижения
static char lexer_peek(Lexer *lexer) {
    return lexer->source[lexer->pos];
}

// Создание токена
static Token lexer_make_token(Lexer *lexer, TokenType type) {
    Token token;
    token.type = type;
    token.line = lexer->line;
    token.column = lexer->column - 1; // текущий символ уже продвинут
    token.length = 1;
    token.text = NULL;
    return token;
}

// Пропуск пробелов и табуляций
static void lexer_skip_whitespace(Lexer *lexer) {
    while (lexer->current_char == ' ' || lexer->current_char == '\t' || lexer->current_char == '\r' || lexer->current_char == '\n') {
        lexer_advance(lexer);
    }
}

// Парсинг идентификатора или ключевого слова
static Token lexer_parse_identifier_or_keyword(Lexer *lexer) {
    size_t start_pos = lexer->pos - 1;
    size_t start_col = lexer->column - 1;

    while (isalnum(lexer->current_char) || lexer->current_char == '_') {
        lexer_advance(lexer);
    }

    size_t length = lexer->pos - 1 - start_pos;

    // Сохраняем текст токена
    char *text = malloc(length + 1);
    memcpy(text, &lexer->source[start_pos], length);
    text[length] = '\0';

    Token token;
    token.text = text;
    token.length = (int)length;
    token.line = lexer->line;
    token.column = start_col;

    // Проверка ключевых слов (упрощённо, например IF и ENDIF)
    if (strcasecmp(text, "IF") == 0) {
        token.type = TOKEN_KEYWORD_IF;
    } else if (strcasecmp(text, "ENDIF") == 0) {
        token.type = TOKEN_KEYWORD_ENDIF;
    } else if (strcasecmp(text, "DATA") == 0) {
        token.type = TOKEN_KEYWORD_DATA;
    } else {
        token.type = TOKEN_IDENTIFIER;
    }

    return token;
}

// Парсинг числового литерала
static Token lexer_parse_number(Lexer *lexer) {
    size_t start_pos = lexer->pos - 1;
    size_t start_col = lexer->column - 1;

    while (isdigit(lexer->current_char)) {
        lexer_advance(lexer);
    }

    size_t length = lexer->pos - 1 - start_pos;

    char *text = malloc(length + 1);
    memcpy(text, &lexer->source[start_pos], length);
    text[length] = '\0';

    Token token;
    token.type = TOKEN_LITERAL_NUMBER;
    token.text = text;
    token.length = (int)length;
    token.line = lexer->line;
    token.column = start_col;
    return token;
}

// Парсинг строкового литерала, заключенного в одинарные кавычки
static Token lexer_parse_string(Lexer *lexer) {
    size_t start_col = lexer->column - 1;
    size_t start_pos = lexer->pos; // после '

    while (lexer->current_char != '\'' && lexer->current_char != '\0') {
        lexer_advance(lexer);
    }

    size_t length = lexer->pos - start_pos;

    char *text = malloc(length + 1);
    memcpy(text, &lexer->source[start_pos], length);
    text[length] = '\0';

    if (lexer->current_char == '\'') {
        lexer_advance(lexer); // пропускаем закрывающую кавычку
    }

    Token token;
    token.type = TOKEN_LITERAL_STRING;
    token.text = text;
    token.length = (int)length;
    token.line = lexer->line;
    token.column = start_col;
    return token;
}

// Пропуск строки комментария, начинающейся с '*' в начале строки (ABAP style)
static void lexer_skip_comment(Lexer *lexer) {
    while (lexer->current_char != '\n' && lexer->current_char != '\0') {
        lexer_advance(lexer);
    }
}
