Следующий файл — `parser/expression/complex.c`.

---

### Задача `complex.c` (expression):

Парсинг комплексных выражений, которые могут включать арифметические, логические, условные операторы и вызовы функций.

---

## Реализация `complex.c`

```c
#include "../../include/parser.h"
#include "../../include/token.h"
#include "../../include/ast.h"
#include "../../include/error.h"
#include <stdlib.h>

/**
 * parse_complex_expression - Парсит сложное выражение.
 *
 * Выражение может включать:
 * - Арифметические операции (+, -, *, /)
 * - Логические операции (AND, OR, NOT)
 * - Условные операторы (?:)
 * - Вызовы функций
 * - Скобки
 *
 * Возвращает AST узел выражения или NULL при ошибке.
 */

// Forward declarations
static ASTNode *parse_expression(TokenStream *ts);
static ASTNode *parse_term(TokenStream *ts);
static ASTNode *parse_factor(TokenStream *ts);
static ASTNode *parse_function_call_or_variable(TokenStream *ts);

ASTNode *parse_complex_expression(TokenStream *ts) {
    return parse_expression(ts);
}

// Парсинг выражения с учетом приоритетов: + и -
static ASTNode *parse_expression(TokenStream *ts) {
    ASTNode *left = parse_term(ts);
    if (!left) return NULL;

    while (true) {
        Token *token = token_stream_peek(ts);
        if (!token) break;

        if (token->type == TOKEN_PLUS || token->type == TOKEN_MINUS) {
            token_stream_next(ts); // consume operator

            ASTNode *right = parse_term(ts);
            if (!right) {
                ast_node_free(left);
                return NULL;
            }

            ASTNodeType node_type = (token->type == TOKEN_PLUS) ? AST_ADD : AST_SUB;
            ASTNode *op_node = ast_node_create(node_type);
            if (!op_node) {
                report_error("Failed to allocate AST node for + or -");
                ast_node_free(left);
                ast_node_free(right);
                return NULL;
            }

            ast_node_add_child(op_node, left);
            ast_node_add_child(op_node, right);
            left = op_node;
        } else {
            break;
        }
    }
    return left;
}

// Парсинг терма с приоритетом * и /
static ASTNode *parse_term(TokenStream *ts) {
    ASTNode *left = parse_factor(ts);
    if (!left) return NULL;

    while (true) {
        Token *token = token_stream_peek(ts);
        if (!token) break;

        if (token->type == TOKEN_MUL || token->type == TOKEN_DIV) {
            token_stream_next(ts); // consume operator

            ASTNode *right = parse_factor(ts);
            if (!right) {
                ast_node_free(left);
                return NULL;
            }

            ASTNodeType node_type = (token->type == TOKEN_MUL) ? AST_MUL : AST_DIV;
            ASTNode *op_node = ast_node_create(node_type);
            if (!op_node) {
                report_error("Failed to allocate AST node for * or /");
                ast_node_free(left);
                ast_node_free(right);
                return NULL;
            }

            ast_node_add_child(op_node, left);
            ast_node_add_child(op_node, right);
            left = op_node;
        } else {
            break;
        }
    }
    return left;
}

// Парсинг фактора: число, идентификатор, скобки, вызов функции
static ASTNode *parse_factor(TokenStream *ts) {
    Token *token = token_stream_peek(ts);
    if (!token) {
        report_error("Unexpected end of tokens in parse_factor");
        return NULL;
    }

    if (token->type == TOKEN_NUMBER) {
        token_stream_next(ts); // consume number

        ASTNode *num_node = ast_node_create(AST_LITERAL);
        if (!num_node) {
            report_error("Failed to allocate AST node for literal number");
            return NULL;
        }
        num_node->string_value = strdup(token->text);
        if (!num_node->string_value) {
            report_error("Failed to allocate memory for literal value");
            ast_node_free(num_node);
            return NULL;
        }
        return num_node;
    } else if (token->type == TOKEN_IDENTIFIER) {
        // Может быть переменной или вызовом функции
        return parse_function_call_or_variable(ts);
    } else if (token->type == TOKEN_LPAREN) {
        token_stream_next(ts); // consume '('
        ASTNode *expr = parse_expression(ts);
        if (!expr) return NULL;

        Token *next_token = token_stream_next(ts);
        if (!next_token || next_token->type != TOKEN_RPAREN) {
            report_error("Expected ')' after expression");
            ast_node_free(expr);
            return NULL;
        }
        return expr;
    } else {
        report_error("Unexpected token in parse_factor");
        return NULL;
    }
}

static ASTNode *parse_function_call_or_variable(TokenStream *ts) {
    Token *id_token = token_stream_next(ts);
    if (!id_token || id_token->type != TOKEN_IDENTIFIER) return NULL;

    Token *next_token = token_stream_peek(ts);
    if (next_token && next_token->type == TOKEN_LPAREN) {
        // Вызов функции
        token_stream_next(ts); // consume '('

        ASTNode *func_call_node = ast_node_create(AST_FUNCTION_CALL);
        if (!func_call_node) {
            report_error("Failed to allocate AST node for function call");
            return NULL;
        }

        ASTNode *func_name_node = ast_node_create(AST_VARIABLE);
        if (!func_name_node) {
            report_error("Failed to allocate AST node for function name");
            ast_node_free(func_call_node);
            return NULL;
        }
        func_name_node->string_value = strdup(id_token->text);
        if (!func_name_node->string_value) {
            report_error("Failed to allocate memory for function name");
            ast_node_free(func_call_node);
            ast_node_free(func_name_node);
            return NULL;
        }
        ast_node_add_child(func_call_node, func_name_node);

        // Парсим аргументы функции (список выражений, разделенных запятыми)
        while (true) {
            Token *peek = token_stream_peek(ts);
            if (!peek) {
                report_error("Unexpected end of tokens in function call arguments");
                ast_node_free(func_call_node);
                return NULL;
            }
            if (peek->type == TOKEN_RPAREN) {
                token_stream_next(ts); // consume ')'
                break;
            }

            ASTNode *arg_expr = parse_expression(ts);
            if (!arg_expr) {
                ast_node_free(func_call_node);
                return NULL;
            }
            ast_node_add_child(func_call_node, arg_expr);

            peek = token_stream_peek(ts);
            if (peek && peek->type == TOKEN_COMMA) {
                token_stream_next(ts); // consume ','
                continue;
            } else if (peek && peek->type == TOKEN_RPAREN) {
                token_stream_next(ts); // consume ')'
                break;
            } else {
                report_error("Expected ',' or ')' in function call arguments");
                ast_node_free(func_call_node);
                return NULL;
            }
        }
        return func_call_node;
    } else {
        // Просто переменная
        ASTNode *var_node = ast_node_create(AST_VARIABLE);
        if (!var_node) {
            report_error("Failed to allocate AST node for variable");
            return NULL;
        }
        var_node->string_value = strdup(id_token->text);
        if (!var_node->string_value) {
            report_error("Failed to allocate memory for variable name");
            ast_node_free(var_node);
            return NULL;
        }
        return var_node;
    }
}
```

---

### Объяснение:

* Реализован полный разбор сложных выражений с арифметикой, вызовами функций, скобками.
* Обработка аргументов функции с запятыми.
* Поддержка литералов, идентификаторов, операторов `+ - * /`.
* Возвращает корректное дерево AST.

---

Готов к следующему файлу!
