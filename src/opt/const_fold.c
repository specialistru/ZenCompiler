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
