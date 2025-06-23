/**
 * @file optimizer.c
 * @brief Управляет последовательностью применения оптимизаций к IR.
 */

#include "optimizer.h"
#include "dead_code_elim.h"
#include "const_fold.h"
#include "inlining.h"
#include "loop_opt.h"
#include <stdio.h>

void optimize_ir(IRInstruction *ir, int *count) {
    printf("[opt] Начало оптимизации (%d инструкций)\n", *count);

    const_fold(ir, *count);
    *count = eliminate_dead_code(ir, *count);
    *count = inline_functions(ir, *count);
    *count = optimize_loops(ir, *count);

    printf("[opt] Оптимизация завершена (%d инструкций)\n", *count);
}

#include "ir_api.h"
// другие заголовки

/**
 * Удаление мертвого кода — пример функции оптимизации.
 * @param func IR-функция для оптимизации.
 */
void optimizer_dead_code_elimination(IRFunction *func) {
    if (!func) return;

    // Пример: простой проход, удаляющий инструкции без использования результата
    // (реальная реализация сложнее)
    for (int i = 0; i < func->ir_count; /* i инкрементируем внутри */) {
        IRInstruction *instr = &func->ir[i];

        // Допустим, мы считаем инструкцию мертвой, если dest не используется далее
        // В реальном мире — нужен анализ использования

        bool is_dead = false; // Заглушка для анализа

        if (is_dead) {
            // Удаляем инструкцию, сдвигая остальные влево
            for (int j = i; j < func->ir_count - 1; j++) {
                func->ir[j] = func->ir[j + 1];
            }
            func->ir_count--;
            // не увеличиваем i, т.к. сдвинули
        } else {
            i++;
        }
    }
}

// Другие оптимизации подключаются аналогично, все работают с IRFunction и IRInstruction
