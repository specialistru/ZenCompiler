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
