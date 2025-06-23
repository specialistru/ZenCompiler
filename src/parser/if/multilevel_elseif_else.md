Следующий файл — `parser/if/multilevel_elseif_else.c`.

---

### Задача `multilevel_elseif_else.c`:

Реализовать парсер для вложенных конструкций IF с множественными ELSEIF и ELSE.

---

## Реализация `multilevel_elseif_else.c`

```c
#include "../../include/parser.h"
#include "../../include/token.h"
#include "../../include/ast.h"
#include "../../include/error.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * parse_if_multilevel - Парсит конструкцию IF с несколькими ELSEIF и ELSE.
 *
 * Возвращает AST узел или NULL при ошибке.
 */
ASTNode *parse_if_multilevel(TokenStream *ts) {
    if (!ts) {
        fprintf(stderr, "TokenStream is NULL\n");
        return NULL;
    }

    Token *token = token_stream_peek(ts);
    if (!token || token->type != TOKEN_KEYWORD_IF) {
        report_error("Expected IF keyword");
        return NULL;
    }
    token_stream_next(ts); // съесть IF

    ASTNode *if_node = ast_node_create(AST_IF);
    if (!if_node) {
        report_error("Failed to create AST IF node");
        return NULL;
    }

    ASTNode *condition = parse_expression(ts);
    if (!condition) {
        report_error("Failed to parse IF condition");
        ast_node_free(if_node);
        return NULL;
    }

    token = token_stream_next(ts);
    if (!token || token->type != TOKEN_DOT) {
        report_error("Expected '.' after IF condition");
        ast_node_free(condition);
        ast_node_free(if_node);
        return NULL;
    }

    if_node->if_condition = condition;
    if_node->if_body = ast_node_list_create();
    if (!if_node->if_body) {
        report_error("Failed to create ASTNodeList for IF body");
        ast_node_free(if_node);
        return NULL;
    }

    while (1) {
        token = token_stream_peek(ts);
        if (!token) {
            report_error("Unexpected end of input inside IF body");
            ast_node_free(if_node);
            return NULL;
        }

        if (token->type == TOKEN_KEYWORD_ELSEIF) {
            // Парсим ELSEIF
            token_stream_next(ts); // съесть ELSEIF

            ASTNode *elseif_node = ast_node_create(AST_ELSEIF);
            if (!elseif_node) {
                report_error("Failed to create AST ELSEIF node");
                ast_node_free(if_node);
                return NULL;
            }

            ASTNode *elseif_cond = parse_expression(ts);
            if (!elseif_cond) {
                report_error("Failed to parse ELSEIF condition");
                ast_node_free(elseif_node);
                ast_node_free(if_node);
                return NULL;
            }

            token = token_stream_next(ts);
            if (!token || token->type != TOKEN_DOT) {
                report_error("Expected '.' after ELSEIF condition");
                ast_node_free(elseif_node);
                ast_node_free(if_node);
                return NULL;
            }

            elseif_node->if_condition = elseif_cond;
            elseif_node->if_body = ast_node_list_create();
            if (!elseif_node->if_body) {
                report_error("Failed to create ASTNodeList for ELSEIF body");
                ast_node_free(elseif_node);
                ast_node_free(if_node);
                return NULL;
            }

            // Парсим тело ELSEIF
            while (1) {
                token = token_stream_peek(ts);
                if (!token) {
                    report_error("Unexpected end of input inside ELSEIF body");
                    ast_node_free(elseif_node);
                    ast_node_free(if_node);
                    return NULL;
                }
                if (token->type == TOKEN_KEYWORD_ELSEIF ||
                    token->type == TOKEN_KEYWORD_ELSE ||
                    token->type == TOKEN_KEYWORD_ENDIF) {
                    break;
                }

                ASTNode *stmt = parse_statement(ts);
                if (!stmt) {
                    report_error("Failed to parse statement inside ELSEIF body");
                    ast_node_free(elseif_node);
                    ast_node_free(if_node);
                    return NULL;
                }
                ast_node_list_append(elseif_node->if_body, stmt);
            }

            ast_node_list_append(if_node->elseif_list, elseif_node);

        } else if (token->type == TOKEN_KEYWORD_ELSE) {
            // Парсим ELSE
            token_stream_next(ts); // съесть ELSE

            token = token_stream_next(ts);
            if (!token || token->type != TOKEN_DOT) {
                report_error("Expected '.' after ELSE");
                ast_node_free(if_node);
                return NULL;
            }

            if_node->else_body = ast_node_list_create();
            if (!if_node->else_body) {
                report_error("Failed to create ASTNodeList for ELSE body");
                ast_node_free(if_node);
                return NULL;
            }

            // Парсим тело ELSE
            while (1) {
                token = token_stream_peek(ts);
                if (!token) {
                    report_error("Unexpected end of input inside ELSE body");
                    ast_node_free(if_node);
                    return NULL;
                }
                if (token->type == TOKEN_KEYWORD_ENDIF) {
                    break;
                }

                ASTNode *stmt = parse_statement(ts);
                if (!stmt) {
                    report_error("Failed to parse statement inside ELSE body");
                    ast_node_free(if_node);
                    return NULL;
                }
                ast_node_list_append(if_node->else_body, stmt);
            }

        } else if (token->type == TOKEN_KEYWORD_ENDIF) {
            break;
        } else {
            ASTNode *stmt = parse_statement(ts);
            if (!stmt) {
                report_error("Failed to parse statement inside IF body");
                ast_node_free(if_node);
                return NULL;
            }
            ast_node_list_append(if_node->if_body, stmt);
        }
    }

    token = token_stream_next(ts);
    if (!token || token->type != TOKEN_KEYWORD_ENDIF) {
        report_error("Expected ENDIF keyword");
        ast_node_free(if_node);
        return NULL;
    }

    token = token_stream_next(ts);
    if (!token || token->type != TOKEN_DOT) {
        report_error("Expected '.' after ENDIF");
        ast_node_free(if_node);
        return NULL;
    }

    return if_node;
}
```

---

### Объяснение:

* Парсит IF, после него тело.
* Поддерживает много ELSEIF — список `elseif_list`.
* Поддерживает ELSE.
* Завершается ENDIF.
* Создает сложное AST с вложенными узлами.

---

Если хочешь — могу перейти к следующему файлу.
