Следующий файл — `parser/module/chain_and_field.c`.

---

### Задача `chain_and_field.c` (module):

Парсинг цепочек вызовов и операций с полями (chain & field access) в модулях ABAP, например, обработка выражений с цепочками вызовов методов, обращений к вложенным структурам и т.п.

---

## Реализация `chain_and_field.c`

```c
#include "../../include/parser.h"
#include "../../include/token.h"
#include "../../include/ast.h"
#include "../../include/error.h"
#include <stdlib.h>
#include <string.h>

/**
 * parse_chain_and_field_access - Парсит цепочки вызовов и обращения к полям.
 *
 * Пример:
 *  object->method()->field-subfield
 *
 * Возвращает AST узел, представляющий цепочку вызовов и полей.
 * В случае ошибки возвращает NULL.
 */

ASTNode *parse_chain_and_field_access(TokenStream *ts) {
    if (!ts) {
        report_error("TokenStream is NULL in parse_chain_and_field_access");
        return NULL;
    }

    ASTNode *root = NULL;
    ASTNode *current = NULL;

    // Первый элемент — идентификатор или выражение
    Token *token = token_stream_next(ts);
    if (!token || token->type != TOKEN_IDENTIFIER) {
        report_error("Expected identifier at start of chain or field access");
        return NULL;
    }

    root = ast_node_create(AST_CHAIN);
    if (!root) {
        report_error("Failed to allocate AST node for chain");
        return NULL;
    }

    current = ast_node_create(AST_IDENTIFIER);
    if (!current) {
        report_error("Failed to allocate AST node for identifier");
        ast_node_free(root);
        return NULL;
    }

    current->string_value = strdup(token->text);
    if (!current->string_value) {
        report_error("Failed to allocate memory for identifier string");
        ast_node_free(root);
        ast_node_free(current);
        return NULL;
    }

    ast_node_add_child(root, current);

    // Разбор последовательности ->method(), -field и т.п.
    while (1) {
        Token *next = token_stream_peek(ts);
        if (!next) break;

        if (next->type == TOKEN_ARROW) {
            token_stream_next(ts); // consume '->'

            Token *method_token = token_stream_next(ts);
            if (!method_token || method_token->type != TOKEN_IDENTIFIER) {
                report_error("Expected method name after '->'");
                ast_node_free(root);
                return NULL;
            }

            ASTNode *method_node = ast_node_create(AST_METHOD_CALL);
            if (!method_node) {
                report_error("Failed to allocate AST node for method call");
                ast_node_free(root);
                return NULL;
            }

            method_node->string_value = strdup(method_token->text);
            if (!method_node->string_value) {
                report_error("Failed to allocate memory for method name");
                ast_node_free(root);
                ast_node_free(method_node);
                return NULL;
            }

            // Ожидается '(' для вызова метода
            Token *open_paren = token_stream_next(ts);
            if (!open_paren || open_paren->type != TOKEN_LPAREN) {
                report_error("Expected '(' after method name");
                ast_node_free(root);
                ast_node_free(method_node);
                return NULL;
            }

            // Парсим параметры вызова метода — упрощенно: пропускаем до ')'
            while (1) {
                Token *param_token = token_stream_next(ts);
                if (!param_token) {
                    report_error("Unexpected end of input in method call parameters");
                    ast_node_free(root);
                    ast_node_free(method_node);
                    return NULL;
                }
                if (param_token->type == TOKEN_RPAREN) break;
                // В реальности здесь должен быть разбор выражений параметров
            }

            ast_node_add_child(root, method_node);
            continue;
        }
        else if (next->type == TOKEN_DASH) {
            token_stream_next(ts); // consume '-'

            Token *field_token = token_stream_next(ts);
            if (!field_token || field_token->type != TOKEN_IDENTIFIER) {
                report_error("Expected field name after '-'");
                ast_node_free(root);
                return NULL;
            }

            ASTNode *field_node = ast_node_create(AST_FIELD_ACCESS);
            if (!field_node) {
                report_error("Failed to allocate AST node for field access");
                ast_node_free(root);
                return NULL;
            }

            field_node->string_value = strdup(field_token->text);
            if (!field_node->string_value) {
                report_error("Failed to allocate memory for field name");
                ast_node_free(root);
                ast_node_free(field_node);
                return NULL;
            }

            ast_node_add_child(root, field_node);
            continue;
        }
        else {
            // Конец цепочки
            break;
        }
    }

    return root;
}
```

---

### Объяснение:

* Парсится идентификатор, с которого начинается цепочка.
* Последовательно обрабатываются `->method()` вызовы и `-field` обращения к полям.
* Метод вызывается с параметрами (параметры упрощенно пропущены).
* Для каждого элемента создаются соответствующие AST узлы.

---

Если хотите, могу перейти к следующему файлу.
