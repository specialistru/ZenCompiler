/*

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
*/

#ifndef IR_H
#define IR_H

#include <stdbool.h>

// Типы операций IR (пример, можно расширять под свои нужды)
typedef enum {
    IR_OP_ADD,       // Сложение
    IR_OP_SUB,       // Вычитание
    IR_OP_MUL,       // Умножение
    IR_OP_DIV,       // Деление
    IR_OP_LOAD,      // Загрузка значения
    IR_OP_STORE,     // Сохранение значения
    IR_OP_JMP,       // Безусловный переход
    IR_OP_JMP_IF,    // Условный переход
    IR_OP_CALL,      // Вызов функции
    IR_OP_RET,       // Возврат из функции
    IR_OP_NOP,       // Пустая операция (no operation)
    // Добавьте другие операции по необходимости
} ir_op_t;

// Типы операндов IR
typedef enum {
    IR_OPERAND_REGISTER,   // Регистровый операнд
    IR_OPERAND_CONSTANT,   // Константа (число)
    IR_OPERAND_LABEL,      // Метка (адрес для переходов)
    IR_OPERAND_VARIABLE,   // Переменная (символ)
} ir_operand_type_t;

// Операнд IR
typedef struct ir_operand {
    ir_operand_type_t type;  // Тип операнда
    union {
        int reg;             // Номер регистра
        int constant;        // Константное значение
        char *label;         // Имя метки
        char *var_name;      // Имя переменной
    } value;
} ir_operand_t;

// Структура инструкции IR
typedef struct ir_instruction {
    ir_op_t op;                      // Код операции
    ir_operand_t *dst;               // Операнд назначения (может быть NULL)
    ir_operand_t *src1;              // Первый исходный операнд (может быть NULL)
    ir_operand_t *src2;              // Второй исходный операнд (может быть NULL)
    struct ir_instruction *next;     // Следующая инструкция в списке
} ir_instruction_t;

// Структура списка IR-инструкций
typedef struct {
    ir_instruction_t *head;          // Голова списка инструкций
    ir_instruction_t *tail;          // Хвост списка инструкций
} ir_list_t;

/**
 * @brief Создаёт новый операнд с типом регистр и номером.
 */
ir_operand_t *ir_operand_new_register(int reg_num);

/**
 * @brief Создаёт новый операнд с типом константа.
 */
ir_operand_t *ir_operand_new_constant(int constant);

/**
 * @brief Создаёт новый операнд с типом метка.
 * @param label Имя метки (строка дублируется)
 */
ir_operand_t *ir_operand_new_label(const char *label);

/**
 * @brief Создаёт новый операнд с типом переменная.
 * @param var_name Имя переменной (строка дублируется)
 */
ir_operand_t *ir_operand_new_variable(const char *var_name);

/**
 * @brief Освобождает память, занятую операндом.
 */
void ir_operand_free(ir_operand_t *operand);

/**
 * @brief Создаёт новую IR-инструкцию с заданной операцией и операндами.
 */
ir_instruction_t *ir_instruction_new(ir_op_t op, ir_operand_t *dst, ir_operand_t *src1, ir_operand_t *src2);

/**
 * @brief Освобождает память, занятую инструкцией (включая операнды).
 */
void ir_instruction_free(ir_instruction_t *inst);

/**
 * @brief Инициализирует пустой список IR-инструкций.
 */
void ir_list_init(ir_list_t *list);

/**
 * @brief Добавляет инструкцию в конец списка IR.
 */
void ir_list_append(ir_list_t *list, ir_instruction_t *inst);

/**
 * @brief Освобождает память всего списка IR-инструкций.
 */
void ir_list_free(ir_list_t *list);

/**
 * @brief Печатает список инструкций IR для отладки.
 */
void ir_list_print(const ir_list_t *list);

#endif // IR_H
