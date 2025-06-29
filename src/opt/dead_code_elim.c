/*
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
*/

/**
 * @file dead_code_elim.c
 * @brief Удаляет мёртвые инструкции в IR (более строгий вариант).
 */

#include "dead_code_elim.h"
#include <string.h>

int eliminate_dead_code(IRInstruction *ir, int count) {
    bool *used = calloc(count, sizeof(bool));
    if (!used) return count;

    // 1. Сбор использования dst-операндов
    for (int i = 0; i < count; i++) {
        IRInstruction *instr = &ir[i];
        if (instr->op == IR_ASSIGN && instr->dst[0] && !(instr->arg1_type == ARG_REG && strcmp(instr->dst, instr->src1_reg) == 0)) {
            // dst = reg, считаем dst "использованным"
            used[i] = true;
        }
        if (instr->op == IR_STORE && instr->dst[0]) used[i] = true;
        if (instr->op == IR_CALL && instr->dst[0]) used[i] = true;
    }

    // 2. Удаление инструкций, не производящих эффект
    int write_idx = 0;
    for (int i = 0; i < count; i++) {
        IRInstruction *instr = &ir[i];
        bool keep = true;
        if (instr->op == IR_NOP) keep = false;
        if (instr->op == IR_ASSIGN && !used[i]) keep = false;
        if (!keep) continue;

        ir[write_idx++] = ir[i];
    }

    free(used);
    return write_idx;
}


#include "dead_code_elim.h"
#include <stdbool.h>
#include <stdio.h>

/**
 * @brief Основной алгоритм удаления мёртвого кода.
 * Анализирует инструкции и удаляет те, что не влияют на результат.
 */
void dead_code_elim(IR_Function *function) {
    if (!function) return;

    printf("Dead Code Elimination: Analyzing function %s\n", function->name);

    bool changed = true;

    while (changed) {
        changed = false;

        for (int i = 0; i < function->instructions_count; i++) {
            IR_Instruction *instr = &function->instructions[i];

            if (!instr->used && !instr->has_side_effects) {
                // Удаляем инструкцию
                // Реально здесь нужно аккуратно сдвигать массив, обновлять индексы и зависимости
                instr->opcode = IR_NOP;
                changed = true;
                printf("Removed dead instruction at index %d\n", i);
            }
        }
    }
}
