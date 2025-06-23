Следующий файл — `parser/perform/and_or.c`.

---

### Задача `and_or.c` для perform:

Парсинг логических операций AND и OR, с учётом приоритетов и составных выражений в контексте PERFORM.

---

## Реализация `and_or.c`

```c
#include "../../include/parser.h"
#include "../../include/token.h"
#include "../../include/ast.h"
#include "../../include/error.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * parse_perform_and_or - Парсит выражения с логическими операциями AND и OR,
 * учитывая приоритеты операторов.
 *
 * Используется рекурсивный парсинг с разбиением на уровни приоритета.
 *
 * Возвращает AST узел или NULL при ошибке.
 */

static ASTNode *parse_perform_and_or_expr(TokenStream *ts, int min_prec);

/**
 * parse_perform_logical_operand - Парсит операнд логического выражения.
 */
static ASTNode *parse_perform_logical_operand(TokenStream *ts) {
    Token *token = token_stream_peek(ts);
    if (!token) return NULL;

    if (token->type == TOKEN_KEYWORD_NOT) {
        token_stream_next(ts);
        ASTNode *operand = parse_perform_logical_operand(ts);
        if (!operand) {
            report_error("Failed to parse operand after NOT");
            return NULL;
        }
        ASTNode *not_node = ast_node_create(AST_LOGICAL_NOT);
        if (!not_node) {
            report_error("Failed to create AST node for NOT");
            ast_node_free(operand);
            return NULL;
        }
        not_node->operand = operand;
        return not_node;
    } else if (token->type == TOKEN_LPAREN) {
        token_stream_next(ts); // съесть '('
        ASTNode *expr = parse_perform_and_or_expr(ts, 0);
        if (!expr) return NULL;
        token = token_stream_next(ts);
        if (!token || token->type != TOKEN_RPAREN) {
            report_error("Expected ')' after expression");
            ast_node_free(expr);
            return NULL;
        }
        return expr;
    } else {
        // Предполагается простой идентификатор или литерал
        return parse_simple_expression(ts);
    }
}

static int get_precedence(TokenType type) {
    if (type == TOKEN_KEYWORD_AND) return 2;
    if (type == TOKEN_KEYWORD_OR) return 1;
    return 0;
}

static ASTNode *parse_perform_and_or_expr(TokenStream *ts, int min_prec) {
    ASTNode *lhs = parse_perform_logical_operand(ts);
    if (!lhs) return NULL;

    while (1) {
        Token *token = token_stream_peek(ts);
        if (!token) break;

        int prec = get_precedence(token->type);
        if (prec < min_prec) break;

        TokenType op_type = token->type;
        token_stream_next(ts);

        ASTNode *rhs = parse_perform_and_or_expr(ts, prec + 1);
        if (!rhs) {
            ast_node_free(lhs);
            return NULL;
        }

        ASTNode *op_node = ast_node_create(AST_LOGICAL_OP);
        if (!op_node) {
            report_error("Failed to create AST node for logical operator");
            ast_node_free(lhs);
            ast_node_free(rhs);
            return NULL;
        }
        op_node->logical_op_type = (op_type == TOKEN_KEYWORD_AND) ? LOGICAL_AND : LOGICAL_OR;
        op_node->left = lhs;
        op_node->right = rhs;

        lhs = op_node;
    }

    return lhs;
}

/**
 * Основная точка входа
 */
ASTNode *parse_perform_and_or(TokenStream *ts) {
    return parse_perform_and_or_expr(ts, 0);
}
```

---

### Объяснение:

* Реализован парсер логических выражений с учетом приоритетов AND > OR.
* Использован подход парсинга с помощью предшествующей рекурсии.
* Поддерживает скобочные выражения и NOT.
* Предполагается наличие `parse_simple_expression`.

---

Готов к следующему файлу!
