Понял задачу — адаптируем твои текущие `ir_generator.c` и `optimizer.c` полностью под API IR. Сделаем так, чтобы генератор и оптимизатор оперировали через единый интерфейс из `ir_api.h`, а не напрямую с сырыми структурами. Это повысит гибкость и позволит легко масштабировать проект.

---

# 1. Полная адаптация `ir_generator.c`

```c
// Compiler/src/ir/ir_generator.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ir_api.h"

// Текущая функция, в которую идёт генерация IR
static IRFunction *current_function = NULL;

/**
 * Начинает генерацию новой IR-функции с заданным именем.
 * Если предыдущая функция существует — освобождает её.
 */
void ir_generator_start_function(const char *name) {
    if (current_function) {
        ir_free_function(current_function);
    }
    current_function = ir_create_function(name);
    if (!current_function) {
        fprintf(stderr, "Failed to create IR function '%s'\n", name);
        exit(EXIT_FAILURE);
    }
}

/**
 * Завершает генерацию функции, выводит IR в stdout.
 * В реальном проекте здесь можно сохранить IR или передать дальше.
 */
void ir_generator_finish_function() {
    if (!current_function) return;

    printf("Generated IR for function '%s':\n", current_function->name);
    ir_function_print(current_function);

    // Временно не освобождаем — функция может понадобиться дальше
}

/**
 * Создаёт и добавляет инструкцию в текущую функцию.
 * Полностью через API.
 */
void ir_generator_emit(IROpCode op, IRArg dest, bool dest_is_temp,
                      IRArg arg1, bool arg1_is_const,
                      IRArg arg2, bool arg2_is_const) {
    if (!current_function) {
        fprintf(stderr, "No current function for IR generation\n");
        return;
    }

    IRInstruction *instr = ir_create_instruction(op, dest, dest_is_temp, arg1, arg1_is_const, arg2, arg2_is_const);
    if (!instr) {
        fprintf(stderr, "Failed to allocate IR instruction\n");
        return;
    }

    if (!ir_function_add_instruction(current_function, instr)) {
        fprintf(stderr, "Failed to add instruction to IR function\n");
        ir_free_instruction(instr);
    }
}

/**
 * Пример генерации инструкции присваивания
 */
void ir_generator_emit_assign(IRArg dest, bool dest_is_temp, IRArg src, bool src_is_const) {
    ir_generator_emit(IR_ASSIGN, dest, dest_is_temp, src, src_is_const, (IRArg){0}, false);
}
```

---

# 2. Полная адаптация `optimizer.c`

```c
// Compiler/src/opt/optimizer.c
#include <stdio.h>
#include <stdbool.h>

#include "ir_api.h"

/**
 * Пример простейшей оптимизации — удаление мёртвого кода.
 * Очень упрощённая версия, на практике надо делать анализ использования.
 * @param func IR-функция для оптимизации.
 */
void optimizer_dead_code_elimination(IRFunction *func) {
    if (!func) return;

    for (int i = 0; i < func->ir_count; /* i увеличиваем внутри */) {
        IRInstruction *instr = &func->ir[i];

        // Заглушка для анализа — считаем, что инструкция с пустым dest мертва
        bool is_dead = (instr->dest_is_temp == false && instr->dest.value == 0);

        if (is_dead) {
            // Удаляем инструкцию — сдвигаем массив
            for (int j = i; j < func->ir_count - 1; j++) {
                func->ir[j] = func->ir[j + 1];
            }
            func->ir_count--;
            // i не увеличиваем, т.к. сдвинули
        } else {
            i++;
        }
    }
}

/**
 * Основной контроллер оптимизаций.
 * Вызывает поочерёдно оптимизации.
 * @param func IR-функция.
 */
void optimizer_optimize(IRFunction *func) {
    if (!func) return;

    printf("Starting optimizations for function '%s'\n", func->name);

    optimizer_dead_code_elimination(func);

    // Здесь могут быть вызовы других оптимизаций:
    // optimizer_const_fold(func);
    // optimizer_inlining(func);
    // optimizer_loop_opt(func);
    // и т.д.

    printf("Optimizations complete\n");
}
```

---

# 3. Краткое резюме интеграции

* `ir_generator.c` теперь **создаёт** IR функции и инструкции через API (`ir_create_function()`, `ir_create_instruction()`, `ir_function_add_instruction()`).
* `optimizer.c` принимает `IRFunction *`, изменяет её инструкции (удаляет, заменяет) напрямую в массиве `func->ir` — в идеале, можно добавить API для удаления/замены инструкций, но пока это внутренние детали.
* Вызовы генератора и оптимизатора в основном коде должны быть такими:

