// Compiler/src/ir/ir_generator.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ir_generator.h"
#include "ir.h"
#include "parser.h"
#include "logging.h"  // модуль логирования ошибок и предупреждений

// Контекст генерации IR - хранит состояние генератора
typedef struct {
    int label_count;          // Счётчик для генерации уникальных меток
    IRFunction *current_func; // Текущая функция, куда генерируем IR
} IRGenContext;

// Вспомогательная функция для создания новой метки
static char *generate_label(IRGenContext *ctx, const char *prefix) {
    char *label = malloc(64);
    if (!label) {
        log_error("Out of memory while generating label");
        exit(EXIT_FAILURE);
    }
    snprintf(label, 64, "%s_%d", prefix, ctx->label_count++);
    return label;
}

// Вспомогательная функция для создания операнда IR из AST узла переменной
static IROperand ir_operand_from_ast_var(ASTNode *node) {
    IROperand op;
    op.type = IR_OPERAND_VAR;
    strncpy(op.name, node->var_name, sizeof(op.name));
    return op;
}

// Генерация IR для выражений
static IRNode *generate_ir_expr(IRGenContext *ctx, ASTNode *expr) {
    if (!expr) {
        log_error("Null expression node in IR generation");
        return NULL;
    }

    switch (expr->type) {
        case AST_INT_LITERAL: {
            IRNode *node = ir_node_create(IR_OPCODE_LOAD_CONST);
            node->dest = ir_operand_create_temp();
            node->src1.type = IR_OPERAND_CONST_INT;
            node->src1.int_value = expr->int_value;
            return node;
        }
        case AST_VAR: {
            IRNode *node = ir_node_create(IR_OPCODE_LOAD_VAR);
            node->dest = ir_operand_create_temp();
            node->src1 = ir_operand_from_ast_var(expr);
            return node;
        }
        case AST_BINARY_OP: {
            IRNode *left_ir = generate_ir_expr(ctx, expr->left);
            IRNode *right_ir = generate_ir_expr(ctx, expr->right);
            if (!left_ir || !right_ir) return NULL;

            IRNode *bin_op = ir_node_create(expr->bin_op_code);
            bin_op->dest = ir_operand_create_temp();
            bin_op->src1 = left_ir->dest;
            bin_op->src2 = right_ir->dest;

            ir_node_chain(left_ir, bin_op);
            ir_node_chain(bin_op, right_ir);

            return left_ir; // голова цепочки
        }
        case AST_FUNC_CALL: {
            IRNode *call_node = ir_node_create(IR_OPCODE_CALL);
            call_node->func_name = strdup(expr->func_name);

            // Генерация IR для аргументов
            IRNode *args_head = NULL;
            IRNode *args_tail = NULL;
            for (int i = 0; i < expr->arg_count; i++) {
                IRNode *arg_ir = generate_ir_expr(ctx, expr->args[i]);
                if (!arg_ir) return NULL;
                if (!args_head) args_head = args_tail = arg_ir;
                else {
                    ir_node_chain(args_tail, arg_ir);
                    args_tail = arg_ir;
                }
            }

            if (args_head)
                ir_node_chain(args_tail, call_node);

            call_node->dest = ir_operand_create_temp();

            return args_head ? args_head : call_node;
        }
        default:
            log_error("Unsupported expression type in IR generation: %d", expr->type);
            return NULL;
    }
}

// Генерация IR для оператора if
static IRNode *generate_ir_if(IRGenContext *ctx, ASTNode *if_node) {
    if (!if_node || if_node->type != AST_IF) {
        log_error("Invalid AST node passed to generate_ir_if");
        return NULL;
    }

    IRNode *cond_ir = generate_ir_expr(ctx, if_node->condition);
    if (!cond_ir) return NULL;

    char *label_then = generate_label(ctx, "then");
    char *label_else = generate_label(ctx, "else");
    char *label_end = generate_label(ctx, "endif");

    IRNode *jmp_if_false = ir_node_create(IR_OPCODE_JMP_IF_FALSE);
    jmp_if_false->src1 = cond_ir->dest;
    jmp_if_false->label = label_else;

    IRNode *then_ir = generate_ir_block(ctx, if_node->then_block);
    IRNode *jmp_end = ir_node_create(IR_OPCODE_JMP);
    jmp_end->label = label_end;

    IRNode *else_ir = NULL;
    if (if_node->else_block)
        else_ir = generate_ir_block(ctx, if_node->else_block);

    // Построение цепочки IR:
    // cond_ir -> jmp_if_false -> then_ir -> jmp_end -> else_ir -> label_end

    ir_node_chain(cond_ir, jmp_if_false);
    ir_node_chain(jmp_if_false, then_ir);
    ir_node_chain(then_ir, jmp_end);

    if (else_ir) {
        ir_node_chain(jmp_end, else_ir);
        // Метка конца после else
        IRNode *label_end_node = ir_node_create(IR_OPCODE_LABEL);
        label_end_node->label = label_end;
        ir_node_chain(else_ir, label_end_node);
    } else {
        // Метка конца после then, если else нет
        IRNode *label_end_node = ir_node_create(IR_OPCODE_LABEL);
        label_end_node->label = label_end;
        ir_node_chain(jmp_end, label_end_node);
    }

    return cond_ir;
}

