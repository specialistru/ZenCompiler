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

// Compiler/src/opt/optimizer.c
#include <stdio.h>
#include <stdbool.h>

#include "ir_api.h"

/**
 * Пример простейшей оптимизации — удаление мёртвого кода.
 * Очень упрощённая версия, на практике надо делать анализ использования.
 * @param func IR-функция для оптимизации.
 */
void optimizer_dead_code_elimination(IRFunction *func) {
    if (!func) return;

    for (int i = 0; i < func->ir_count; /* i увеличиваем внутри */) {
        IRInstruction *instr = &func->ir[i];

        // Заглушка для анализа — считаем, что инструкция с пустым dest мертва
        bool is_dead = (instr->dest_is_temp == false && instr->dest.value == 0);

        if (is_dead) {
            // Удаляем инструкцию — сдвигаем массив
            for (int j = i; j < func->ir_count - 1; j++) {
                func->ir[j] = func->ir[j + 1];
            }
            func->ir_count--;
            // i не увеличиваем, т.к. сдвинули
        } else {
            i++;
        }
    }
}

/**
 * Основной контроллер оптимизаций.
 * Вызывает поочерёдно оптимизации.
 * @param func IR-функция.
 */
void optimizer_optimize(IRFunction *func) {
    if (!func) return;

    printf("Starting optimizations for function '%s'\n", func->name);

    optimizer_dead_code_elimination(func);

    // Здесь могут быть вызовы других оптимизаций:
    // optimizer_const_fold(func);
    // optimizer_inlining(func);
    // optimizer_loop_opt(func);
    // и т.д.

    printf("Optimizations complete\n");
}


// Compiler/src/opt/optimizer.c
#include <stdio.h>
#include "ir_api.h"

// Объявления функций оптимизаций
void optimizer_dead_code_elimination(IRFunction *func);
void optimizer_const_fold(IRFunction *func);
void optimizer_inlining(IRFunction *func, IRModule *module);
void optimizer_loop_opt(IRFunction *func);

/**
 * Контроллер оптимизации, запускает все стадии по порядку.
 * @param func IR-функция для оптимизации.
 * @param module IR-модуль с доступом ко всем функциям (нужен для инлайнинга).
 */
void optimizer_optimize(IRFunction *func, IRModule *module) {
    if (!func) return;

    printf("Starting optimizations for function '%s'\n", func->name);

    optimizer_dead_code_elimination(func);
    optimizer_const_fold(func);
    optimizer_inlining(func, module);
    optimizer_loop_opt(func);

    printf("Optimizations complete\n");
}

#include "optimizer.h"
#include "dead_code_elim.h"
#include "const_fold.h"
#include "inlining.h"
#include "loop_opt.h"
#include <stdio.h>

/**
 * @brief Запуск полного цикла оптимизаций над функцией IR.
 * Оптимизации вызываются в определённом порядке для максимальной эффективности.
 */
void optimizer_run(IR_Function *function) {
    if (!function) return;

    printf("Optimizer: Starting optimizations...\n");

    // 1. Удаление мёртвого кода
    dead_code_elim(function);

    // 2. Свёртывание констант
    const_fold(function);

    // 3. Инлайнинг функций
    inlining(function);

    // 4. Оптимизация циклов
    loop_opt(function);

    // Итог: функция оптимизирована
    printf("Optimizer: Optimizations completed.\n");
}