```c
ir_generator_start_function("main");
// Генерация IR...
ir_generator_emit_assign(...);

ir_generator_finish_function();

optimizer_optimize(current_function);
```

---

Если хочешь, могу помочь и с адаптацией других оптимизаций (`const_fold.c`, `inlining.c`, `loop_opt.c`) под API и сделать реальную аналитику использования переменных. Либо перейти к интеграции с парсером и лексером.

Что дальше?

Отлично! Давай подключим созданный API IR к двум ключевым модулям: генератору кода (`ir_generator.c`) и оптимизатору (`optimizer.c`), чтобы они работали с единым интерфейсом.

---

# 1. Подключение в генератор кода (`ir_generator.c`)

---

### Шаги:

* В начале файла подключаем `ir_api.h`.
* Заменяем прямое создание и управление IR на вызовы API.
* Для добавления инструкций используем `ir_function_add_instruction`.
* Для создания функций — `ir_create_function`.

---

### Пример изменений в `ir_generator.c`

```c
#include "ir_api.h"
// Другие нужные заголовки...

// Глобальный текущий IR-функция, в которую мы генерируем код
static IRFunction *current_function = NULL;

void ir_generator_start_function(const char *name) {
    if (current_function) {
        ir_free_function(current_function);
    }
    current_function = ir_create_function(name);
}

void ir_generator_finish_function() {
    if (!current_function) return;
    // Тут можно добавить вывод или сохранение IR
    ir_function_print(current_function);
    // Освобождать пока не будем, функция может быть нужна дальше
}

// Пример генерации IR инструкции — теперь через API
void ir_generator_emit(IROpCode op, IRArg dest, bool dest_is_temp,
                      IRArg arg1, bool arg1_is_const,
                      IRArg arg2, bool arg2_is_const) {
    IRInstruction *instr = ir_create_instruction(op, dest, dest_is_temp, arg1, arg1_is_const, arg2, arg2_is_const);
    if (!instr) {
        fprintf(stderr, "Ошибка выделения памяти для IR инструкции\n");
        return;
    }
    ir_function_add_instruction(current_function, instr);
}

// Пример: генерация простой инструкции присваивания
void ir_generator_emit_assign(IRArg dest, bool dest_is_temp, IRArg src, bool src_is_const) {
    ir_generator_emit(IR_ASSIGN, dest, dest_is_temp, src, src_is_const, (IRArg){0}, false);
}

// ... остальной код генератора будет строиться аналогично
```

---

# 2. Подключение в оптимизатор (`optimizer.c`)

---

### Цели:

* Оптимизатор должен принимать IR-функцию (типа `IRFunction *`).
* Обрабатывать `func->ir` (массив инструкций).
* Вносить изменения, добавлять/удалять/заменять инструкции через API или напрямую через массив (если динамически изменяется).

---

### Пример использования API в оптимизаторе

```c
#include "ir_api.h"
// другие заголовки

/**
 * Удаление мертвого кода — пример функции оптимизации.
 * @param func IR-функция для оптимизации.
 */
void optimizer_dead_code_elimination(IRFunction *func) {
    if (!func) return;

    // Пример: простой проход, удаляющий инструкции без использования результата
    // (реальная реализация сложнее)
    for (int i = 0; i < func->ir_count; /* i инкрементируем внутри */) {
        IRInstruction *instr = &func->ir[i];

        // Допустим, мы считаем инструкцию мертвой, если dest не используется далее
        // В реальном мире — нужен анализ использования

        bool is_dead = false; // Заглушка для анализа

        if (is_dead) {
            // Удаляем инструкцию, сдвигая остальные влево
            for (int j = i; j < func->ir_count - 1; j++) {
                func->ir[j] = func->ir[j + 1];
            }
            func->ir_count--;
            // не увеличиваем i, т.к. сдвинули
        } else {
            i++;
        }
    }
}

// Другие оптимизации подключаются аналогично, все работают с IRFunction и IRInstruction
```

---

# Итог:

* Теперь **весь IR создается, хранится и обрабатывается через единый API** из `ir_api.h`/`ir_api.c`.
* Генератор кода вызывает функции API для построения IR.
* Оптимизатор принимает IRFunction и изменяет её в рамках API или через прямой доступ к массиву инструкций.
* Это упрощает поддержку, масштабирование и добавление новых оптимизаций.

---

Если хочешь, могу помочь адаптировать твой текущий `ir_generator.c` и `optimizer.c` под этот API полностью, с реальным кодом. Или перейти к реализации конкретных оптимизаций на базе этого API.

Что предпочитаешь?

