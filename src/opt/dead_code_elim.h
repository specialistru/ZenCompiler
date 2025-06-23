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

