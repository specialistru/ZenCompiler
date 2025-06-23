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
