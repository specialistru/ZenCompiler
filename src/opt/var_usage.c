// Compiler/src/opt/var_usage.c
#include <stdbool.h>
#include "ir_api.h"

/**
 * Структура для хранения информации о переменных.
 */
typedef struct VarUsage {
    IRArg var;
    bool is_defined;
    bool is_used;
} VarUsage;

/**
 * Анализ использования переменных в функции.
 * Помечает переменные, которые определяются (assigned) и используются.
 */
void analyze_var_usage(IRFunction *func, VarUsage *usage, int max_vars) {
    if (!func || !usage) return;

    int usage_count = 0;

    for (int i = 0; i < func->ir_count; i++) {
        IRInstruction *instr = &func->ir[i];

        // Добавляем dest как определённую переменную
        if (instr->dest_is_temp) {
            bool found = false;
            for (int j = 0; j < usage_count; j++) {
                if (usage[j].var.value == instr->dest.value) {
                    usage[j].is_defined = true;
                    found = true;
                    break;
                }
            }
            if (!found && usage_count < max_vars) {
                usage[usage_count].var = instr->dest;
                usage[usage_count].is_defined = true;
                usage[usage_count].is_used = false;
                usage_count++;
            }
        }

        // Добавляем аргументы как используемые переменные
        if (instr->arg1_is_const == false) {
            bool found = false;
            for (int j = 0; j < usage_count; j++) {
                if (usage[j].var.value == instr->arg1.value) {
                    usage[j].is_used = true;
                    found = true;
                    break;
                }
            }
            if (!found && usage_count < max_vars) {
                usage[usage_count].var = instr->arg1;
                usage[usage_count].is_defined = false;
                usage[usage_count].is_used = true;
                usage_count++;
            }
        }

        if (instr->arg2_is_const == false) {
            bool found = false;
            for (int j = 0; j < usage_count; j++) {
                if (usage[j].var.value == instr->arg2.value) {
                    usage[j].is_used = true;
                    found = true;
                    break;
                }
            }
            if (!found && usage_count < max_vars) {
                usage[usage_count].var = instr->arg2;
                usage[usage_count].is_defined = false;
                usage[usage_count].is_used = true;
                usage_count++;
            }
        }
    }

    // Вывод для отладки
    for (int i = 0; i < usage_count; i++) {
        printf("Var %d: defined=%s, used=%s\n", 
               usage[i].var.value, 
               usage[i].is_defined ? "yes" : "no", 
               usage[i].is_used ? "yes" : "no");
    }
}

