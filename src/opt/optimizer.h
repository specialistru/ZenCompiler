#ifndef OPTIMIZER_H
#define OPTIMIZER_H

#include "ir.h"

/**
 * @file optimizer.h
 * @brief Интерфейс контроллера IR-оптимизаций.
 */

/**
 * @brief Выполняет все фазы оптимизации IR последовательно.
 *
 * @param ir Массив IR-инструкций.
 * @param count Указатель на количество инструкций (обновляется после оптимизаций).
 */
void optimize_ir(IRInstruction *ir, int *count);

#endif // OPTIMIZER_H

#ifndef OPTIMIZER_H
#define OPTIMIZER_H

#include "../ir/ir_api.h"

/**
 * @brief Контроллер оптимизаций IR.
 * Управляет последовательностью вызова различных оптимизаций.
 */
void optimizer_run(IR_Function *function);

#endif // OPTIMIZER_H
