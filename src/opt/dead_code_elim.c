/**
 * @file dead_code_elim.c
 * @brief Удаление мёртвых инструкций и ветвей IR.
 */

#include "dead_code_elim.h"

int eliminate_dead_code(IRInstruction *ir, int count) {
    int write_index = 0;

    for (int i = 0; i < count; ++i) {
        IRInstruction *instr = &ir[i];

        // Простейший критерий: IR_NOP или пустая операция
        if (instr->op == IR_NOP || (instr->op == IR_ASSIGN && instr->dst[0] == '\0')) {
            continue;
        }

        ir[write_index++] = *instr;
    }

    return write_index;
}
