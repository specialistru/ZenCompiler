/*
#ifndef DEAD_CODE_ELIM_H
#define DEAD_CODE_ELIM_H

#include "ir.h"

/**
 * @file dead_code_elim.h
 * @brief Интерфейс оптимизации: устранение мёртвого кода.
 */

/**
 * @brief Удаляет инструкции, не влияющие на результат выполнения.
 *
 * @param ir Массив IR-инструкций.
 * @param count Количество инструкций.
 * @return Обновлённое количество инструкций.
 */
int eliminate_dead_code(IRInstruction *ir, int count);

#endif // DEAD_CODE_ELIM_H

*/

#ifndef DEAD_CODE_ELIM_H
#define DEAD_CODE_ELIM_H

#include "ir.h"

/**
 * @file dead_code_elim.h
 * @brief Интерфейс удаления мёртвого кода из IR.
 */

/**
 * @brief Удаляет избыточные инструкции:
 *  - Пустые операции (IR_NOP)
 *  - Присваивания dst = dst (самоприсваивание)
 *  - Вызовы, чьи результаты не используются
 *
 * Использует анализ использования регистров/меток.
 *
 * @param ir Массив IR-инструкций.
 * @param count Начальное количество инструкций.
 * @return Обновлённое количество инструкций.
 */
int eliminate_dead_code(IRInstruction *ir, int count);

#endif // DEAD_CODE_ELIM_H

#ifndef DEAD_CODE_ELIM_H
#define DEAD_CODE_ELIM_H

#include "../ir/ir_api.h"

/**
 * @brief Удаляет неиспользуемые инструкции и ветвления в IR.
 * Выполняет анализ живых переменных для определения мёртвого кода.
 */
void dead_code_elim(IR_Function *function);

#endif // DEAD_CODE_ELIM_H