// Генерация IR для блока AST (списка операторов)
IRNode *generate_ir_block(IRGenContext *ctx, ASTNode *block) {
    if (!block) {
        log_warn("Empty block in IR generation");
        return NULL;
    }

    IRNode *head = NULL;
    IRNode *tail = NULL;

    for (ASTNode *stmt = block; stmt != NULL; stmt = stmt->next) {
        IRNode *stmt_ir = NULL;

        switch (stmt->type) {
            case AST_ASSIGNMENT:
                stmt_ir = generate_ir_assignment(ctx, stmt);
                break;
            case AST_IF:
                stmt_ir = generate_ir_if(ctx, stmt);
                break;
            case AST_FUNC_CALL:
                stmt_ir = generate_ir_expr(ctx, stmt);
                break;
            case AST_RETURN:
                stmt_ir = generate_ir_return(ctx, stmt);
                break;
            case AST_LOOP:
                stmt_ir = generate_ir_loop(ctx, stmt);
                break;
            // Добавить поддержку других конструкций по необходимости
            default:
                log_error("Unsupported statement type in block: %d", stmt->type);
                break;
        }

        if (!stmt_ir) continue;

        if (!head) {
            head = tail = stmt_ir;
        } else {
            ir_node_chain(tail, stmt_ir);
            tail = stmt_ir;
        }
    }

    return head;
}

// Генерация IR для присваивания
IRNode *generate_ir_assignment(IRGenContext *ctx, ASTNode *assign) {
    IRNode *expr_ir = generate_ir_expr(ctx, assign->rhs);
    if (!expr_ir) return NULL;

    IRNode *assign_node = ir_node_create(IR_OPCODE_STORE);
    assign_node->dest = ir_operand_from_ast_var(assign->lhs);
    assign_node->src1 = expr_ir->dest;

    ir_node_chain(expr_ir, assign_node);
    return expr_ir;
}

// Генерация IR для return
IRNode *generate_ir_return(IRGenContext *ctx, ASTNode *ret) {
    IRNode *expr_ir = NULL;
    if (ret->return_value) {
        expr_ir = generate_ir_expr(ctx, ret->return_value);
        if (!expr_ir) return NULL;
    }

    IRNode *ret_node = ir_node_create(IR_OPCODE_RETURN);
    if (expr_ir)
        ret_node->src1 = expr_ir->dest;

    if (expr_ir)
        ir_node_chain(expr_ir, ret_node);
    else
        return ret_node;

    return expr_ir;
}

// Генерация IR для циклов (пример для while)
IRNode *generate_ir_loop(IRGenContext *ctx, ASTNode *loop) {
    if (!loop || (loop->type != AST_LOOP && loop->type != AST_WHILE)) {
        log_error("Invalid node for generate_ir_loop");
        return NULL;
    }

    char *label_start = generate_label(ctx, "loop_start");
    char *label_end = generate_label(ctx, "loop_end");

    IRNode *label_start_node = ir_node_create(IR_OPCODE_LABEL);
    label_start_node->label = label_start;

    IRNode *cond_ir = generate_ir_expr(ctx, loop->condition);
    if (!cond_ir) return NULL;

    IRNode *jmp_if_false = ir_node_create(IR_OPCODE_JMP_IF_FALSE);
    jmp_if_false->src1 = cond_ir->dest;
    jmp_if_false->label = label_end;

    IRNode *body_ir = generate_ir_block(ctx, loop->body);

    IRNode *jmp_start = ir_node_create(IR_OPCODE_JMP);
    jmp_start->label = label_start;

    IRNode *label_end_node = ir_node_create(IR_OPCODE_LABEL);
    label_end_node->label = label_end;

    // chain all
    ir_node_chain(label_start_node, cond_ir);
    ir_node_chain(cond_ir, jmp_if_false);
    ir_node_chain(jmp_if_false, body_ir);
    ir_node_chain(body_ir, jmp_start);
    ir_node_chain(jmp_start, label_end_node);

    return label_start_node;
}

// Внешний API функции генерации IR из AST
IRNode *ir_generate_from_ast(ASTNode *ast) {
    IRGenContext ctx = {0};
    return generate_ir_block(&ctx, ast);
}


#include "ir_api.h"
// Другие нужные заголовки...

// Глобальный текущий IR-функция, в которую мы генерируем код
static IRFunction *current_function = NULL;

void ir_generator_start_function(const char *name) {
    if (current_function) {
        ir_free_function(current_function);
    }
    current_function = ir_create_function(name);
}

void ir_generator_finish_function() {
    if (!current_function) return;
    // Тут можно добавить вывод или сохранение IR
    ir_function_print(current_function);
    // Освобождать пока не будем, функция может быть нужна дальше
}

// Пример генерации IR инструкции — теперь через API
void ir_generator_emit(IROpCode op, IRArg dest, bool dest_is_temp,
                      IRArg arg1, bool arg1_is_const,
                      IRArg arg2, bool arg2_is_const) {
    IRInstruction *instr = ir_create_instruction(op, dest, dest_is_temp, arg1, arg1_is_const, arg2, arg2_is_const);
    if (!instr) {
        fprintf(stderr, "Ошибка выделения памяти для IR инструкции\n");
        return;
    }
    ir_function_add_instruction(current_function, instr);
}

// Пример: генерация простой инструкции присваивания
void ir_generator_emit_assign(IRArg dest, bool dest_is_temp, IRArg src, bool src_is_const) {
    ir_generator_emit(IR_ASSIGN, dest, dest_is_temp, src, src_is_const, (IRArg){0}, false);
}

// ... остальной код генератора будет строиться аналогично

