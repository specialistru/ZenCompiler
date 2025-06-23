#ifndef IR_H
#define IR_H

#include <stdint.h>

// Типы инструкций промежуточного представления (IR)
typedef enum {
    IR_NOP,
    IR_LOAD_CONST,
    IR_LOAD_VAR,
    IR_STORE_VAR,
    IR_ADD,
    IR_SUB,
    IR_MUL,
    IR_DIV,
    IR_JUMP,
    IR_JUMP_IF_FALSE,
    IR_CALL,
    IR_RETURN,
    // Добавить по мере необходимости
} IROpcode;

// Структура одной IR-инструкции
typedef struct {
    IROpcode opcode;     // Код операции
    int operand1;        // Первый операнд (например, индекс переменной или константы)
    int operand2;        // Второй операнд (если нужен)
    int result;          // Результат операции
} IRInstruction;

// Структура программы в IR
typedef struct {
    IRInstruction *instructions;  // Массив инструкций
    int count;                    // Количество инструкций
    int capacity;                 // Вместимость массива
} IRProgram;

// Инициализация IR-программы
void ir_program_init(IRProgram *prog);

// Добавление инструкции в IR-программу
void ir_program_add(IRProgram *prog, IROpcode opcode, int operand1, int operand2, int result);

// Очистка и освобождение памяти IR-программы
void ir_program_free(IRProgram *prog);

#endif // IR_H
