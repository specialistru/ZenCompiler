Переходим к следующему файлу — `parser/if/logical_ops.c`.

---

### Задача `logical_ops.c`:

Реализовать парсер логических операторов (AND, OR) в условиях IF/ELSEIF.

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
 * parse_logical_expression - Парсит логические выражения с операторами AND и OR.
 *
 * Возвращает AST узел, представляющий логическое выражение,
 * или NULL при ошибке.
 */
ASTNode *parse_logical_expression(TokenStream *ts) {
    if (!ts) {
        fprintf(stderr, "TokenStream is NULL\n");
        return NULL;
    }

    // Парсим левый операнд (например, простое выражение)
    ASTNode *left = parse_expression(ts);
    if (!left) {
        report_error("Failed to parse left operand of logical expression");
        return NULL;
    }

    while (1) {
        Token *token = token_stream_peek(ts);
        if (!token) {
            break;
        }

        // Проверяем, есть ли логический оператор
        if (token->type == TOKEN_KEYWORD_AND || token->type == TOKEN_KEYWORD_OR) {
            token_stream_next(ts); // съесть оператор

            // Создаем новый AST узел для логического оператора
            ASTNode *logical_node = ast_node_create(AST_LOGICAL_OP);
            if (!logical_node) {
                report_error("Failed to create AST node for logical operator");
                ast_node_free(left);
                return NULL;
            }
            logical_node->logical_op_type = (token->type == TOKEN_KEYWORD_AND) ? LOGICAL_OP_AND : LOGICAL_OP_OR;
            logical_node->left = left;

            // Парсим правый операнд
            ASTNode *right = parse_expression(ts);
            if (!right) {
                report_error("Failed to parse right operand of logical expression");
                ast_node_free(logical_node);
                return NULL;
            }
            logical_node->right = right;

            // Теперь новый левый узел — это логический оператор с левым и правым подузлами
            left = logical_node;
        } else {
            break; // нет логического оператора — выходим
        }
    }

    return left;
}
```

---

### Объяснение:

* Парсим левый операнд.
* Пока следующий токен — AND или OR, создаём AST узел с этим оператором.
* Рекурсивно парсим правый операнд.
* Строим дерево логических операций.

---

Если хочешь, могу взять следующий файл из списка!
