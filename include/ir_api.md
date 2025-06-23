Понял! Сделаем подробный API для создания, управления и работы с IR-инструкциями и функциями. Он будет универсальным и удобным для использования в оптимизациях и генерации кода.

---

## Полный пример API для IR: `ir_api.h` и `ir_api.c`

---

### 1. Заголовочный файл `ir_api.h`

```c
#ifndef IR_API_H
#define IR_API_H

#include "ir.h"

/**
 * @file ir_api.h
 * @brief API для создания и управления промежуточным представлением (IR).
 */

/**
 * @brief Создать новую IR-инструкцию.
 * @param op Код операции.
 * @param dest Целевая переменная (куда пишем результат).
 * @param dest_is_temp Флаг: dest — временная переменная (true) или нет.
 * @param arg1 Первый аргумент.
 * @param arg1_is_const Флаг, указывающий, является ли arg1 константой.
 * @param arg2 Второй аргумент.
 * @param arg2_is_const Флаг, указывающий, является ли arg2 константой.
 * @return Указатель на выделенную IR-инструкцию (нужно free после использования).
 */
IRInstruction *ir_create_instruction(
    IROpCode op,
    IRArg dest, bool dest_is_temp,
    IRArg arg1, bool arg1_is_const,
    IRArg arg2, bool arg2_is_const
);

/**
 * @brief Освободить память, занятую IR-инструкцией.
 * @param instr Указатель на инструкцию.
 */
void ir_free_instruction(IRInstruction *instr);

/**
 * @brief Создать новую IR-функцию с заданным именем.
 * @param name Имя функции (копируется).
 * @return Указатель на новую функцию.
 */
IRFunction *ir_create_function(const char *name);

/**
 * @brief Освободить IR-функцию и все связанные с ней данные.
 * @param func Указатель на функцию.
 */
void ir_free_function(IRFunction *func);

/**
 * @brief Добавить инструкцию в функцию.
 * @param func Функция, в которую добавляем.
 * @param instr Инструкция.
 */
void ir_function_add_instruction(IRFunction *func, IRInstruction *instr);

/**
 * @brief Вывести IR-функцию в человекочитаемом виде (для отладки).
 * @param func Функция для вывода.
 */
void ir_function_print(const IRFunction *func);

#endif // IR_API_H
```

---

### 2. Реализация `ir_api.c`

