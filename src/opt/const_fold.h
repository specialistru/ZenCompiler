/*
#ifndef CONST_FOLD_H
#define CONST_FOLD_H

#include "ir.h"

/**
 * @file const_fold.h
 * @brief Интерфейс константного свёртывания.
 */

/**
 * @brief Выполняет замену выражений с константами на их значения.
 *
 * @param ir Массив IR-инструкций.
 * @param count Количество инструкций.
 */
void const_fold(IRInstruction *ir, int count);

#endif // CONST_FOLD_H
*/

#ifndef CONST_FOLD_H
#define CONST_FOLD_H

#include "ir.h"

/**
 * @file const_fold.h
 * @brief Интерфейс модуля свёртывания констант IR.
 */

/**
 * @brief Выполняет свёртывание выражений с константами и упрощение выражений.
 *
 * Реализует:
 * - Арифметические операции: +, -, *, /, %
 * - Логические операции: &&, ||, !
 * - Сравнения: ==, !=, <, >, <=, >=
 * - Peephole оптимизация (x = x + 0 → x)
 *
 * @param ir Массив IR-инструкций.
 * @param count Количество инструкций.
 */
void const_fold(IRInstruction *ir, int count);

#endif // CONST_FOLD_H

#ifndef CONST_FOLD_H
#define CONST_FOLD_H

#include "../ir/ir_api.h"

/**
 * @brief Выполняет свёртывание констант в IR.
 * Вычисляет на этапе компиляции выражения с константными операндами.
 */
void const_fold(IR_Function *function);

#endif // CONST_FOLD_H

