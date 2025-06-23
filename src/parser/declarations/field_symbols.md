Следующий файл — `parser/declarations/field_symbols.c`.

---

### Задача `field_symbols.c` (declarations):

Парсинг инструкции `FIELD-SYMBOLS` в ABAP — объявление указателей на данные (динамические ссылки).

---

## Реализация `field_symbols.c`

```c
#include "../../include/parser.h"
#include "../../include/token.h"
#include "../../include/ast.h"
#include "../../include/error.h"
#include <stdlib.h>
#include <string.h>

/**
 * parse_declaration_field_symbols - Обрабатывает FIELD-SYMBOLS.
 *
 * Поддерживает:
 * - FIELD-SYMBOLS: <fs> TYPE i.
 * - FIELD-SYMBOLS: <wa> TYPE LINE OF itab.
 * - FIELD-SYMBOLS: <any> STRUCTURE like_some_structure.
 *
 * Возвращает AST узел FIELD_SYMBOL или NULL при ошибке.
 */
ASTNode *parse_declaration_field_symbols(TokenStream *ts) {
    if (!ts) {
        report_error("TokenStream is NULL in parse_declaration_field_symbols");
        return NULL;
    }

    ASTNode *fs_node = ast_node_create(AST_FIELD_SYMBOL_DECL);
    if (!fs_node) {
        report_error("Failed to allocate AST node for FIELD-SYMBOL");
        return NULL;
    }

    Token *sym_tok = token_stream_next(ts);
    if (!sym_tok || sym_tok->type != TOKEN_LT_IDENTIFIER) {
        report_error("Expected field-symbol name in angle brackets: <symbol>");
        ast_node_free(fs_node);
        return NULL;
    }

    fs_node->string_value = strdup(sym_tok->text);
    if (!fs_node->string_value) {
        report_error("Failed to store field-symbol name");
        ast_node_free(fs_node);
        return NULL;
    }

    // Ожидается ключевое слово TYPE или LIKE
    Token *kw_tok = token_stream_next(ts);
    if (!kw_tok || (kw_tok->type != TOKEN_TYPE && kw_tok->type != TOKEN_LIKE)) {
        report_error("Expected TYPE or LIKE in FIELD-SYMBOL declaration");
        ast_node_free(fs_node);
        return NULL;
    }

    ASTNodeType spec_type = (kw_tok->type == TOKEN_TYPE) ? AST_TYPE_SPEC : AST_LIKE_SPEC;

    Token *val_tok = token_stream_next(ts);
    if (!val_tok || val_tok->type != TOKEN_IDENTIFIER) {
        report_error("Expected type or like target in FIELD-SYMBOLS");
        ast_node_free(fs_node);
        return NULL;
    }

    ASTNode *spec_node = ast_node_create(spec_type);
    if (!spec_node) {
        report_error("Failed to create AST node for field-symbol spec");
        ast_node_free(fs_node);
        return NULL;
    }

    spec_node->string_value = strdup(val_tok->text);
    ast_node_add_child(fs_node, spec_node);

    // Опционально: STRUCTURE, LINE OF
    Token *next_tok = token_stream_peek(ts);
    if (next_tok && next_tok->type == TOKEN_LINE) {
        token_stream_next(ts); // consume LINE
        Token *of_tok = token_stream_next(ts);
        if (!of_tok || of_tok->type != TOKEN_OF) {
            report_error("Expected 'OF' after 'LINE' in FIELD-SYMBOL");
            ast_node_free(fs_node);
            return NULL;
        }

        Token *line_target = token_stream_next(ts);
        if (!line_target || line_target->type != TOKEN_IDENTIFIER) {
            report_error("Expected identifier after LINE OF");
            ast_node_free(fs_node);
            return NULL;
        }

        ASTNode *line_node = ast_node_create(AST_LINE_OF_SPEC);
        line_node->string_value = strdup(line_target->text);
        ast_node_add_child(fs_node, line_node);
    }

    return fs_node;
}
```

---

### Примеры кода, который будет обработан:

```abap
FIELD-SYMBOLS: <line> TYPE LINE OF itab.
FIELD-SYMBOLS: <val> TYPE i.
FIELD-SYMBOLS: <wa> LIKE some_structure.
```

---

### Ключевые узлы AST:

* `AST_FIELD_SYMBOL_DECL` — основной узел объявления.
* `AST_TYPE_SPEC` / `AST_LIKE_SPEC` — тип или подобие.
* `AST_LINE_OF_SPEC` — если поле указывает на строку таблицы.

---

Готов продолжить с **следующим файлом**.
