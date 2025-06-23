/*
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

*/

/**
 * @file inlining.c
 * @brief Производит инлайнинг тривиальных функций в IR.
 */

#include "inlining.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// Простейшая таблица функций, пригодных для инлайнинга
static const char *inlineable_funcs[] = {
    "noop",     // Без действий
    "get_one",  // Возвращает 1
    NULL
};

int inline_functions(IRInstruction *ir, int count) {
    int write_idx = 0;

    for (int i = 0; i < count; i++) {
        IRInstruction *instr = &ir[i];

        if (instr->op == IR_CALL && instr->dst) {
            // Проверяем — входит ли функция в список инлайнинга
            for (int f = 0; inlineable_funcs[f] != NULL; f++) {
                if (strcmp(instr->dst, inlineable_funcs[f]) == 0) {
                    // Инлайним GET_ONE: просто заменяем вызов на assign dst = 1
                    ir[write_idx].op = IR_ASSIGN;
                    ir[write_idx].dst = strdup(instr->dst);
                    ir[write_idx].arg1_type = ARG_IMM;
                    ir[write_idx].arg1_value = 1;
                    ir[write_idx].arg2_type = ARG_NONE;
                    write_idx++;
                    goto skip_copying_default;
                }
            }
        }
        // Без инлайнинга копируем инструкцию "как есть"
        ir[write_idx++] = ir[i];
        skip_copying_default:
            ;
    }

    return write_idx;
}
