#include "parser.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void parser_advance(Parser *parser);
static ASTNode* parse_statement(Parser *parser);
static ASTNode* parse_expression(Parser *parser);
static ASTNode* create_ast_node(ASTNodeType type, Token token);

// Инициализация парсера с указателем на лексер
void parser_init(Parser *parser, Lexer *lexer) {
    parser->lexer = lexer;
    parser_advance(parser);
}

// Получить следующий токен
static void parser_advance(Parser *parser) {
    parser->current_token = lexer_next_token(parser->lexer);
}

// Парсинг программы (простейшая реализация: последовательность операторов)
ASTNode* parser_parse_program(Parser *parser) {
    ASTNode *program_node = create_ast_node(AST_NODE_PROGRAM, parser->current_token);
    program_node->children = NULL;
    program_node->child_count = 0;

    while (parser->current_token.type != TOKEN_EOF) {
        ASTNode *stmt = parse_statement(parser);
        if (stmt) {
            program_node->children = realloc(program_node->children, sizeof(ASTNode*) * (program_node->child_count + 1));
            program_node->children[program_node->child_count++] = stmt;
        } else {
            fprintf(stderr, "Parsing error at line %d, column %d\n", parser->current_token.line, parser->current_token.column);
            break;
        }
    }

    return program_node;
}

// Парсинг отдельного оператора (очень упрощенно)
static ASTNode* parse_statement(Parser *parser) {
    // Для примера разберём только простейшее выражение: идентификатор или ключевое слово
    Token token = parser->current_token;

    if (token.type == TOKEN_KEYWORD) {
        // Например, поддерживаем IF ... ENDIF
        if (strncasecmp(token.text, "IF", token.length) == 0) {
            parser_advance(parser);
            ASTNode *expr = parse_expression(parser);
            if (!expr) return NULL;

            ASTNode *if_node = create_ast_node(AST_NODE_STATEMENT, token);
            if_node->children = malloc(sizeof(ASTNode*) * 1);
            if_node->children[0] = expr;
            if_node->child_count = 1;

            // Здесь можно добавить разбор тела if, else и endif — упрощаем

            // Пропускаем ENDIF
            while (parser->current_token.type != TOKEN_EOF && 
                   !(parser->current_token.type == TOKEN_KEYWORD && 
                     strncasecmp(parser->current_token.text, "ENDIF", parser->current_token.length) == 0)) {
                parser_advance(parser);
            }
            if (parser->current_token.type == TOKEN_KEYWORD) {
                parser_advance(parser); // пропустить ENDIF
            }
            return if_node;
        }

        // Другие ключевые слова - заглушка
        parser_advance(parser);
        return create_ast_node(AST_NODE_STATEMENT, token);
    }

    if (token.type == TOKEN_IDENTIFIER) {
        parser_advance(parser);
        return create_ast_node(AST_NODE_STATEMENT, token);
    }

    // Ошибка: неизвестный токен
    return NULL;
}

// Парсинг выражения — упрощённая версия
static ASTNode* parse_expression(Parser *parser) {
    Token token = parser->current_token;
    if (token.type == TOKEN_IDENTIFIER || token.type == TOKEN_LITERAL_NUMBER || token.type == TOKEN_LITERAL_STRING) {
        parser_advance(parser);
        return create_ast_node(AST_NODE_EXPRESSION, token);
    }
    return NULL;
}

// Создание AST-узла
static ASTNode* create_ast_node(ASTNodeType type, Token token) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = type;
    node->token = token;
    node->children = NULL;
    node->child_count = 0;
    return node;
}

// Освобождение AST из памяти
void parser_free_ast(ASTNode *node) {
    if (!node) return;
    for (int i = 0; i < node->child_count; i++) {
        parser_free_ast(node->children[i]);
    }
    free(node->children);
    free(node);
}


////////////////////////////

#include "parser.h"
#include "lexer.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// Вспомогательные функции объявления
static Token parser_peek(Parser *parser);
static Token parser_consume(Parser *parser, TokenType expected_type);
static void parser_error(Parser *parser, const char *message);

// Инициализация парсера
void parser_init(Parser *parser, Lexer *lexer) {
    parser->lexer = lexer;
    parser->current_token = lexer_next_token(lexer);
}

// Проверка следующего токена
static Token parser_peek(Parser *parser) {
    return parser->current_token;
}

// Проверка и потребление ожидаемого токена
static Token parser_consume(Parser *parser, TokenType expected_type) {
    Token token = parser->current_token;
    if (token.type != expected_type) {
        char error_msg[128];
        snprintf(error_msg, sizeof(error_msg), "Expected token type %d but got %d at line %d column %d",
                 expected_type, token.type, token.line, token.column);
        parser_error(parser, error_msg);
    }
    parser->current_token = lexer_next_token(parser->lexer);
    return token;
}

// Обработка ошибки парсинга (здесь просто вывод и завершение)
static void parser_error(Parser *parser, const char *message) {
    fprintf(stderr, "Parser error: %s\n", message);
    exit(EXIT_FAILURE);
}

// Пример функции парсинга условного оператора IF
ASTNode *parse_if_statement(Parser *parser) {
    ASTNode *node = malloc(sizeof(ASTNode));
    if (!node) {
        fprintf(stderr, "Out of memory\n");
        exit(EXIT_FAILURE);
    }
    node->type = AST_IF_STATEMENT;
    node->line = parser->current_token.line;

    // Потребляем ключевое слово IF
    parser_consume(parser, TOKEN_KEYWORD_IF);

    // Парсим условие (упрощенно — ожидаем идентификатор)
    node->condition = malloc(sizeof(ASTNode));
    node->condition->type = AST_IDENTIFIER;
    node->condition->token = parser_consume(parser, TOKEN_IDENTIFIER);

    // Здесь могла бы быть рекурсивная обработка тела IF (пропущено для простоты)
    // ...

    // Потребляем ENDIF
    parser_consume(parser, TOKEN_KEYWORD_ENDIF);

    return node;
}

// Главная функция парсинга
ASTNode *parser_parse(Parser *parser) {
    Token token = parser_peek(parser);

    switch (token.type) {
        case TOKEN_KEYWORD_IF:
            return parse_if_statement(parser);
        // Добавьте обработку других конструкций по мере необходимости
        default:
            parser_error(parser, "Unexpected token at start of statement");
            return NULL;
    }
}