```c
#include "ir_api.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

IRInstruction *ir_create_instruction(
    IROpCode op,
    IRArg dest, bool dest_is_temp,
    IRArg arg1, bool arg1_is_const,
    IRArg arg2, bool arg2_is_const
) {
    IRInstruction *instr = (IRInstruction*)malloc(sizeof(IRInstruction));
    if (!instr) return NULL;

    instr->op = op;
    instr->dest = dest;
    instr->dest_is_temp = dest_is_temp;

    instr->arg1 = arg1;
    instr->arg1_is_const = arg1_is_const;

    instr->arg2 = arg2;
    instr->arg2_is_const = arg2_is_const;

    return instr;
}

void ir_free_instruction(IRInstruction *instr) {
    if (!instr) return;

    // Если dest, arg1 или arg2 — строковые данные (например, func_name), их нужно отдельно освобождать
    // В данном примере предполагаем, что строки выделяются и освобождаются отдельно,
    // или указываем, что вызывающий код отвечает за управление памятью строк.
    free(instr);
}

IRFunction *ir_create_function(const char *name) {
    IRFunction *func = (IRFunction*)malloc(sizeof(IRFunction));
    if (!func) return NULL;

    func->name = strdup(name);
    func->ir = NULL;
    func->ir_count = 0;

    return func;
}

void ir_free_function(IRFunction *func) {
    if (!func) return;

    free(func->name);
    for (int i = 0; i < func->ir_count; i++) {
        ir_free_instruction(&func->ir[i]);
    }
    free(func->ir);
    free(func);
}

void ir_function_add_instruction(IRFunction *func, IRInstruction *instr) {
    if (!func || !instr) return;

    IRInstruction *new_ir = realloc(func->ir, sizeof(IRInstruction) * (func->ir_count + 1));
    if (!new_ir) {
        // Ошибка выделения памяти — оставляем функцию без изменений
        return;
    }

    func->ir = new_ir;
    func->ir[func->ir_count] = *instr; // Копируем инструкцию
    func->ir_count++;

    free(instr); // Освободим внешнюю копию, так как мы сделали копию внутри
}

static const char *ir_opcode_to_string(IROpCode op) {
    switch (op) {
        case IR_NOP: return "NOP";
        case IR_ASSIGN: return "ASSIGN";
        case IR_ADD: return "ADD";
        case IR_SUB: return "SUB";
        case IR_MUL: return "MUL";
        case IR_DIV: return "DIV";
        case IR_MOD: return "MOD";
        case IR_AND: return "AND";
        case IR_OR: return "OR";
        case IR_NOT: return "NOT";
        case IR_EQ: return "EQ";
        case IR_NEQ: return "NEQ";
        case IR_LT: return "LT";
        case IR_LE: return "LE";
        case IR_GT: return "GT";
        case IR_GE: return "GE";
        case IR_CALL: return "CALL";
        case IR_RET: return "RET";
        case IR_LOOP_START: return "LOOP_START";
        case IR_LOOP_END: return "LOOP_END";
        case IR_LABEL: return "LABEL";
        case IR_JMP: return "JMP";
        case IR_JMP_IF: return "JMP_IF";
        default: return "UNKNOWN";
    }
}

static void ir_print_arg(IRArg arg, bool is_const, bool is_temp) {
    if (is_const) {
        printf("%ld", arg.value);
    } else if (is_temp) {
        printf("t%d", arg.temp_id);
    } else if (arg.func_name) {
        printf("%s", arg.func_name);
    } else if (arg.label_name) {
        printf("%s", arg.label_name);
    } else {
        printf("?");
    }
}

void ir_function_print(const IRFunction *func) {
    if (!func) return;

    printf("Function %s:\n", func->name);
    for (int i = 0; i < func->ir_count; i++) {
        IRInstruction *instr = &func->ir[i];
        printf("  %3d: %s ", i, ir_opcode_to_string(instr->op));

        // Выводим dest
        if (instr->dest_is_temp) {
            printf("t%d = ", instr->dest.temp_id);
        } else if (instr->dest.func_name) {
            printf("%s = ", instr->dest.func_name);
        }

        // Аргументы в зависимости от оператора
        switch (instr->op) {
            case IR_NOT:
            case IR_RET:
            case IR_LABEL:
                ir_print_arg(instr->arg1, instr->arg1_is_const, !instr->arg1_is_const);
                break;

            case IR_CALL:
                printf("%s(", instr->arg1.func_name ? instr->arg1.func_name : "func");
                // Для простоты не печатаем аргументы вызова — можно добавить, если надо
                printf(")");
                break;

            default:
                ir_print_arg(instr->arg1, instr->arg1_is_const, !instr->arg1_is_const);
                if (instr->op != IR_NOP && instr->op != IR_RET && instr->op != IR_LABEL) {
                    printf(", ");
                    ir_print_arg(instr->arg2, instr->arg2_is_const, !instr->arg2_is_const);
                }
                break;
        }
        printf("\n");
    }
}
```

---

## Объяснение:

* `ir_create_instruction` — создает инструкцию с полным набором аргументов и флагов.
* `ir_free_instruction` — освобождает инструкцию.
* `ir_create_function` / `ir_free_function` — создание и удаление функции IR.
* `ir_function_add_instruction` — добавляет инструкцию в функцию (динамически расширяет массив).
* `ir_function_print` — удобный вывод для отладки.
* Строковые представления операций для легкого чтения.

---

## Следующий шаг:

* Подключить эти структуры и API в генератор кода и оптимизатор.
* Реализовать операции создания IR в парсере, чтобы генерировать этот IR.
* Создать модули оптимизаций, которые будут использовать API для перебора и модификации IR.

---

Если хочешь — могу помочь с этими шагами или сразу перейти к написанию одного из оптимизаторов (например, удаления мертвого кода).

---

Готов двигаться дальше?
