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
