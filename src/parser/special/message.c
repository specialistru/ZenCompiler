#include "../../include/parser.h"
#include "../../include/token.h"
#include "../../include/ast.h"
#include "../../include/error.h"

/**
 * parse_special_message - Парсит оператор MESSAGE.
 *
 * MESSAGE <msg> TYPE <type> [NUMBER <num>] [WITH <args>] [DISPLAY LIKE <like>]
 * Используется для вывода системных сообщений разного типа.
 *
 * Возвращает AST узел MESSAGE с параметрами сообщения.
 */
ASTNode *parse_special_message(TokenStream *ts) {
    if (!ts) {
        report_error("TokenStream is NULL in parse_special_message");
        return NULL;
    }

    Token *msg_tok = token_stream_next(ts);
    if (!msg_tok || msg_tok->type != TOKEN_MESSAGE) {
        report_error("Expected MESSAGE token");
        return NULL;
    }

    ASTNode *node = ast_node_create(AST_SPECIAL_MESSAGE);
    if (!node) {
        report_error("Failed to create AST node for MESSAGE");
        return NULL;
    }

    // Ожидаем сообщение - либо литерал, либо идентификатор
    Token *msg_id_tok = token_stream_next(ts);
    if (!msg_id_tok || (msg_id_tok->type != TOKEN_LITERAL && msg_id_tok->type != TOKEN_IDENTIFIER)) {
        report_error("Expected message literal or identifier");
        ast_node_free(node);
        return NULL;
    }

    ASTNode *msg_id_node = ast_node_create(AST_LITERAL);
    msg_id_node->string_value = strdup(msg_id_tok->text);
    ast_node_add_child(node, msg_id_node);

    // Парсим опциональные параметры: TYPE, NUMBER, WITH, DISPLAY LIKE
    while (1) {
        Token *next_tok = token_stream_peek(ts);
        if (!next_tok) break;

        if (next_tok->type == TOKEN_TYPE) {
            token_stream_next(ts); // consume TYPE
            Token *type_tok = token_stream_next(ts);
            if (!type_tok || type_tok->type != TOKEN_IDENTIFIER) {
                report_error("Expected type identifier after TYPE");
                ast_node_free(node);
                return NULL;
            }
            ASTNode *type_node = ast_node_create(AST_IDENTIFIER);
            type_node->string_value = strdup(type_tok->text);
            ast_node_add_child(node, type_node);

        } else if (next_tok->type == TOKEN_NUMBER) {
            token_stream_next(ts); // consume NUMBER
            Token *num_tok = token_stream_next(ts);
            if (!num_tok || num_tok->type != TOKEN_LITERAL) {
                report_error("Expected literal after NUMBER");
                ast_node_free(node);
                return NULL;
            }
            ASTNode *num_node = ast_node_create(AST_LITERAL);
            num_node->string_value = strdup(num_tok->text);
            ast_node_add_child(node, num_node);

        } else if (next_tok->type == TOKEN_WITH) {
            token_stream_next(ts); // consume WITH
            // WITH может иметь несколько аргументов, парсим их пока идентификаторы/литералы
            ASTNode *with_node = ast_node_create(AST_SPECIAL_MESSAGE_WITH);

            while (1) {
                Token *arg_tok = token_stream_peek(ts);
                if (!arg_tok || (arg_tok->type != TOKEN_IDENTIFIER && arg_tok->type != TOKEN_LITERAL)) {
                    break;
                }
                token_stream_next(ts); // consume arg
                ASTNode *arg_node = ast_node_create(AST_LITERAL);
                arg_node->string_value = strdup(arg_tok->text);
                ast_node_add_child(with_node, arg_node);
            }

            ast_node_add_child(node, with_node);

        } else if (next_tok->type == TOKEN_DISPLAY) {
            token_stream_next(ts); // consume DISPLAY
            Token *like_tok = token_stream_next(ts);
            if (!like_tok || like_tok->type != TOKEN_LIKE) {
                report_error("Expected LIKE after DISPLAY");
                ast_node_free(node);
                return NULL;
            }
            Token *style_tok = token_stream_next(ts);
            if (!style_tok || style_tok->type != TOKEN_IDENTIFIER) {
                report_error("Expected identifier after LIKE");
                ast_node_free(node);
                return NULL;
            }
            ASTNode *display_node = ast_node_create(AST_SPECIAL_MESSAGE_DISPLAY);
            display_node->string_value = strdup(style_tok->text);
            ast_node_add_child(node, display_node);

        } else {
            // Нет больше опций
            break;
        }
    }

    return node;
}
