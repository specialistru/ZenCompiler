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

