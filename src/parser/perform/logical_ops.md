Следующий файл — `parser/perform/logical_ops.c`.

---

### Задача `logical_ops.c` для perform:

Парсинг логических операций в контексте PERFORM, например логических выражений в параметрах или условиях.

---

## Реализация `logical_ops.c`

```c
#include "../../include/parser.h"
#include "../../include/token.h"
#include "../../include/ast.h"
#include "../../include/error.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * parse_perform_logical_ops - Парсит логические операции внутри PERFORM.
 *
 * Поддерживаются операторы AND, OR, NOT.
 *
 * Возвращает AST узел логического выражения или NULL при ошибке.
 */
ASTNode *parse_perform_logical_ops(TokenStream *ts) {
    if (!ts) {
        report_error("TokenStream is NULL in parse_perform_logical_ops");
        return NULL;
    }

    // Начинаем с парсинга простого выражения
    ASTNode *left = parse_simple_expression(ts);
    if (!left) {
        report_error("Failed to parse left operand in logical operation");
        return NULL;
    }

    while (1) {
        Token *token = token_stream_peek(ts);
        if (!token) break;

        if (token->type == TOKEN_KEYWORD_AND || token->type == TOKEN_KEYWORD_OR) {
            token_stream_next(ts); // съесть оператор
            ASTNode *right = parse_simple_expression(ts);
            if (!right) {
                report_error("Failed to parse right operand in logical operation");
                ast_node_free(left);
                return NULL;
            }

            ASTNode *logical_node = ast_node_create(AST_LOGICAL_OP);
            if (!logical_node) {
                report_error("Failed to create AST node for logical operation");
                ast_node_free(left);
                ast_node_free(right);
                return NULL;
            }

            logical_node->logical_op_type = (token->type == TOKEN_KEYWORD_AND) ? LOGICAL_AND : LOGICAL_OR;
            logical_node->left = left;
            logical_node->right = right;

            left = logical_node; // Новый корень выражения
        } else if (token->type == TOKEN_KEYWORD_NOT) {
            token_stream_next(ts); // съесть NOT
            ASTNode *not_operand = parse_simple_expression(ts);
            if (!not_operand) {
                report_error("Failed to parse operand for NOT");
                ast_node_free(left);
                return NULL;
            }
            ASTNode *not_node = ast_node_create(AST_LOGICAL_NOT);
            if (!not_node) {
                report_error("Failed to create AST node for NOT operation");
                ast_node_free(not_operand);
                ast_node_free(left);
                return NULL;
            }
            not_node->operand = not_operand;

            // Обновляем левую часть как NOT выражение
            if (left) ast_node_free(left);
            left = not_node;
        } else {
            break;
        }
    }

    return left;
}
```

---

### Объяснение:

* Парсит цепочки логических операций AND, OR, NOT.
* Использует рекурсивный подход для построения дерева AST.
* Основан на разборе простых выражений (предполагается, что `parse_simple_expression` реализован).

---

Готов к следующему файлу!
