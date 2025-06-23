#ifndef LOOP_OPT_H
#define LOOP_OPT_H

#include "ir.h"

/**
 * @file loop_opt.h
 * @brief Интерфейс оптимизации циклов в IR.
 */

/**
 * @brief Оптимизирует циклы: удаляет, разворачивает, упрощает.
 *
 * @param ir Массив IR-инструкций.
 * @param count Количество инструкций.
 * @return Новое количество инструкций.
 */
int optimize_loops(IRInstruction *ir, int count);

#endif // LOOP_OPT_H

#ifndef LOOP_OPT_H
#define LOOP_OPT_H

#include "../ir/ir_api.h"

/**
 * @brief Оптимизации циклов IR.
 * Разворачивание циклов, распараллеливание и улучшение условий выхода.
 */
void loop_opt(IR_Function *function);

#endif // LOOP_OPT_H
