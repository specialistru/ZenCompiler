#ifndef OPTIMIZER_H
#define OPTIMIZER_H

#include "ir.h"

/**
 * @file optimizer.h
 * @brief Интерфейс контроллера IR-оптимизаций.
 */

/**
 * @brief Выполняет все фазы оптимизации IR последовательно.
 *
 * @param ir Массив IR-инструкций.
 * @param count Указатель на количество инструкций (обновляется после оптимизаций).
 */
void optimize_ir(IRInstruction *ir, int *count);

#endif // OPTIMIZER_H

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
