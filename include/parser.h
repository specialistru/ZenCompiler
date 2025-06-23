#ifndef PARSER_H
#define PARSER_H

#include "token.h"
#include "ast.h"

// Структура потока токенов для парсера
typedef struct {
    Token **tokens;      // Массив указателей на токены
    int count;           // Количество токенов
    int position;        // Текущая позиция в потоке токенов
} TokenStream;

// Инициализация TokenStream
void token_stream_init(TokenStream *ts, Token **tokens, int count);

// Получение следующего токена (сдвиг позиции)
Token *token_stream_next(TokenStream *ts);

// Просмотр текущего токена без сдвига позиции
Token *token_stream_peek(TokenStream *ts);

// Функция для запуска парсинга исходного кода
ASTNode *parse_program(TokenStream *ts);

// Функция для освобождения ресурсов AST
void ast_node_free(ASTNode *node);

// Функция для вывода ошибок парсинга
void report_error(const char *format, ...);

#endif // PARSER_H


////////////////////////


#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"

// Типы узлов AST
typedef enum {
    AST_NODE_PROGRAM,
    AST_NODE_STATEMENT,
    AST_NODE_EXPRESSION,
    AST_NODE_DECLARATION,
    // ... другие типы узлов
} ASTNodeType;

// Структура узла AST
typedef struct ASTNode {
    ASTNodeType type;
    struct ASTNode **children;
    int child_count;
    Token token;  // Токен, связанный с этим узлом (например, ключевое слово)
} ASTNode;

// Контекст парсера
typedef struct {
    Lexer *lexer;
    Token current_token;
} Parser;

// Инициализация парсера
void parser_init(Parser *parser, Lexer *lexer);

// Разбор программы (возвращает корень AST)
ASTNode* parser_parse_program(Parser *parser);

// Освобождение памяти AST
void parser_free_ast(ASTNode *node);

#endif // PARSER_H
