Следующий файл — `parser/declarations/parameters.c`.

---

### Задача `parameters.c` (declarations):

Парсинг инструкции `PARAMETERS` в ABAP — объявление параметров ввода (в форме, функциях, экранах).

---

## Реализация `parameters.c`

```c
#include "../../include/parser.h"
#include "../../include/token.h"
#include "../../include/ast.h"
#include "../../include/error.h"
#include <stdlib.h>
#include <string.h>

/**
 * parse_declaration_parameters - Парсит инструкцию PARAMETERS.
 *
 * Ожидается, что ключевое слово PARAMETERS уже прочитано.
 * Возвращает AST узел PARAMETERS или NULL при ошибке.
 */
ASTNode *parse_declaration_parameters(TokenStream *ts) {
    if (!ts) {
        report_error("TokenStream is NULL in parse_declaration_parameters");
        return NULL;
    }

    ASTNode *param_node = ast_node_create(AST_PARAMETERS_DECL);
    if (!param_node) {
        report_error("Failed to allocate AST node for PARAMETERS declaration");
        return NULL;
    }

    // Имя параметра
    Token *id_tok = token_stream_next(ts);
    if (!id_tok || id_tok->type != TOKEN_IDENTIFIER) {
        report_error("Expected identifier after PARAMETERS");
        ast_node_free(param_node);
        return NULL;
    }

    param_node->string_value = strdup(id_tok->text);
    if (!param_node->string_value) {
        report_error("Failed to allocate memory for parameter name");
        ast_node_free(param_node);
        return NULL;
    }

    // Опциональные: TYPE, LIKE, DEFAULT, OBLIGATORY, LOWER CASE, etc.
    Token *tok;
    while ((tok = token_stream_peek(ts)) != NULL &&
           (tok->type == TOKEN_TYPE || tok->type == TOKEN_LIKE ||
            tok->type == TOKEN_DEFAULT || tok->type == TOKEN_OBLIGATORY ||
            tok->type == TOKEN_LOWER || tok->type == TOKEN_CASE)) {

        tok = token_stream_next(ts); // consume keyword

        ASTNodeType subtype;
        switch (tok->type) {
            case TOKEN_TYPE:       subtype = AST_TYPE_SPEC;       break;
            case TOKEN_LIKE:       subtype = AST_LIKE_SPEC;       break;
            case TOKEN_DEFAULT:    subtype = AST_DEFAULT_SPEC;    break;
            case TOKEN_OBLIGATORY: subtype = AST_OBLIGATORY_SPEC; break;
            case TOKEN_LOWER:      // Check for "LOWER CASE"
                if (token_stream_peek(ts) && token_stream_peek(ts)->type == TOKEN_CASE) {
                    token_stream_next(ts); // consume CASE
                    subtype = AST_LOWER_CASE_SPEC;
                    break;
                } else {
                    report_error("Expected CASE after LOWER in PARAMETERS");
                    ast_node_free(param_node);
                    return NULL;
                }
            default:
                report_error("Unexpected keyword in PARAMETERS");
                ast_node_free(param_node);
                return NULL;
        }

        ASTNode *spec_node = ast_node_create(subtype);
        if (!spec_node) {
            report_error("Failed to allocate AST node for PARAMETERS specifier");
            ast_node_free(param_node);
            return NULL;
        }

        if (subtype == AST_TYPE_SPEC || subtype == AST_LIKE_SPEC || subtype == AST_DEFAULT_SPEC) {
            Token *val_tok = token_stream_next(ts);
            if (!val_tok || val_tok->type != TOKEN_IDENTIFIER) {
                report_error("Expected identifier or literal after specifier in PARAMETERS");
                ast_node_free(param_node);
                ast_node_free(spec_node);
                return NULL;
            }

            spec_node->string_value = strdup(val_tok->text);
            if (!spec_node->string_value) {
                report_error("Failed to allocate memory for specifier value");
                ast_node_free(param_node);
                ast_node_free(spec_node);
                return NULL;
            }
        }

        ast_node_add_child(param_node, spec_node);
    }

    return param_node;
}
```

---

### Объяснение:

* Поддерживает конструкции вида:

  ```abap
  PARAMETERS: p_date TYPE d DEFAULT '20230601' OBLIGATORY.
  PARAMETERS: p_user LIKE sy-uname.
  PARAMETERS: p_flag TYPE c LOWER CASE.
  ```

* Использует типы узлов AST:

  * `AST_PARAMETERS_DECL` — базовый параметр;
  * `AST_TYPE_SPEC`, `AST_LIKE_SPEC`, `AST_DEFAULT_SPEC` — уточнения;
  * `AST_OBLIGATORY_SPEC`, `AST_LOWER_CASE_SPEC` — флаги.

* Ветка `LOWER CASE` реализована как двойной токен.

---

Готов перейти к следующему файлу!
