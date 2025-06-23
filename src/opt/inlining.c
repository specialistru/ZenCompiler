/**
 * @file inlining.c
 * @brief Реализация функции инлайнинга.
 */

#include "inlining.h"
#include <string.h>

int inline_functions(IRInstruction *ir, int count) {
    // ⚠ Заглушка — полноценная реализация требует IR-функций и вызовов
    int inlined = 0;
    for (int i = 0; i < count; ++i) {
        if (ir[i].op == IR_CALL && strcmp(ir[i].dst, "trivial_func") == 0) {
            // Пример: заменить вызов "trivial_func" на присваивание 42
            ir[i].op = IR_ASSIGN;
            ir[i].arg1_type = ARG_IMM;
            ir[i].arg1_value = 42;
            ir[i].arg2_type = ARG_NONE;
            inlined++;
        }
    }
    return count;
}
