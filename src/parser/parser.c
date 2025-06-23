/*
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
*/

#include "parser.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/**
 * Инициализация парсера.
 * Устанавливает лексер, загружает два первых токена для lookahead.
 */
void parser_init(parser_t *parser, lexer_t *lexer) {
    parser->lexer = lexer;
    parser_next_token(parser);
    parser_next_token(parser);
    parser->error_flag = false;
}

/**
 * Перемещение на следующий токен.
 * Текущий токен становится peek_token, новый токен загружается из лексера.
 */
void parser_next_token(parser_t *parser) {
    parser->current_token = parser->peek_token;
    parser->peek_token = lexer_next_token(parser->lexer);
}

/**
 * Проверка, соответствует ли текущий токен ожидаемому типу.
 */
bool parser_expect_token(parser_t *parser, token_type_t expected) {
    if (parser->current_token.type == expected) {
        parser_next_token(parser);
        return true;
    }
    char err_msg[128];
    snprintf(err_msg, sizeof(err_msg), "Expected token type %d but found %d", expected, parser->current_token.type);
    parser_report_error(parser, err_msg);
    return false;
}

/**
 * Проверка типа peek токена.
 */
bool parser_peek_token_is(parser_t *parser, token_type_t expected) {
    return parser->peek_token.type == expected;
}

/**
 * Сообщение об ошибке парсинга.
 */
void parser_report_error(parser_t *parser, const char *message) {
    fprintf(stderr, "Parser error at line %d, col %d: %s\n",
            parser->current_token.line,
            parser->current_token.column,
            message);
    parser->error_flag = true;
}

/**
 * Создание нового AST узла.
 */
static ast_node_t *ast_node_new(ast_node_type_t type, token_t token) {
    ast_node_t *node = malloc(sizeof(ast_node_t));
    if (!node) {
        fprintf(stderr, "Out of memory allocating AST node\n");
        exit(1);
    }
    node->type = type;
    node->token = token;
    node->children = NULL;
    node->children_count = 0;
    node->data = NULL;
    return node;
}

/**
 * Добавление дочернего узла к родителю.
 */
static void ast_node_add_child(ast_node_t *parent, ast_node_t *child) {
    parent->children = realloc(parent->children, sizeof(ast_node_t *) * (parent->children_count + 1));
    if (!parent->children) {
        fprintf(stderr, "Out of memory reallocating AST children\n");
        exit(1);
    }
    parent->children[parent->children_count++] = child;
}

/**
 * Освобождение памяти AST дерева.
 */
void ast_free(ast_node_t *node) {
    if (!node) return;
    for (int i = 0; i < node->children_count; i++) {
        ast_free(node->children[i]);
    }
    free(node->children);
    if (node->data) {
        free(node->data);
    }
    free(node);
}

/**
 * Парсинг программы (корневой узел).
 * Предполагает последовательность операторов до конца файла.
 */
ast_node_t *parser_parse_program(parser_t *parser) {
    ast_node_t *program_node = ast_node_new(AST_PROGRAM, parser->current_token);
    while (parser->current_token.type != TOKEN_EOF && !parser->error_flag) {
        ast_node_t *stmt = parser_parse_statement(parser);
        if (stmt) {
            ast_node_add_child(program_node, stmt);
        } else {
            // Пропускаем ошибочный токен для попытки продолжения парсинга
            parser_next_token(parser);
        }
    }
    return program_node;
}

/**
 * Парсинг одного оператора (statement).
 * Распределяет по типам и вызывает соответствующую функцию.
 */
