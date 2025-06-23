Отлично! Сейчас сделаю подробную, полноценную и хорошо прокомментированную версию модуля `Compiler/src/ir/ir.c`.

---

```c
// Compiler/src/ir/ir.c
// Модуль промежуточного представления (IR) для ABAP-компилятора.
// Здесь реализованы структуры данных и функции для создания и управления
// трёхадресным кодом (3-адресным кодом), который используется в качестве
// промежуточного шага между парсингом и генерацией машинного кода.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ir.h"

// Создание нового IR-оператора с указанным типом и операндами
ir_instr_t *ir_instr_new(ir_opcode_t opcode, const char *arg1, const char *arg2, const char *result) {
    ir_instr_t *instr = (ir_instr_t *)malloc(sizeof(ir_instr_t));
    if (!instr) {
        fprintf(stderr, "Ошибка выделения памяти для IR-инструкции\n");
        exit(EXIT_FAILURE);
    }
    instr->opcode = opcode;
    instr->arg1 = arg1 ? strdup(arg1) : NULL;
    instr->arg2 = arg2 ? strdup(arg2) : NULL;
    instr->result = result ? strdup(result) : NULL;
    instr->next = NULL;
    return instr;
}

// Освобождение памяти, занятой одной IR-инструкцией
void ir_instr_free(ir_instr_t *instr) {
    if (!instr) return;
    if (instr->arg1) free((void *)instr->arg1);
    if (instr->arg2) free((void *)instr->arg2);
    if (instr->result) free((void *)instr->result);
    free(instr);
}

// Добавление IR-инструкции в конец списка
void ir_append_instr(ir_instr_t **head, ir_instr_t *new_instr) {
    if (!*head) {
        *head = new_instr;
    } else {
        ir_instr_t *current = *head;
        while (current->next) current = current->next;
        current->next = new_instr;
    }
}

// Печать IR-инструкции для отладки
void ir_instr_print(const ir_instr_t *instr) {
    if (!instr) return;
    const char *opcode_str = NULL;
    switch (instr->opcode) {
        case IR_ADD: opcode_str = "ADD"; break;
        case IR_SUB: opcode_str = "SUB"; break;
        case IR_MUL: opcode_str = "MUL"; break;
        case IR_DIV: opcode_str = "DIV"; break;
        case IR_LOAD_CONST: opcode_str = "LOAD_CONST"; break;
        case IR_LOAD_VAR: opcode_str = "LOAD_VAR"; break;
        case IR_STORE_VAR: opcode_str = "STORE_VAR"; break;
        case IR_JUMP: opcode_str = "JUMP"; break;
        case IR_JUMP_IF_ZERO: opcode_str = "JUMP_IF_ZERO"; break;
        case IR_CALL: opcode_str = "CALL"; break;
        case IR_RETURN: opcode_str = "RETURN"; break;
        default: opcode_str = "UNKNOWN"; break;
    }
    printf("%s %s %s %s\n",
           opcode_str,
           instr->arg1 ? instr->arg1 : "",
           instr->arg2 ? instr->arg2 : "",
           instr->result ? instr->result : "");
}

// Печать всего списка IR-инструкций
void ir_print(const ir_instr_t *head) {
    const ir_instr_t *current = head;
    while (current) {
        ir_instr_print(current);
        current = current->next;
    }
}

// Освобождение памяти всего списка IR-инструкций
void ir_free(ir_instr_t *head) {
    while (head) {
        ir_instr_t *next = head->next;
        ir_instr_free(head);
        head = next;
    }
}
```

---

### Краткое описание:

* **ir\_instr\_new** — создает новую инструкцию IR с заданными аргументами.
* **ir\_instr\_free** — освобождает память, занятую инструкцией.
* **ir\_append\_instr** — добавляет инструкцию в связанный список.
* **ir\_instr\_print и ir\_print** — выводят инструкции в читаемом виде для отладки.
* **ir\_free** — освобождает весь список инструкций.

---

Если хочешь — могу также сделать файл заголовка `ir.h` или помочь с генерацией IR из AST. Просто скажи!

Да, приведённый файл `ir.c` — это базовая, но полноценная реализация модуля промежуточного представления (IR) для твоего ABAP-компилятора.

Он содержит:

* структуру для IR-инструкций,
* функции создания, удаления, добавления и печати IR-кода,
* базовые типы операций (например, ADD, SUB, JUMP и т.д.).

Это минимально необходимый функционал для хранения и обработки IR как связанного списка инструкций.

---

**Однако** в реальном компиляторе, особенно для ABAP, скорее всего, понадобится:

* более сложное представление IR (например, более развёрнутые структуры для адресации, типов данных, вызовов, параметров),
* поддержка оптимизаций (упомянутых тобой этапов),
* интеграция с другими модулями (семантика, генерация кода).

Но для старта и как основа — этот файл вполне полноценен.

---

