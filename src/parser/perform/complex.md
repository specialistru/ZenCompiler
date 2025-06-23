Следующий файл — `parser/perform/complex.c`.

---

### Задача `complex.c` для perform:

Парсинг сложных конструкций PERFORM, включая вложенные вызовы, параметры, условия и блоки кода.

---

## Реализация `complex.c`

```c
#include "../../include/parser.h"
#include "../../include/token.h"
#include "../../include/ast.h"
#include "../../include/error.h"
#include <stdlib.h>

/**
 * parse_perform_complex - Парсит сложные конструкции PERFORM,
 * включая параметры и опциональные условия.
 *
 * Синтаксис примера:
 * PERFORM <form_name> [USING <params>] [CHANGING <params>] [ON EXCEPTIONS] [TABLES <tables>] [WHERE <condition>].
 *
 * Возвращает AST узел PERFORM или NULL при ошибке.
 */
ASTNode *parse_perform_complex(TokenStream *ts) {
    if (!ts) {
        report_error("TokenStream is NULL in parse_perform_complex");
        return NULL;
    }

    Token *token = token_stream_next(ts);
    if (!token || token->type != TOKEN_KEYWORD_PERFORM) {
        report_error("Expected PERFORM keyword");
        return NULL;
    }

    // Имя формы
    token = token_stream_next(ts);
    if (!token || token->type != TOKEN_IDENTIFIER) {
        report_error("Expected form name after PERFORM");
        return NULL;
    }

    ASTNode *perform_node = ast_node_create(AST_PERFORM);
    if (!perform_node) {
        report_error("Failed to create AST node for PERFORM");
        return NULL;
    }

    perform_node->form_name = strdup(token->text);
    if (!perform_node->form_name) {
        report_error("Memory allocation failed for form_name");
        ast_node_free(perform_node);
        return NULL;
    }

    // Обрабатываем параметры (USING, CHANGING, TABLES и др.)
    while (1) {
        token = token_stream_peek(ts);
        if (!token) break;

        if (token->type == TOKEN_KEYWORD_USING) {
            token_stream_next(ts); // съесть USING
            ASTNodeList *using_params = parse_parameter_list(ts);
            if (!using_params) {
                report_error("Failed to parse USING parameters");
                ast_node_free(perform_node);
                return NULL;
            }
            perform_node->using_params = using_params;
        } else if (token->type == TOKEN_KEYWORD_CHANGING) {
            token_stream_next(ts); // съесть CHANGING
            ASTNodeList *changing_params = parse_parameter_list(ts);
            if (!changing_params) {
                report_error("Failed to parse CHANGING parameters");
                ast_node_free(perform_node);
                return NULL;
            }
            perform_node->changing_params = changing_params;
        } else if (token->type == TOKEN_KEYWORD_TABLES) {
            token_stream_next(ts); // съесть TABLES
            ASTNodeList *tables_params = parse_parameter_list(ts);
            if (!tables_params) {
                report_error("Failed to parse TABLES parameters");
                ast_node_free(perform_node);
                return NULL;
            }
            perform_node->tables_params = tables_params;
        } else if (token->type == TOKEN_KEYWORD_ON) {
            // ON EXCEPTIONS
            token_stream_next(ts); // съесть ON
            token = token_stream_next(ts);
            if (!token || token->type != TOKEN_KEYWORD_EXCEPTIONS) {
                report_error("Expected EXCEPTIONS after ON");
                ast_node_free(perform_node);
                return NULL;
            }
            perform_node->on_exceptions = 1;
        } else {
            break;
        }
    }

    return perform_node;
}
```

---

### Объяснение:

* Парсит сложный синтаксис PERFORM с параметрами USING, CHANGING, TABLES и опциями ON EXCEPTIONS.
* Создаёт AST узел с заполнением соответствующих полей.
* Проверяет корректность последовательности токенов.
* Использует вспомогательные функции (например, `parse_parameter_list`) для разбора списков параметров.

---

Готов к следующему файлу!