ast_node_t *parser_parse_statement(parser_t *parser) {
    switch (parser->current_token.type) {
        case TOKEN_IF:
            return parser_parse_if_statement(parser);
        case TOKEN_LOOP:
            return parser_parse_loop_statement(parser);
        case TOKEN_WHILE:
            return parser_parse_while_statement(parser);
        case TOKEN_DO:
            return parser_parse_do_statement(parser);
        case TOKEN_PERFORM:
            return parser_parse_perform_statement(parser);
        case TOKEN_CALL_FUNCTION:
            return parser_parse_function_call(parser);
        case TOKEN_DATA:
        case TOKEN_CONSTANTS:
        case TOKEN_TYPES:
        case TOKEN_FIELD_SYMBOLS:
        case TOKEN_SELECT_OPTIONS:
        case TOKEN_RANGES:
            // Для упрощения, можно парсить объявления как отдельный узел
            return parser_parse_declaration(parser);
        case TOKEN_ASSIGN:
            return parser_parse_assignment(parser);
        case TOKEN_TRY:
            return parser_parse_try_statement(parser);
        case TOKEN_EXIT:
            {
                ast_node_t *node = ast_node_new(AST_EXIT_STATEMENT, parser->current_token);
                parser_next_token(parser);
                return node;
            }
        case TOKEN_CONTINUE:
            {
                ast_node_t *node = ast_node_new(AST_CONTINUE_STATEMENT, parser->current_token);
                parser_next_token(parser);
                return node;
            }
        case TOKEN_CHECK:
            {
                ast_node_t *node = ast_node_new(AST_CHECK_STATEMENT, parser->current_token);
                parser_next_token(parser);
                return node;
            }
        case TOKEN_RETURN:
            {
                ast_node_t *node = ast_node_new(AST_RETURN_STATEMENT, parser->current_token);
                parser_next_token(parser);
                return node;
            }
        default:
            parser_report_error(parser, "Unknown statement or unexpected token");
            return NULL;
    }
}

/**
 * Парсинг IF оператора.
 * Пример структуры:
 * IF <expression>.
 *   <statements>
 * ELSEIF <expression>.
 *   <statements>
 * ELSE.
 *   <statements>
 * ENDIF.
 */
ast_node_t *parser_parse_if_statement(parser_t *parser) {
    if (parser->current_token.type != TOKEN_IF) {
        parser_report_error(parser, "Expected IF");
        return NULL;
    }

    ast_node_t *if_node = ast_node_new(AST_IF_STATEMENT, parser->current_token);
    parser_next_token(parser);

    // Парсим условие (выражение)
    ast_node_t *condition = parser_parse_expression(parser);
    if (!condition) {
        parser_report_error(parser, "Invalid IF condition");
        return NULL;
    }
    ast_node_add_child(if_node, condition);

    // Парсим тело IF
    while (parser->current_token.type != TOKEN_ELSEIF &&
           parser->current_token.type != TOKEN_ELSE &&
           parser->current_token.type != TOKEN_ENDIF &&
           parser->current_token.type != TOKEN_EOF) {
        ast_node_t *stmt = parser_parse_statement(parser);
        if (stmt) {
            ast_node_add_child(if_node, stmt);
        } else {
            parser_next_token(parser);
        }
    }

    // Парсим блоки ELSEIF
    while (parser->current_token.type == TOKEN_ELSEIF) {
        token_t elseif_token = parser->current_token;
        parser_next_token(parser);
        ast_node_t *elseif_node = ast_node_new(AST_ELSEIF_CLAUSE, elseif_token);
        ast_node_t *elseif_condition = parser_parse_expression(parser);
        if (!elseif_condition) {
            parser_report_error(parser, "Invalid ELSEIF condition");
            ast_free(if_node);
            return NULL;
        }
        ast_node_add_child(elseif_node, elseif_condition);

        // Тело ELSEIF
        while (parser->current_token.type != TOKEN_ELSEIF &&
               parser->current_token.type != TOKEN_ELSE &&
               parser->current_token.type != TOKEN_ENDIF &&
               parser->current_token.type != TOKEN_EOF) {
            ast_node_t *stmt = parser_parse_statement(parser);
            if (stmt) {
                ast_node_add_child(elseif_node, stmt);
            } else {
                parser_next_token(parser);
            }
        }

        ast_node_add_child(if_node, elseif_node);
    }

    // Парсим ELSE
    if (parser->current_token.type == TOKEN_ELSE) {
        token_t else_token = parser->current_token;
        parser_next_token(parser);
        ast_node_t *else_node = ast_node_new(AST_ELSE_CLAUSE, else_token);

        while (parser->current_token.type != TOKEN_ENDIF &&
               parser->current_token.type != TOKEN_EOF) {
            ast_node_t *stmt = parser_parse_statement(parser);
            if (stmt) {
                ast_node_add_child(else_node, stmt);
            } else {
                parser_next_token(parser);
            }
        }

        ast_node_add_child(if_node, else_node);
    }

    // Ожидаем ENDIF
    if (!parser_expect_token(parser, TOKEN_ENDIF)) {
        parser_report_error(parser, "Expected ENDIF");
        ast_free(if_node);
        return NULL;
    }

    return if_node;
}

