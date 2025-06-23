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


// Compiler/src/opt/loop_opt.c
#include <stdio.h>
#include "ir_api.h"

/**
 * Выполняет разворачивание простых циклов.
 * Находит инструкции цикла и дублирует тело цикла несколько раз.
 * Упрощённая демонстрация.
 */
void optimizer_loop_unroll(IRFunction *func) {
    if (!func) return;

    // Поиск цикла по специальной инструкции (например, IR_LOOP_START)
    for (int i = 0; i < func->ir_count; i++) {
        IRInstruction *instr = &func->ir[i];
        if (instr->op == IR_LOOP_START) {
            printf("Loop unrolling at instruction %d in function %s\n", i, func->name);

            // Простое дублирование тела цикла
            int loop_end = -1;
            for (int j = i + 1; j < func->ir_count; j++) {
                if (func->ir[j].op == IR_LOOP_END) {
                    loop_end = j;
                    break;
                }
            }
            if (loop_end == -1) continue;

            int loop_body_len = loop_end - i - 1;
            if (loop_body_len <= 0) continue;

            // Для примера — добавляем ещё один клон тела в конец функции
            if (func->ir_count + loop_body_len >= IR_MAX_INSTRUCTIONS) {
                printf("Cannot unroll loop: no space\n");
                continue;
            }

            for (int k = 0; k < loop_body_len; k++) {
                func->ir[func->ir_count + k] = func->ir[i + 1 + k];
            }
            func->ir_count += loop_body_len;
        }
    }
}

/**
 * Основная функция оптимизации циклов.
 */
void optimizer_loop_opt(IRFunction *func) {
    optimizer_loop_unroll(func);

    // TODO: Добавить распараллеливание, оптимизацию условий выхода
}
