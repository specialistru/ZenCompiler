#ifndef INLINING_H
#define INLINING_H

#include "ir.h"

/**
 * @file inlining.h
 * @brief Интерфейс модуля инлайнинга функций (подстановка тела вместо вызова).
 */

/**
 * @brief Выполняет инлайнинг функций на основе простой таблицы включаемых тел.
 * 
 * Подходит для небольших функций без побочных эффектов. Производит:
 * - Поиск инструкций IR_CALL
 * - Замещение последовательности IR_LOAD/IR_CALL/IR_STORE на тело функции
 * - Обновление меток и регистров
 * 
 * @param ir Массив IR-инструкций.
 * @param count Количество инструкций.
 * @return Новое количество инструкций после инлайнинга.
 */
int inline_functions(IRInstruction *ir, int count);

#endif // INLINING_H

#ifndef INLINING_H
#define INLINING_H

#include "../ir/ir_api.h"

/**
 * @brief Выполняет инлайнинг функций.
 * Вставляет тело вызываемой функции в место вызова, снижая накладные расходы.
 */
void inlining(IR_Function *function);

#endif // INLINING_H
