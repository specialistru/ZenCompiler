/*
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
*/

#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include <stdbool.h>

/**
 * Типы AST-узлов (Abstract Syntax Tree)
 * Каждый тип отражает конкретный элемент синтаксиса ABAP.
 */
typedef enum {
    // Общие узлы
    AST_PROGRAM,              // Корневой узел программы
    AST_BLOCK,                // Блок операторов (BEGIN ... END)
    AST_STATEMENT,            // Общий оператор
    
    // Управляющие конструкции
    AST_IF_STATEMENT,         // Оператор IF ... ELSEIF ... ELSE ... ENDIF
    AST_ELSEIF_CLAUSE,        // Клауза ELSEIF в IF
    AST_ELSE_CLAUSE,          // Клауза ELSE в IF
    AST_CASE_STATEMENT,       // Оператор CASE ... WHEN ... ENDCASE
    AST_LOOP_STATEMENT,       // LOOP (итерация по таблице)
    AST_WHILE_STATEMENT,      // WHILE цикл
    AST_DO_STATEMENT,         // DO цикл
    AST_EXIT_STATEMENT,       // Оператор EXIT
    AST_CONTINUE_STATEMENT,   // Оператор CONTINUE
    AST_CHECK_STATEMENT,      // Оператор CHECK
    AST_RETURN_STATEMENT,     // Оператор RETURN
    
    // Объявления и определения
    AST_DATA_DECLARATION,     // DATA
    AST_TYPES_DECLARATION,    // TYPES
    AST_CONSTANT_DECLARATION, // CONSTANTS
    AST_FIELD_SYMBOL_DECLARATION, // FIELD-SYMBOLS
    AST_SELECT_OPTIONS_DECLARATION, // SELECT-OPTIONS
    AST_RANGES_DECLARATION,   // RANGES
    AST_CLASS_DECLARATION,    // CLASS
    AST_INTERFACE_DECLARATION,// INTERFACE
    AST_METHOD_DEFINITION,    // METHOD (определение метода)
    AST_METHOD_IMPLEMENTATION,// Реализация метода
    
    // Вызовы и процедуры
    AST_PERFORM_STATEMENT,    // PERFORM вызов подпрограммы
    AST_FUNCTION_CALL,        // Вызов функции (CALL FUNCTION)
    
    // Выражения
    AST_ASSIGNMENT,           // Присваивание
    AST_EXPRESSION,           // Общие выражения
    AST_LITERAL,              // Литералы (числа, строки)
    AST_IDENTIFIER,           // Идентификаторы (переменные, функции)
    AST_OPERATOR,             // Операторы (+, -, *, /, AND, OR и т.д.)
    AST_FUNCTION_CALL_EXPR,   // Вызов функции внутри выражения
    AST_ARRAY_ACCESS,         // Обращение к элементу массива/таблицы
    AST_TERNARY_EXPRESSION,   // Тернарный оператор
    AST_CONDITIONAL_EXPRESSION,// Условные выражения
    
    // Специальные конструкции
    AST_TRY_STATEMENT,        // TRY ... CATCH ... CLEANUP ... ENDTRY
    AST_CATCH_CLAUSE,         // CATCH в TRY
    AST_CLEANUP_CLAUSE,       // CLEANUP в TRY
    
    AST_MODULE_CALL,          // Вызов модуля (CALL MODULE)
    
    // Ошибки
    AST_ERROR_NODE            // Узел ошибки парсинга
    
} ast_node_type_t;

/**
 * Структура узла AST
 * Используется для построения синтаксического дерева.
 */
typedef struct ast_node {
    ast_node_type_t type;         // Тип узла
    token_t token;                // Токен, связанный с узлом (например, ключевое слово или идентификатор)
    
    struct ast_node **children;   // Массив указателей на дочерние узлы
    int children_count;           // Количество дочерних узлов
    
    // Дополнительные данные в зависимости от типа узла (например, тип переменной)
    void *data;
} ast_node_t;

/**
 * Структура парсера
 * Хранит текущее состояние парсинга, текущий токен и ссылку на лексер.
 */
typedef struct {
    lexer_t *lexer;               // Указатель на лексер
    token_t current_token;        // Текущий токен для анализа
    token_t peek_token;           // Следующий токен (lookahead)
    
    bool error_flag;              // Флаг ошибки парсинга
} parser_t;

/**
 * Инициализация парсера.
 * Принимает указатель на лексер и подготавливает внутреннее состояние.
 */
void parser_init(parser_t *parser, lexer_t *lexer);

/**
 * Основная функция парсинга программы.
 * Возвращает корневой узел AST программы.
 */
ast_node_t *parser_parse_program(parser_t *parser);

/**
 * Парсинг отдельных операторов и конструкций.
 * Каждая функция отвечает за парсинг конкретного синтаксического элемента.
 */
ast_node_t *parser_parse_statement(parser_t *parser);
ast_node_t *parser_parse_if_statement(parser_t *parser);
ast_node_t *parser_parse_loop_statement(parser_t *parser);
ast_node_t *parser_parse_while_statement(parser_t *parser);
ast_node_t *parser_parse_do_statement(parser_t *parser);
ast_node_t *parser_parse_perform_statement(parser_t *parser);
ast_node_t *parser_parse_function_call(parser_t *parser);
ast_node_t *parser_parse_assignment(parser_t *parser);
ast_node_t *parser_parse_expression(parser_t *parser);
ast_node_t *parser_parse_try_statement(parser_t *parser);

/**
 * Вспомогательные функции для управления токенами.
 */
void parser_next_token(parser_t *parser);
bool parser_expect_token(parser_t *parser, token_type_t expected);
bool parser_peek_token_is(parser_t *parser, token_type_t expected);

/**
 * Функция для обработки ошибок парсинга.
 * Устанавливает флаг ошибки и выводит сообщение.
 */
void parser_report_error(parser_t *parser, const char *message);

/**
 * Функция освобождения памяти AST.
 */
void ast_free(ast_node_t *node);

#endif // PARSER_H
