#include "ir_api.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

IRInstruction *ir_create_instruction(
    IROpCode op,
    IRArg dest, bool dest_is_temp,
    IRArg arg1, bool arg1_is_const,
    IRArg arg2, bool arg2_is_const
) {
    IRInstruction *instr = (IRInstruction*)malloc(sizeof(IRInstruction));
    if (!instr) return NULL;

    instr->op = op;
    instr->dest = dest;
    instr->dest_is_temp = dest_is_temp;

    instr->arg1 = arg1;
    instr->arg1_is_const = arg1_is_const;

    instr->arg2 = arg2;
    instr->arg2_is_const = arg2_is_const;

    return instr;
}

void ir_free_instruction(IRInstruction *instr) {
    if (!instr) return;

    // Если dest, arg1 или arg2 — строковые данные (например, func_name), их нужно отдельно освобождать
    // В данном примере предполагаем, что строки выделяются и освобождаются отдельно,
    // или указываем, что вызывающий код отвечает за управление памятью строк.
    free(instr);
}

IRFunction *ir_create_function(const char *name) {
    IRFunction *func = (IRFunction*)malloc(sizeof(IRFunction));
    if (!func) return NULL;

    func->name = strdup(name);
    func->ir = NULL;
    func->ir_count = 0;

    return func;
}

void ir_free_function(IRFunction *func) {
    if (!func) return;

    free(func->name);
    for (int i = 0; i < func->ir_count; i++) {
        ir_free_instruction(&func->ir[i]);
    }
    free(func->ir);
    free(func);
}

void ir_function_add_instruction(IRFunction *func, IRInstruction *instr) {
    if (!func || !instr) return;

    IRInstruction *new_ir = realloc(func->ir, sizeof(IRInstruction) * (func->ir_count + 1));
    if (!new_ir) {
        // Ошибка выделения памяти — оставляем функцию без изменений
        return;
    }

    func->ir = new_ir;
    func->ir[func->ir_count] = *instr; // Копируем инструкцию
    func->ir_count++;

    free(instr); // Освободим внешнюю копию, так как мы сделали копию внутри
}

static const char *ir_opcode_to_string(IROpCode op) {
    switch (op) {
        case IR_NOP: return "NOP";
        case IR_ASSIGN: return "ASSIGN";
        case IR_ADD: return "ADD";
        case IR_SUB: return "SUB";
        case IR_MUL: return "MUL";
        case IR_DIV: return "DIV";
        case IR_MOD: return "MOD";
        case IR_AND: return "AND";
        case IR_OR: return "OR";
        case IR_NOT: return "NOT";
        case IR_EQ: return "EQ";
        case IR_NEQ: return "NEQ";
        case IR_LT: return "LT";
        case IR_LE: return "LE";
        case IR_GT: return "GT";
        case IR_GE: return "GE";
        case IR_CALL: return "CALL";
        case IR_RET: return "RET";
        case IR_LOOP_START: return "LOOP_START";
        case IR_LOOP_END: return "LOOP_END";
        case IR_LABEL: return "LABEL";
        case IR_JMP: return "JMP";
        case IR_JMP_IF: return "JMP_IF";
        default: return "UNKNOWN";
    }
}

static void ir_print_arg(IRArg arg, bool is_const, bool is_temp) {
    if (is_const) {
        printf("%ld", arg.value);
    } else if (is_temp) {
        printf("t%d", arg.temp_id);
    } else if (arg.func_name) {
        printf("%s", arg.func_name);
    } else if (arg.label_name) {
        printf("%s", arg.label_name);
    } else {
        printf("?");
    }
}

void ir_function_print(const IRFunction *func) {
    if (!func) return;

    printf("Function %s:\n", func->name);
    for (int i = 0; i < func->ir_count; i++) {
        IRInstruction *instr = &func->ir[i];
        printf("  %3d: %s ", i, ir_opcode_to_string(instr->op));

        // Выводим dest
        if (instr->dest_is_temp) {
            printf("t%d = ", instr->dest.temp_id);
        } else if (instr->dest.func_name) {
            printf("%s = ", instr->dest.func_name);
        }

        // Аргументы в зависимости от оператора
        switch (instr->op) {
            case IR_NOT:
            case IR_RET:
            case IR_LABEL:
                ir_print_arg(instr->arg1, instr->arg1_is_const, !instr->arg1_is_const);
                break;

            case IR_CALL:
                printf("%s(", instr->arg1.func_name ? instr->arg1.func_name : "func");
                // Для простоты не печатаем аргументы вызова — можно добавить, если надо
                printf(")");
                break;

            default:
                ir_print_arg(instr->arg1, instr->arg1_is_const, !instr->arg1_is_const);
                if (instr->op != IR_NOP && instr->op != IR_RET && instr->op != IR_LABEL) {
                    printf(", ");
                    ir_print_arg(instr->arg2, instr->arg2_is_const, !instr->arg2_is_const);
                }
                break;
        }
        printf("\n");
    }
}
