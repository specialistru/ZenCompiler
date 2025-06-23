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

