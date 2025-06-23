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
