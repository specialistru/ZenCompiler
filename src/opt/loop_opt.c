/**
 * @file loop_opt.c
 * @brief Примитивная оптимизация циклов.
 */

#include "loop_opt.h"

int optimize_loops(IRInstruction *ir, int count) {
    // ⚠ Простейший пример: пропуск неэффективных циклов
    int changes = 0;
    for (int i = 0; i < count; ++i) {
        if (ir[i].op == IR_LOOP && ir[i].arg1_value == 0) {
            ir[i].op = IR_NOP; // Бесконечный цикл заменяется на NOP
            changes++;
        }
    }
    return count;
}
