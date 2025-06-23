Следующий файл — `parser/expression/operator.c`.

---

### Задача `operator.c` (expression):

Парсинг операторов в выражениях (арифметические, логические, сравнения).

---

## Реализация `operator.c`

```c
#include "../../include/parser.h"
#include "../../include/token.h"
#include "../../include/ast.h"
#include "../../include/error.h"
#include <stdlib.h>

/**
 * parse_operator - Парсит оператор из токенов.
 *
 * Поддерживаются операторы: +, -, *, /, AND, OR, =, <>, >, <, >=, <= и другие.
 *
 * Возвращает AST узел оператора или NULL при ошибке.
 */
ASTNode *parse_operator(TokenStream *ts) {
    if (!ts) {
        report_error("TokenStream is NULL in parse_operator");
        return NULL;
    }

    Token *token = token_stream_next(ts);
    if (!token) {
        report_error("Unexpected end of tokens while parsing operator");
        return NULL;
    }

    ASTNodeType node_type;
    switch (token->type) {
        case TOKEN_PLUS:
            node_type = AST_OPERATOR_PLUS;
            break;
        case TOKEN_MINUS:
            node_type = AST_OPERATOR_MINUS;
            break;
        case TOKEN_ASTERISK:
            node_type = AST_OPERATOR_MULTIPLY;
            break;
        case TOKEN_SLASH:
            node_type = AST_OPERATOR_DIVIDE;
            break;
        case TOKEN_KEYWORD_AND:
            node_type = AST_OPERATOR_AND;
            break;
        case TOKEN_KEYWORD_OR:
            node_type = AST_OPERATOR_OR;
            break;
        case TOKEN_EQUAL:
            node_type = AST_OPERATOR_EQUAL;
            break;
        case TOKEN_NOT_EQUAL:
            node_type = AST_OPERATOR_NOT_EQUAL;
            break;
        case TOKEN_GREATER:
            node_type = AST_OPERATOR_GREATER;
            break;
        case TOKEN_LESS:
            node_type = AST_OPERATOR_LESS;
            break;
        case TOKEN_GREATER_EQUAL:
            node_type = AST_OPERATOR_GREATER_EQUAL;
            break;
        case TOKEN_LESS_EQUAL:
            node_type = AST_OPERATOR_LESS_EQUAL;
            break;
        default:
            report_error("Unexpected token type for operator");
            return NULL;
    }

    ASTNode *operator_node = ast_node_create(node_type);
    if (!operator_node) {
        report_error("Failed to allocate AST node for operator");
        return NULL;
    }

    return operator_node;
}
```

---

### Объяснение:

* Парсит отдельный оператор из потока токенов.
* Поддерживает арифметические, логические и сравнения.
* Возвращает соответствующий AST узел.

---

Готов продолжить!