/**
 * Парсинг выражения.
 * Для упрощения, здесь реализована заглушка, которая просто создает узел литерала или идентификатора.
 * В реальном парсере тут будет рекурсивный спуск с приоритетами операторов.
 */
ast_node_t *parser_parse_expression(parser_t *parser) {
    token_t tok = parser->current_token;
    switch (tok.type) {
        case TOKEN_IDENTIFIER:
        case TOKEN_INTEGER_LITERAL:
        case TOKEN_STRING_LITERAL: {
            ast_node_t *expr_node = ast_node_new(AST_LITERAL, tok);
            parser_next_token(parser);
            return expr_node;
        }
        default:
            parser_report_error(parser, "Expected expression");
            return NULL;
    }
}

/**
 * Заглушка для парсинга объявления.
 */
ast_node_t *parser_parse_declaration(parser_t *parser) {
    // Для примера просто создаём узел DATA_DECLARATION и сдвигаем токен
    ast_node_t *decl_node = ast_node_new(AST_DATA_DECLARATION, parser->current_token);
    parser_next_token(parser);
    return decl_node;
}

/**
 * Заглушка для парсинга цикла LOOP.
 */
ast_node_t *parser_parse_loop_statement(parser_t *parser) {
    ast_node_t *loop_node = ast_node_new(AST_LOOP_STATEMENT, parser->current_token);
    parser_next_token(parser);
    // Тут должен быть разбор тела цикла, условие и т.д.
    return loop_node;
}

/**
 * Заглушка для парсинга WHILE.
 */
ast_node_t *parser_parse_while_statement(parser_t *parser) {
    ast_node_t *while_node = ast_node_new(AST_WHILE_STATEMENT, parser->current_token);
    parser_next_token(parser);
    return while_node;
}

/**
 * Заглушка для парсинга DO.
 */
ast_node_t *parser_parse_do_statement(parser_t *parser) {
    ast_node_t *do_node = ast_node_new(AST_DO_STATEMENT, parser->current_token);
    parser_next_token(parser);
    return do_node;
}

/**
 * Заглушка для парсинга PERFORM.
 */
ast_node_t *parser_parse_perform_statement(parser_t *parser) {
    ast_node_t *perform_node = ast_node_new(AST_PERFORM_STATEMENT, parser->current_token);
    parser_next_token(parser);
    return perform_node;
}

/**
 * Заглушка для парсинга CALL FUNCTION.
 */
ast_node_t *parser_parse_function_call(parser_t *parser) {
    ast_node_t *call_node = ast_node_new(AST_FUNCTION_CALL, parser->current_token);
    parser_next_token(parser);
    return call_node;
}

/**
 * Заглушка для парсинга присваивания.
 */
ast_node_t *parser_parse_assignment(parser_t *parser) {
    ast_node_t *assign_node = ast_node_new(AST_ASSIGNMENT, parser->current_token);
    parser_next_token(parser);
    return assign_node;
}

/**
 * Заглушка для парсинга TRY ... CATCH ... ENDTRY.
 */
ast_node_t *parser_parse_try_statement(parser_t *parser) {
    ast_node_t *try_node = ast_node_new(AST_TRY_STATEMENT, parser->current_token);
    parser_next_token(parser);
    return try_node;
}
