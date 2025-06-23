Следующий файл — `parser/module/simple.c`.

---

### Задача `simple.c` (module):

Парсинг простых модулей, например, модулей обработки событий или программных блоков ABAP с базовой структурой.

---

## Реализация `simple.c`

```c
#include "../../include/parser.h"
#include "../../include/token.h"
#include "../../include/ast.h"
#include "../../include/error.h"
#include <stdlib.h>
#include <string.h>

/**
 * parse_simple_module - Парсит простой модуль.
 *
 * Обрабатывает объявление модуля, включая его имя и тело,
 * которое представляет собой последовательность операторов,
 * заканчивающуюся ключевым словом ENDMODULE с точкой.
 *
 * Возвращает AST узел модуля или NULL при ошибке.
 */

ASTNode *parse_simple_module(TokenStream *ts) {
    if (!ts) {
        report_error("TokenStream is NULL in parse_simple_module");
        return NULL;
    }

    // Ожидается ключевое слово MODULE уже прочитано, теперь имя модуля
    Token *name_token = token_stream_next(ts);
    if (!name_token || name_token->type != TOKEN_IDENTIFIER) {
        report_error("Expected identifier as MODULE name");
        return NULL;
    }

    ASTNode *module_node = ast_node_create(AST_MODULE);
    if (!module_node) {
        report_error("Failed to allocate AST node for MODULE");
        return NULL;
    }

    module_node->string_value = strdup(name_token->text);
    if (!module_node->string_value) {
        report_error("Failed to allocate memory for MODULE name");
        ast_node_free(module_node);
        return NULL;
    }

    // Ожидаем точку (конец объявления MODULE)
    Token *dot_token = token_stream_next(ts);
    if (!dot_token || dot_token->type != TOKEN_DOT) {
        report_error("Expected '.' after MODULE name");
        ast_node_free(module_node);
        return NULL;
    }

    // Парсим тело модуля до ENDMODULE
    while (1) {
        Token *next_token = token_stream_peek(ts);
        if (!next_token) {
            report_error("Unexpected end of tokens inside MODULE");
            ast_node_free(module_node);
            return NULL;
        }

        if (next_token->type == TOKEN_ENDMODULE) {
            token_stream_next(ts); // consume ENDMODULE
            Token *end_dot = token_stream_next(ts);
            if (!end_dot || end_dot->type != TOKEN_DOT) {
                report_error("Expected '.' after ENDMODULE");
                ast_node_free(module_node);
                return NULL;
            }
            break;
        }

        ASTNode *stmt = parse_statement(ts);
        if (!stmt) {
            report_error("Failed to parse statement inside MODULE");
            ast_node_free(module_node);
            return NULL;
        }

        ast_node_add_child(module_node, stmt);
    }

    return module_node;
}
```

---

### Объяснение:

* Парсится имя модуля после ключевого слова MODULE.
* Тело модуля — последовательность операторов до ключевого слова ENDMODULE.
* Создается AST узел с дочерними операторами.
* Выполняется проверка синтаксиса и обязательных точек.

---

Готов к следующему файлу?
