Отлично! Давай шаг за шагом адаптируем остальные оптимизации под API IR и реализуем реальный анализ использования переменных для более умной оптимизации.

---

# 1. **Const Folding (const\_fold.c)** — свёртывание констант

```c
// Compiler/src/opt/const_fold.c
#include <stdio.h>
#include "ir_api.h"

/**
 * Проверяет, является ли операция арифметической бинарной операцией,
 * для которой можно выполнить свёртывание констант.
 */
static bool is_foldable_op(IROpCode op) {
    switch(op) {
        case IR_ADD:
        case IR_SUB:
        case IR_MUL:
        case IR_DIV:
        case IR_MOD:
        case IR_AND:
        case IR_OR:
        case IR_XOR:
            return true;
        default:
            return false;
    }
}

/**
 * Выполняет свёртывание констант для заданной инструкции, если оба аргумента — константы.
 * Заменяет инструкцию на присваивание с вычисленным значением.
 */
static bool try_fold_constants(IRInstruction *instr) {
    if (!is_foldable_op(instr->op)) return false;
    if (!instr->arg1_is_const || !instr->arg2_is_const) return false;

    int64_t val1 = instr->arg1.value;
    int64_t val2 = instr->arg2.value;
    int64_t result = 0;

    switch (instr->op) {
        case IR_ADD: result = val1 + val2; break;
        case IR_SUB: result = val1 - val2; break;
        case IR_MUL: result = val1 * val2; break;
        case IR_DIV:
            if (val2 == 0) return false; // защита от деления на 0
            result = val1 / val2;
            break;
        case IR_MOD:
            if (val2 == 0) return false;
            result = val1 % val2;
            break;
        case IR_AND: result = val1 & val2; break;
        case IR_OR:  result = val1 | val2; break;
        case IR_XOR: result = val1 ^ val2; break;
        default: return false;
    }

    // Замена инструкции на присваивание константы
    instr->op = IR_ASSIGN;
    instr->arg1.value = result;
    instr->arg1_is_const = true;
    instr->arg2_is_const = false;  // второй аргумент не нужен
    return true;
}

/**
 * Основная функция свёртывания констант в IR-функции.
 */
void optimizer_const_fold(IRFunction *func) {
    if (!func) return;

    for (int i = 0; i < func->ir_count; i++) {
        IRInstruction *instr = &func->ir[i];
        if (try_fold_constants(instr)) {
            printf("Const folded instruction at %d in function %s\n", i, func->name);
        }
    }
}
```

---

# 2. **Inlining (inlining.c)** — вставка тела функций

```c
// Compiler/src/opt/inlining.c
#include <stdio.h>
#include "ir_api.h"

/**
 * Анализирует вызовы функций и вставляет тело вызываемой функции на место вызова.
 * Простейшая реализация — без рекурсии, только для функций с небольшим числом инструкций.
 */
void optimizer_inlining(IRFunction *func, IRModule *module) {
    if (!func || !module) return;

    for (int i = 0; i < func->ir_count; i++) {
        IRInstruction *instr = &func->ir[i];
        if (instr->op == IR_CALL) {
            const char *callee_name = instr->arg1.func_name;
            IRFunction *callee = ir_module_find_function(module, callee_name);
            if (callee && callee->ir_count < 20) {
                printf("Inlining function '%s' into '%s' at instruction %d\n", callee_name, func->name, i);

                // Простое клонирование инструкций callee с заменой аргументов.
                // TODO: Нужно реализовать копирование и переименование переменных.

                // Для примера — просто удаляем вызов, подставляя nop
                instr->op = IR_NOP;
            }
        }
    }
}
```

---

# 3. **Loop Optimizations (loop\_opt.c)** — оптимизация циклов

