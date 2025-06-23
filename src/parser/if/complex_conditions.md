Следующий файл — `parser/if/complex_conditions.c`.

---

### Задача `complex_conditions.c`:

Реализовать парсер для сложных логических условий в операторе IF, поддерживающий составные выражения с операторами AND, OR, NOT и скобками.

---

## Реализация `complex_conditions.c`

```c
#include "../../include/parser.h"
#include "../../include/token.h"
#include "../../include/ast.h"
#include "../../include/error.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * parse_complex_condition - Парсит сложное логическое условие для IF.
 *
 * Поддерживаются операторы AND, OR, NOT, скобки, простые условия.
 *
 * Возвращает AST узел условия или NULL при ошибке.
 */
ASTNode *parse_complex_condition(TokenStream *ts);

/**
 * parse_primary_condition - Парсит базовое условие:
 * - скобочное выражение ( )
 * - логический оператор NOT
 * - простое выражение (например, идентификатор или литерал)
 */
static ASTNode *parse_primary_condition(TokenStream *ts) {
    Token *token = token_stream_peek(ts);
    if (!token) {
        report_error("Unexpected end of input in condition");
        return NULL;
    }

    if (token->type == TOKEN_LPAREN) {
        // Скобочное выражение
        token_stream_next(ts); // съесть '('
        ASTNode *expr = parse_complex_condition(ts);
        if (!expr) return NULL;

        token = token_stream_next(ts);
        if (!token || token->type != TOKEN_RPAREN) {
            report_error("Expected ')' after expression");
            ast_node_free(expr);
            return NULL;
        }
        return expr;
    } else if (token->type == TOKEN_KEYWORD_NOT) {
        // Логический NOT
        token_stream_next(ts); // съесть NOT
        ASTNode *operand = parse_primary_condition(ts);
        if (!operand) return NULL;

        ASTNode *not_node = ast_node_create(AST_NOT_OP);
        if (!not_node) {
            report_error("Failed to create AST node for NOT");
            ast_node_free(operand);
            return NULL;
        }
        not_node->child = operand;
        return not_node;
    } else {
        // Простое выражение
        return parse_simple_condition(ts);
    }
}

/**
 * parse_binary_condition - Парсит бинарные операторы AND/OR с учетом приоритета.
 * @lhs: левая часть выражения
 * @ts: поток токенов
 *
 * Возвращает AST узел или NULL.
 */
static ASTNode *parse_binary_condition(ASTNode *lhs, TokenStream *ts) {
    while (1) {
        Token *token = token_stream_peek(ts);
        if (!token) break;

        ASTNodeType op_type;
        if (token->type == TOKEN_KEYWORD_AND) {
            op_type = AST_AND_OP;
        } else if (token->type == TOKEN_KEYWORD_OR) {
            op_type = AST_OR_OP;
        } else {
            break; // оператор не AND/OR
        }

        token_stream_next(ts); // съесть оператор

        ASTNode *rhs = parse_primary_condition(ts);
        if (!rhs) {
            report_error("Failed to parse right operand of binary condition");
            ast_node_free(lhs);
            return NULL;
        }

        ASTNode *bin_node = ast_node_create(op_type);
        if (!bin_node) {
            report_error("Failed to create AST node for binary operator");
            ast_node_free(lhs);
            ast_node_free(rhs);
            return NULL;
        }

        bin_node->left = lhs;
        bin_node->right = rhs;
        lhs = bin_node;
    }
    return lhs;
}

ASTNode *parse_complex_condition(TokenStream *ts) {
    ASTNode *lhs = parse_primary_condition(ts);
    if (!lhs) return NULL;

    return parse_binary_condition(lhs, ts);
}
```

---

### Объяснение:

* Парсит вложенные условия с поддержкой скобок и логического NOT.
* Поддерживает бинарные операторы AND и OR с левым приоритетом.
* Вызывает `parse_simple_condition` для базовых условий (должна быть реализована отдельно).

---

Готов перейти к следующему файлу!
