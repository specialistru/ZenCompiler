/**
 * @file optimizer.c
 * @brief Управляет последовательностью применения оптимизаций к IR.
 */

#include "optimizer.h"
#include "dead_code_elim.h"
#include "const_fold.h"
#include "inlining.h"
#include "loop_opt.h"
#include <stdio.h>

void optimize_ir(IRInstruction *ir, int *count) {
    printf("[opt] Начало оптимизации (%d инструкций)\n", *count);

    const_fold(ir, *count);
    *count = eliminate_dead_code(ir, *count);
    *count = inline_functions(ir, *count);
    *count = optimize_loops(ir, *count);

    printf("[opt] Оптимизация завершена (%d инструкций)\n", *count);
}