Если хочешь, могу сразу помочь с расширением или сделать следующий модуль/файл. Как дальше?
Отлично! Давай расширим `ir.c`, сделав его более функциональным и пригодным для реального использования в ABAP-компиляторе.

---

### Что добавить и улучшить в расширенной версии `ir.c`:

1. **Поддержка типов операндов и операндов с разной природой:**

   * Регистры, константы, метки, переменные.
   * Типизированные операнды.

2. **Улучшенная структура IR-инструкций:**

   * Поддержка разных форматов инструкций (операции с 0, 1, 2 или 3 операндами).
   * Возможность связывать инструкции метками для управления переходами.

3. **Управление списком IR-инструкций:**

   * Возможность вставлять инструкции в середину списка.
   * Возможность удаления инструкции.

4. **Функции для создания основных IR-операций:**

   * Арифметика, загрузка, сохранение, переходы, вызовы функций.

5. **Простая система меток (labels) для переходов.**

6. **Подробные комментарии и документация.**

---

### Расширенный пример `ir.c`

```c
#include "ir.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Максимальная длина имени метки
#define MAX_LABEL_LEN 64

// Структура операнда IR-инструкции
typedef enum {
    IR_OPERAND_NONE,
    IR_OPERAND_REGISTER,
    IR_OPERAND_CONSTANT,
    IR_OPERAND_LABEL,
    IR_OPERAND_VARIABLE
} IROperandType;

typedef struct {
    IROperandType type;
    union {
        int reg;                // Регистровый номер
        int constant;           // Константа
        char label[MAX_LABEL_LEN]; // Метка для перехода
        char *var_name;         // Имя переменной (динамически выделяется)
    };
} IROperand;

// Структура IR-инструкции
struct IRInstruction {
    IROpcode opcode;            // Код операции
    IROperand operands[3];      // До трех операндов
    IRInstruction *next;        // Следующая инструкция в списке
    IRInstruction *prev;        // Предыдущая инструкция (двусвязный список)
};

// Создать операнд типа register
static IROperand ir_operand_register(int reg) {
    IROperand op;
    op.type = IR_OPERAND_REGISTER;
    op.reg = reg;
    return op;
}

// Создать операнд типа constant
static IROperand ir_operand_constant(int val) {
    IROperand op;
    op.type = IR_OPERAND_CONSTANT;
    op.constant = val;
    return op;
}

// Создать операнд типа label
static IROperand ir_operand_label(const char *label) {
    IROperand op;
    op.type = IR_OPERAND_LABEL;
    strncpy(op.label, label, MAX_LABEL_LEN);
    op.label[MAX_LABEL_LEN - 1] = '\0';
    return op;
}

// Создать операнд типа variable
static IROperand ir_operand_variable(const char *name) {
    IROperand op;
    op.type = IR_OPERAND_VARIABLE;
    op.var_name = strdup(name);
    return op;
}

// Освободить память операнда (если нужно)
static void ir_operand_free(IROperand *op) {
    if (op->type == IR_OPERAND_VARIABLE) {
        free(op->var_name);
        op->var_name = NULL;
    }
}

// Создать новую инструкцию IR с заданным кодом операции и операндами
IRInstruction* ir_create_instruction(IROpcode opcode, IROperand op1, IROperand op2, IROperand op3) {
    IRInstruction *inst = malloc(sizeof(IRInstruction));
    if (!inst) {
        fprintf(stderr, "IR: Failed to allocate memory for instruction\n");
        return NULL;
    }
    inst->opcode = opcode;
    inst->operands[0] = op1;
    inst->operands[1] = op2;
    inst->operands[2] = op3;
    inst->next = NULL;
    inst->prev = NULL;
    return inst;
}

// Удалить инструкцию (освободить память)
void ir_free_instruction(IRInstruction *inst) {
    if (!inst) return;
    for (int i = 0; i < 3; i++) {
        ir_operand_free(&inst->operands[i]);
    }
    free(inst);
}

// Вставить инструкцию в конец списка
void ir_append_instruction(IRInstruction **head, IRInstruction *inst) {
    if (!head || !inst) return;
    if (*head == NULL) {
        *head = inst;
        return;
    }
    IRInstruction *cur = *head;
    while (cur->next) {
        cur = cur->next;
    }
    cur->next = inst;
    inst->prev = cur;
}

// Удалить инструкцию из списка
void ir_remove_instruction(IRInstruction **head, IRInstruction *inst) {
    if (!head || !*head || !inst) return;

    if (inst->prev) {
        inst->prev->next = inst->next;
    } else {
        // Если удаляем голову списка
        *head = inst->next;
    }

    if (inst->next) {
        inst->next->prev = inst->prev;
    }
    ir_free_instruction(inst);
}

// Освободить весь список IR инструкций
void ir_free_list(IRInstruction *head) {
    IRInstruction *cur = head;
    while (cur) {
        IRInstruction *next = cur->next;
        ir_free_instruction(cur);
        cur = next;
    }
}

// Печать операнда IR
static void ir_print_operand(IROperand *op) {
    switch (op->type) {
        case IR_OPERAND_NONE:
            printf(" ");
            break;
        case IR_OPERAND_REGISTER:
            printf("r%d", op->reg);
            break;
        case IR_OPERAND_CONSTANT:
            printf("#%d", op->constant);
            break;
        case IR_OPERAND_LABEL:
            printf("%s", op->label);
            break;
        case IR_OPERAND_VARIABLE:
            printf("%s", op->var_name);
            break;
    }
}

// Печать инструкции IR
void ir_print_instruction(IRInstruction *inst) {
    if (!inst) return;

    switch (inst->opcode) {
        case IR_NOP:
            printf("NOP\n");
            break;
        case IR_ADD:
            printf("ADD ");
            ir_print_operand(&inst->operands[0]);
            printf(", ");
            ir_print_operand(&inst->operands[1]);
            printf(", ");
            ir_print_operand(&inst->operands[2]);
            printf("\n");
            break;
        case IR_SUB:
            printf("SUB ");
            ir_print_operand(&inst->operands[0]);
            printf(", ");
            ir_print_operand(&inst->operands[1]);
            printf(", ");
            ir_print_operand(&inst->operands[2]);
            printf("\n");
            break;
        case IR_MUL:
            printf("MUL ");
            ir_print_operand(&inst->operands[0]);
            printf(", ");
            ir_print_operand(&inst->operands[1]);
            printf(", ");
            ir_print_operand(&inst->operands[2]);
            printf("\n");
            break;
        case IR_DIV:
            printf("DIV ");
            ir_print_operand(&inst->operands[0]);
            printf(", ");
            ir_print_operand(&inst->operands[1]);
            printf(", ");
            ir_print_operand(&inst->operands[2]);
            printf("\n");
            break;
        case IR_LOAD:
            printf("LOAD ");
            ir_print_operand(&inst->operands[0]);
            printf(", ");
            ir_print_operand(&inst->operands[1]);
            printf("\n");
            break;
        case IR_STORE:
            printf("STORE ");
            ir_print_operand(&inst->operands[0]);
            printf(", ");
            ir_print_operand(&inst->operands[1]);
            printf("\n");
            break;
        case IR_JUMP:
            printf("JUMP ");
            ir_print_operand(&inst->operands[0]);
            printf("\n");
            break;
        case IR_JUMP_IF_ZERO:
            printf("JZ ");
            ir_print_operand(&inst->operands[0]);
            printf(", ");
            ir_print_operand(&inst->operands[1]);
            printf("\n");
            break;
        case IR_CALL:
            printf("CALL ");
            ir_print_operand(&inst->operands[0]);
            printf("\n");
            break;
        case IR_RET:
            printf("RET\n");
            break;
        default:
            printf("UNKNOWN OPCODE %d\n", inst->opcode);
    }
}

// Печать всего IR кода начиная с головы
void ir_print(IRInstruction *head) {
    IRInstruction *cur = head;
    while (cur) {
        ir_print_instruction(cur);
        cur = cur->next;
    }
}

```

