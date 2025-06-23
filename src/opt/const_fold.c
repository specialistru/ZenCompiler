/*

/**
 * @file const_fold.c
 * @brief Реализация свёртывания констант в IR.
 */

#include "const_fold.h"

void const_fold(IRInstruction *ir, int count) {
    for (int i = 0; i < count; ++i) {
        IRInstruction *instr = &ir[i];

        if (instr->arg1_type == ARG_IMM && instr->arg2_type == ARG_IMM) {
            switch (instr->op) {
                case IR_ADD:
                    instr->arg1_value += instr->arg2_value;
                    instr->arg2_type = ARG_NONE;
                    instr->op = IR_ASSIGN;
                    break;
                case IR_SUB:
                    instr->arg1_value -= instr->arg2_value;
                    instr->arg2_type = ARG_NONE;
                    instr->op = IR_ASSIGN;
                    break;
                // Расширить под другие операции
                default:
                    break;
            }
        }
    }
}
*/

/**
 * @file const_fold.c
 * @brief Реализация расширенного свёртывания констант в IR.
 */

#include "const_fold.h"
#include <stdio.h>

static bool is_identity_op(IRInstruction *instr) {
    // x = x + 0; x = x * 1; etc.
    if (instr->op == IR_ADD && instr->arg2_type == ARG_IMM && instr->arg2_value == 0)
        return true;
    if (instr->op == IR_MUL && instr->arg2_type == ARG_IMM && instr->arg2_value == 1)
        return true;
    return false;
}

void const_fold(IRInstruction *ir, int count) {
    for (int i = 0; i < count; ++i) {
        IRInstruction *instr = &ir[i];

        // Пропуск, если не бинарная операция
        if (instr->arg1_type != ARG_IMM || instr->arg2_type != ARG_IMM)
            continue;

        int a = instr->arg1_value;
        int b = instr->arg2_value;
        int result;
        bool replaced = true;

        switch (instr->op) {
            case IR_ADD:  result = a + b; break;
            case IR_SUB:  result = a - b; break;
            case IR_MUL:  result = a * b; break;
            case IR_DIV:
                if (b == 0) {
                    fprintf(stderr, "Error: Division by zero in IR constant folding\n");
                    continue;
                }
                result = a / b;
                break;
            case IR_MOD:
                if (b == 0) {
                    fprintf(stderr, "Error: Modulo by zero in IR constant folding\n");
                    continue;
                }
                result = a % b;
                break;

            case IR_EQ:   result = (a == b); break;
            case IR_NEQ:  result = (a != b); break;
            case IR_LT:   result = (a < b);  break;
            case IR_GT:   result = (a > b);  break;
            case IR_LE:   result = (a <= b); break;
            case IR_GE:   result = (a >= b); break;

            case IR_AND:  result = a && b; break;
            case IR_OR:   result = a || b; break;

            default:
                replaced = false;
                break;
        }

        if (replaced) {
            instr->op = IR_ASSIGN;
            instr->arg1_value = result;
            instr->arg1_type = ARG_IMM;
            instr->arg2_type = ARG_NONE;
            instr->arg2_value = 0;
        }
    }

    // Peephole optimizations
    for (int i = 0; i < count; ++i) {
        IRInstruction *instr = &ir[i];

        if (is_identity_op(instr)) {
            instr->op = IR_ASSIGN;
            instr->arg2_type = ARG_NONE;
        }
    }
}


// Compiler/src/opt/const_fold.c
#include <stdio.h>
#include "ir_api.h"

/**
 * Проверяет, является ли операция арифметической бинарной операцией,
 * для которой можно выполнить свёртывание констант.
 */
static bool is_foldable_op(IROpCode op) {
    switch(op) {
        case IR_ADD:
        case IR_SUB:
        case IR_MUL:
        case IR_DIV:
        case IR_MOD:
        case IR_AND:
        case IR_OR:
        case IR_XOR:
            return true;
        default:
            return false;
    }
}

/**
 * Выполняет свёртывание констант для заданной инструкции, если оба аргумента — константы.
 * Заменяет инструкцию на присваивание с вычисленным значением.
 */
static bool try_fold_constants(IRInstruction *instr) {
    if (!is_foldable_op(instr->op)) return false;
    if (!instr->arg1_is_const || !instr->arg2_is_const) return false;

    int64_t val1 = instr->arg1.value;
    int64_t val2 = instr->arg2.value;
    int64_t result = 0;

    switch (instr->op) {
        case IR_ADD: result = val1 + val2; break;
        case IR_SUB: result = val1 - val2; break;
        case IR_MUL: result = val1 * val2; break;
        case IR_DIV:
            if (val2 == 0) return false; // защита от деления на 0
            result = val1 / val2;
            break;
        case IR_MOD:
            if (val2 == 0) return false;
            result = val1 % val2;
            break;
        case IR_AND: result = val1 & val2; break;
        case IR_OR:  result = val1 | val2; break;
        case IR_XOR: result = val1 ^ val2; break;
        default: return false;
    }

    // Замена инструкции на присваивание константы
    instr->op = IR_ASSIGN;
    instr->arg1.value = result;
    instr->arg1_is_const = true;
    instr->arg2_is_const = false;  // второй аргумент не нужен
    return true;
}

/**
 * Основная функция свёртывания констант в IR-функции.
 */
void optimizer_const_fold(IRFunction *func) {
    if (!func) return;

    for (int i = 0; i < func->ir_count; i++) {
        IRInstruction *instr = &func->ir[i];
        if (try_fold_constants(instr)) {
            printf("Const folded instruction at %d in function %s\n", i, func->name);
        }
    }
}

#include "const_fold.h"
#include <stdio.h>

/**
 * @brief Свёртывание констант: вычисляет константные выражения.
 */
void const_fold(IR_Function *function) {
    if (!function) return;

    printf("Constant Folding: Processing function %s\n", function->name);

    for (int i = 0; i < function->instructions_count; i++) {
        IR_Instruction *instr = &function->instructions[i];

        if (instr->opcode == IR_ADD || instr->opcode == IR_SUB ||
            instr->opcode == IR_MUL || instr->opcode == IR_DIV) {

            if (instr->operand1.is_constant && instr->operand2.is_constant) {
                int result = 0;
                switch (instr->opcode) {
                    case IR_ADD:
                        result = instr->operand1.constant_value + instr->operand2.constant_value;
                        break;
                    case IR_SUB:
                        result = instr->operand1.constant_value - instr->operand2.constant_value;
                        break;
                    case IR_MUL:
                        result = instr->operand1.constant_value * instr->operand2.constant_value;
                        break;
                    case IR_DIV:
                        if (instr->operand2.constant_value != 0)
                            result = instr->operand1.constant_value / instr->operand2.constant_value;
                        else
                            continue; // Не делим на 0
                        break;
                    default:
                        break;
                }

                instr->opcode = IR_LOAD_CONST;
                instr->operand1.is_constant = true;
                instr->operand1.constant_value = result;
                // Очистить operand2
                instr->operand2.is_constant = false;

                printf("Folded constant expression at instruction %d: result=%d\n", i, result);
            }
        }
    }
}