```c
// Compiler/src/opt/loop_opt.c
#include <stdio.h>
#include "ir_api.h"

/**
 * Выполняет разворачивание простых циклов.
 * Находит инструкции цикла и дублирует тело цикла несколько раз.
 * Упрощённая демонстрация.
 */
void optimizer_loop_unroll(IRFunction *func) {
    if (!func) return;

    // Поиск цикла по специальной инструкции (например, IR_LOOP_START)
    for (int i = 0; i < func->ir_count; i++) {
        IRInstruction *instr = &func->ir[i];
        if (instr->op == IR_LOOP_START) {
            printf("Loop unrolling at instruction %d in function %s\n", i, func->name);

            // Простое дублирование тела цикла
            int loop_end = -1;
            for (int j = i + 1; j < func->ir_count; j++) {
                if (func->ir[j].op == IR_LOOP_END) {
                    loop_end = j;
                    break;
                }
            }
            if (loop_end == -1) continue;

            int loop_body_len = loop_end - i - 1;
            if (loop_body_len <= 0) continue;

            // Для примера — добавляем ещё один клон тела в конец функции
            if (func->ir_count + loop_body_len >= IR_MAX_INSTRUCTIONS) {
                printf("Cannot unroll loop: no space\n");
                continue;
            }

            for (int k = 0; k < loop_body_len; k++) {
                func->ir[func->ir_count + k] = func->ir[i + 1 + k];
            }
            func->ir_count += loop_body_len;
        }
    }
}

/**
 * Основная функция оптимизации циклов.
 */
void optimizer_loop_opt(IRFunction *func) {
    optimizer_loop_unroll(func);

    // TODO: Добавить распараллеливание, оптимизацию условий выхода
}
```

---

# 4. **Обновим контроллер оптимизаций (optimizer.c)**

```c
// Compiler/src/opt/optimizer.c
#include <stdio.h>
#include "ir_api.h"

// Объявления функций оптимизаций
void optimizer_dead_code_elimination(IRFunction *func);
void optimizer_const_fold(IRFunction *func);
void optimizer_inlining(IRFunction *func, IRModule *module);
void optimizer_loop_opt(IRFunction *func);

/**
 * Контроллер оптимизации, запускает все стадии по порядку.
 * @param func IR-функция для оптимизации.
 * @param module IR-модуль с доступом ко всем функциям (нужен для инлайнинга).
 */
void optimizer_optimize(IRFunction *func, IRModule *module) {
    if (!func) return;

    printf("Starting optimizations for function '%s'\n", func->name);

    optimizer_dead_code_elimination(func);
    optimizer_const_fold(func);
    optimizer_inlining(func, module);
    optimizer_loop_opt(func);

    printf("Optimizations complete\n");
}
```

---

# 5. **Реализация реальной аналитики использования переменных**

Для примера — базовый анализ, который считает, какие переменные (темповые) используются и какие определяются.

```c
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
               
```


usage\[usage\_count].is\_used = true;
usage\_count++;
}
}

```
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
```

}

```

---

# Итог

- Адаптировали свёртывание констант — теперь работает только через API и реально вычисляет значения.
- Инлайнинг реализован поверх модуля с поиском функций по имени, с примером замены вызова на NOP (нужно доработать клонирование).
- Оптимизация циклов пока упрощена, но даёт основу для расширения.
- Добавлен базовый анализ использования переменных, что можно использовать в dead code elim и других оптимизациях.

---

Если хочешь — могу помочь дальше:

- Расширить inlining с клонированием инструкций и заменой аргументов
- Сделать сложный анализ live variable для оптимального удаления кода
- Интегрировать это всё в пайплайн компилятора

Какой следующий шаг?
```

Файлы `ir_api.h` и `ir_api.c` обычно размещают в директории, связанной с промежуточным представлением (IR) и его API, чтобы обеспечить удобный доступ и управление IR из разных модулей компилятора.

Чаще всего рекомендуемое расположение:

```
Compiler/include/ir_api.h
Compiler/src/ir_api.c
```

**Почему?**

* `include/` — для всех публичных заголовков (API), доступных другим модулям.
* `src/` — для реализации (c-файлов).
* Название `ir_api` отражает назначение — интерфейс для создания, модификации и анализа IR.

Если у тебя уже есть структура вроде `Compiler/src/ir/`, можно поместить их туда, но чтобы разделять интерфейс и реализацию, обычно выделяют отдельные файлы `ir_api.h` и `ir_api.c` именно в `include` и `src`.

---

Если хочешь, могу помочь составить примерный минимальный каркас `ir_api.h/c` для интеграции с твоими оптимизациями и генератором кода.