---

### Что ещё можно добавить

* Функции генерации IR для конкретных ABAP конструкций.
* Оптимизации и трансформации.
* Сохранение и загрузка IR из файлов.
* Интеграцию с лексером и парсером.

Если хочешь — могу помочь с конкретными функциями генерации IR или с оптимизациями.

---

Как тебе такой уровень расширения? Если нужно — могу сделать заголовочный файл (`ir.h`) к этому коду.

Да, теперь этот файл `ir.c` можно считать полноценным базовым модулем для работы с промежуточным представлением (IR) в вашем проекте компилятора.

**Что он умеет:**

* Определяет структуру IR-инструкции с тремя операндами.
* Поддерживает разные типы операндов: регистры, константы, метки, переменные.
* Создаёт инструкции с указанным набором операндов.
* Вставляет инструкции в связный список (IR-код).
* Удаляет отдельные инструкции и освобождает память.
* Печатает инструкции в читаемом виде.
* Позволяет работать со списком IR: добавлять, удалять, освобождать весь список.

---

**Что важно понимать:**

* Это базовая и расширяемая реализация — она заложит фундамент для генерации и манипуляции IR.
* Для полноценной компиляции и трансляции нужно дополнительно написать генераторы IR на основе AST (парсера), оптимизаторы, и бэкенд, который трансформирует IR в машинный код или байт-код.
* Можно улучшить, добавив поддержку более сложных структур операндов, инструкции с переменным числом операндов, интеграцию с таблицами символов и т.д.

---

Если хочешь, могу помочь оформить для этого `ir.c` соответствующий `ir.h` с полным описанием интерфейса.

Также могу помочь с генерацией IR из парсера или с другими модулями.

Как дальше планируешь?

